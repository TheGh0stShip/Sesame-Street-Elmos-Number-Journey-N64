#!/usr/bin/env python3
"""Table-driven RSP (Reality Signal Processor) disassembler + assembler pair, used to give the five microcode
blobs of both games (rspboot, gspF3DEX 1.23 text/data, audio microcode text/data) a source form.

usage: rsp_g2.py extract ROM UCODE_JSON OUTDIR   -> OUTDIR/<game>/<blob>.s for each blob
       rsp_g2.py check   ROM UCODE_JSON OUTDIR   -> assemble every .s and compare with the ROM bytes
UCODE_JSON = sesassets/ucode/out/ucode_<game>/ucode.json (ROM offsets and sizes of the blobs).

Source syntax (see ../FORMAT.md): `.imem 0xADDR` / `.dmem 0xADDR` sets the address of the first word; labels
`L_xxxx:`; scalar MIPS ops with numeric registers ($0-$31); COP2 moves `mfc2 $rt, $vS[e]`; vector ops
`vmulf $vD, $vS, $vT[e]` (all 4 fields always written, so every encoding round-trips); vector loads/stores
`lqv $vT[e], off($base)` with the byte offset (7-bit field times the access size); `.word`, `.ascii`.
Any word whose decoding does not re-encode to itself is written as `.word` (counted).
"""
import sys, os, json, re, struct

SPECIAL = {0x00: ('sll', 'sh'), 0x02: ('srl', 'sh'), 0x03: ('sra', 'sh'), 0x04: ('sllv', 'shv'), 0x06: ('srlv', 'shv'),
           0x07: ('srav', 'shv'), 0x08: ('jr', 'jr'), 0x09: ('jalr', 'jalr'), 0x0D: ('break', 'brk'),
           0x20: ('add', 'r3'), 0x21: ('addu', 'r3'), 0x22: ('sub', 'r3'), 0x23: ('subu', 'r3'), 0x24: ('and', 'r3'),
           0x25: ('or', 'r3'), 0x26: ('xor', 'r3'), 0x27: ('nor', 'r3'), 0x2A: ('slt', 'r3'), 0x2B: ('sltu', 'r3')}
REGIMM = {0x00: 'bltz', 0x01: 'bgez', 0x10: 'bltzal', 0x11: 'bgezal'}
OPS = {0x02: ('j', 'j'), 0x03: ('jal', 'j'), 0x04: ('beq', 'b2'), 0x05: ('bne', 'b2'), 0x06: ('blez', 'b1'),
       0x07: ('bgtz', 'b1'), 0x08: ('addi', 'is'), 0x09: ('addiu', 'is'), 0x0A: ('slti', 'is'), 0x0B: ('sltiu', 'is'),
       0x0C: ('andi', 'iu'), 0x0D: ('ori', 'iu'), 0x0E: ('xori', 'iu'), 0x0F: ('lui', 'lui'),
       0x20: ('lb', 'mem'), 0x21: ('lh', 'mem'), 0x23: ('lw', 'mem'), 0x24: ('lbu', 'mem'), 0x25: ('lhu', 'mem'),
       0x28: ('sb', 'mem'), 0x29: ('sh', 'mem'), 0x2B: ('sw', 'mem')}
COP0 = {0x00: 'mfc0', 0x04: 'mtc0'}
COP2 = {0x00: 'mfc2', 0x02: 'cfc2', 0x04: 'mtc2', 0x06: 'ctc2'}
VOPS = {0x00: 'vmulf', 0x01: 'vmulu', 0x02: 'vrndp', 0x03: 'vmulq', 0x04: 'vmudl', 0x05: 'vmudm', 0x06: 'vmudn',
        0x07: 'vmudh', 0x08: 'vmacf', 0x09: 'vmacu', 0x0A: 'vrndn', 0x0B: 'vmacq', 0x0C: 'vmadl', 0x0D: 'vmadm',
        0x0E: 'vmadn', 0x0F: 'vmadh', 0x10: 'vadd', 0x11: 'vsub', 0x13: 'vabs', 0x14: 'vaddc', 0x15: 'vsubc',
        0x1D: 'vsar', 0x20: 'vlt', 0x21: 'veq', 0x22: 'vne', 0x23: 'vge', 0x24: 'vcl', 0x25: 'vch', 0x26: 'vcr',
        0x27: 'vmrg', 0x28: 'vand', 0x29: 'vnand', 0x2A: 'vor', 0x2B: 'vnor', 0x2C: 'vxor', 0x2D: 'vnxor',
        0x30: 'vrcp', 0x31: 'vrcpl', 0x32: 'vrcph', 0x33: 'vmov', 0x34: 'vrsq', 0x35: 'vrsql', 0x36: 'vrsqh',
        0x37: 'vnop'}
# LWC2/SWC2 sub-op (rd field): name suffix, access size used to scale the 7-bit offset
VMEM = {0x00: ('bv', 1), 0x01: ('sv', 2), 0x02: ('lv', 4), 0x03: ('dv', 8), 0x04: ('qv', 16), 0x05: ('rv', 16),
        0x06: ('pv', 8), 0x07: ('uv', 8), 0x08: ('hv', 16), 0x09: ('fv', 16), 0x0A: ('wv', 16), 0x0B: ('tv', 16)}

BYNAME = {}
for k, (n, f) in SPECIAL.items(): BYNAME[n] = ('special', k, f)
for k, n in REGIMM.items(): BYNAME[n] = ('regimm', k, 'b1')
for k, (n, f) in OPS.items(): BYNAME[n] = ('op', k, f)
for k, n in COP0.items(): BYNAME[n] = ('cop0', k, 'c0')
for k, n in COP2.items(): BYNAME[n] = ('cop2', k, 'c2')
for k, n in VOPS.items(): BYNAME[n] = ('vop', k, 'v')
for k, (s, sz) in VMEM.items():
    BYNAME['l' + s] = ('lwc2', k, 'vm'); BYNAME['s' + s] = ('swc2', k, 'vm')
VSIZE = {k: sz for k, (s, sz) in VMEM.items()}

def sx(v, b): return v - (1 << b) if v & (1 << (b - 1)) else v

def decode(w, pc, labels):
    """-> (text, branch_target or None) or None if not decodable."""
    op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; rd = (w >> 11) & 31; sa = (w >> 6) & 31; fn = w & 63
    imm = w & 0xFFFF
    def lab(t): return labels.get(t, '0x%X' % t)
    if op == 0:
        if fn not in SPECIAL: return None
        n, f = SPECIAL[fn]
        if f == 'sh': return '%s $%d, $%d, %d' % (n, rd, rt, sa)
        if f == 'shv': return '%s $%d, $%d, $%d' % (n, rd, rt, rs)
        if f == 'jr': return 'jr $%d' % rs
        if f == 'jalr': return 'jalr $%d, $%d' % (rd, rs)
        if f == 'brk': return 'break 0x%X' % ((w >> 6) & 0xFFFFF)
        return '%s $%d, $%d, $%d' % (n, rd, rs, rt)
    if op == 1:
        if rt not in REGIMM: return None
        return '%s $%d, %s' % (REGIMM[rt], rs, lab(pc + 4 + 4 * sx(imm, 16)))
    if op in (0x10, 0x12):
        if op == 0x12 and rs & 0x10:
            if fn not in VOPS: return None
            return '%s $v%d, $v%d, $v%d[%d]' % (VOPS[fn], sa, rd, rt, rs & 15)
        tab = COP0 if op == 0x10 else COP2
        if rs not in tab: return None
        if op == 0x10: return '%s $%d, $c%d' % (tab[rs], rt, rd)
        if rs in (0, 4): return '%s $%d, $v%d[%d]' % (tab[rs], rt, rd, (w >> 7) & 15)
        return '%s $%d, $vc%d' % (tab[rs], rt, rd)
    if op in (0x32, 0x3A):
        if rd not in VMEM: return None
        s, sz = VMEM[rd]
        return '%s%s $v%d[%d], %d($%d)' % ('l' if op == 0x32 else 's', s, rt, (w >> 7) & 15, sx(w & 127, 7) * sz, rs)
    if op not in OPS: return None
    n, f = OPS[op]
    if f == 'j': return '%s %s' % (n, lab((w & 0x3FFFFFF) << 2))
    if f == 'b2': return '%s $%d, $%d, %s' % (n, rs, rt, lab(pc + 4 + 4 * sx(imm, 16)))
    if f == 'b1': return '%s $%d, %s' % (n, rs, lab(pc + 4 + 4 * sx(imm, 16)))
    if f == 'is': return '%s $%d, $%d, %d' % (n, rt, rs, sx(imm, 16))
    if f == 'iu': return '%s $%d, $%d, 0x%X' % (n, rt, rs, imm)
    if f == 'lui': return 'lui $%d, 0x%X' % (rt, imm)
    if f == 'mem': return '%s $%d, %d($%d)' % (n, rt, sx(imm, 16), rs)

def targets(w, pc):
    op = w >> 26
    if op == 1 or 4 <= op <= 7: return pc + 4 + 4 * sx(w & 0xFFFF, 16)
    if op in (2, 3): return (w & 0x3FFFFFF) << 2

def reg(t, p='$'):
    t = t.strip()
    assert t.startswith(p), t
    return int(t[len(p):])

def num(t, labels):
    t = t.strip()
    if t in labels: return labels[t]
    return int(t, 0)

def encode(line, pc, labels):
    m = line.split(None, 1); n = m[0]; args = [a.strip() for a in m[1].split(',')] if len(m) > 1 else []
    kind, code, f = BYNAME[n]
    if kind == 'special':
        w = code
        if f == 'sh': w |= reg(args[0]) << 11 | reg(args[1]) << 16 | int(args[2]) << 6
        elif f == 'shv': w |= reg(args[0]) << 11 | reg(args[1]) << 16 | reg(args[2]) << 21
        elif f == 'jr': w |= reg(args[0]) << 21
        elif f == 'jalr': w |= reg(args[0]) << 11 | reg(args[1]) << 21
        elif f == 'brk': w |= int(args[0], 0) << 6
        else: w |= reg(args[0]) << 11 | reg(args[1]) << 21 | reg(args[2]) << 16
        return w
    def br(t): return ((num(t, labels) - pc - 4) >> 2) & 0xFFFF
    if kind == 'regimm': return 1 << 26 | reg(args[0]) << 21 | code << 16 | br(args[1])
    if kind == 'cop0': return 0x10 << 26 | code << 21 | reg(args[0]) << 16 | reg(args[1], '$c') << 11
    if kind == 'cop2':
        v = re.match(r'\$(vc?)(\d+)(?:\[(\d+)\])?', args[1])
        return 0x12 << 26 | code << 21 | reg(args[0]) << 16 | int(v.group(2)) << 11 | int(v.group(3) or 0) << 7
    if kind == 'vop':
        t = re.match(r'\$v(\d+)\[(\d+)\]', args[2])
        return 0x12 << 26 | 1 << 25 | int(t.group(2)) << 21 | int(t.group(1)) << 16 | reg(args[1], '$v') << 11 | reg(args[0], '$v') << 6 | code
    if kind in ('lwc2', 'swc2'):
        t = re.match(r'\$v(\d+)\[(\d+)\]', args[0]); o = re.match(r'(-?\d+)\(\$(\d+)\)', args[1])
        off = int(o.group(1)); sz = VSIZE[code]; assert off % sz == 0
        return (0x32 if kind == 'lwc2' else 0x3A) << 26 | int(o.group(2)) << 21 | int(t.group(1)) << 16 | code << 11 | int(t.group(2)) << 7 | ((off // sz) & 127)
    w = code << 26
    if f == 'j': return w | (num(args[0], labels) >> 2) & 0x3FFFFFF
    if f == 'b2': return w | reg(args[0]) << 21 | reg(args[1]) << 16 | br(args[2])
    if f == 'b1': return w | reg(args[0]) << 21 | br(args[1])
    if f in ('is', 'iu'): return w | reg(args[1]) << 21 | reg(args[0]) << 16 | (int(args[2], 0) & 0xFFFF)
    if f == 'lui': return w | reg(args[0]) << 16 | int(args[1], 0)
    if f == 'mem':
        o = re.match(r'(-?\d+)\(\$(\d+)\)', args[1])
        return w | int(o.group(2)) << 21 | reg(args[0]) << 16 | (int(o.group(1)) & 0xFFFF)

def disasm_text(blob, base, title):
    n = len(blob) // 4; ws = struct.unpack('>%dI' % n, blob)
    labels = {}
    for i, w in enumerate(ws):
        t = targets(w, base + 4 * i)
        if t is not None and base <= t < base + len(blob): labels[t] = 'L_%04X' % (t & 0xFFFF)
    out = ['# %s: RSP IMEM text, %d bytes' % (title, len(blob)), '.imem 0x%X' % base]
    nword = 0
    for i, w in enumerate(ws):
        pc = base + 4 * i
        if pc in labels: out.append(labels[pc] + ':')
        t = decode(w, pc, labels)
        if t is not None:
            try:
                ok = encode(t, pc, {v: k for k, v in labels.items()}) == w
            except Exception:
                ok = False
            if not ok: t = None
        if t is None:
            nword += 1; out.append('    .word 0x%08X' % w)
        else:
            out.append('    %-36s # %04X: %08X' % (t, pc & 0xFFFF, w))
    return '\n'.join(out) + '\n', nword

def disasm_data(blob, base, title):
    """Data as typed lines: printable NUL-terminated strings of >= 8 chars as .ascii, the rest as .word."""
    out = ['# %s: RSP DMEM data, %d bytes' % (title, len(blob)), '.dmem 0x%X' % base]
    i = 0
    strs = [(m.start(), m.end()) for m in re.finditer(rb'[\x20-\x7e]{8,}\x00*', blob)]
    while i < len(blob):
        s = next((a for a in strs if a[0] <= i < a[1]), None)
        if s and s[0] % 4 == 0 and i == s[0]:
            e = s[1]
            out.append('    .ascii %s    # %04X' % (json.dumps(blob[i:e].decode('latin1')), i)); i = e; continue
        if i % 4:
            out.append('    .byte ' + ', '.join('0x%02X' % b for b in blob[i:i + (4 - i % 4)])); i += 4 - i % 4; continue
        nxt = min([a[0] for a in strs if a[0] > i and a[0] % 4 == 0] + [len(blob)])
        k = min(i + 16, nxt - (nxt - i) % 4) if nxt - i >= 4 else i + 4
        if k <= i: k = i + 4
        ws = blob[i:k]
        out.append('    .word ' + ', '.join('0x%08X' % struct.unpack_from('>I', ws, j)[0] for j in range(0, len(ws) - len(ws) % 4, 4)) + '    # %04X' % i)
        i = k
    return '\n'.join(out) + '\n'

def assemble(src):
    lines = [l.split('#', 1)[0].rstrip() if not l.lstrip().startswith('.ascii') else l.rsplit('    #', 1)[0] for l in src.split('\n')]
    for p in (0, 1):
        pc = 0; labels = {}; out = bytearray() if p else None
        for l in lines:
            s = l.strip()
            if not s: continue
            if s.startswith('.imem') or s.startswith('.dmem'): pc = int(s.split()[1], 0); continue
            if s.endswith(':'): labels[s[:-1]] = pc; continue
            if s.startswith('.ascii'):
                b = json.loads(s[6:].strip()).encode('latin1')
            elif s.startswith('.word'):
                b = b''.join(struct.pack('>I', int(x, 0)) for x in s[5:].split(','))
            elif s.startswith('.byte'):
                b = bytes(int(x, 0) for x in s[5:].split(','))
            else:
                b = struct.pack('>I', encode(s, pc, lab) if p else 0)
            if p: out += b
            pc += len(b)
        lab = labels
    return bytes(out)

BLOBS = [('boot', 'rspboot', 0x04001000, 'text'), ('gfx_text', 'gspF3DEX.text', 0x04001080, 'text'),
         ('gfx_data', 'gspF3DEX.data', 0x04000000, 'data'), ('aud_text', 'aspMain.text', 0x04001080, 'text'),
         ('aud_data', 'aspMain.data', 0x04000000, 'data')]

def main():
    mode, romp, uj, od = sys.argv[1:5]
    rom = open(romp, 'rb').read(); u = json.load(open(uj)); g = u['game']
    gd = os.path.join(od, g); os.makedirs(gd, exist_ok=True); bad = 0
    for key, name, base, kind in BLOBS:
        a = int(u[key]['rom'], 16); b = int(u[key]['rom_end'], 16); blob = rom[a:b]
        p = os.path.join(gd, name + '.s')
        if mode == 'extract':
            if kind == 'text': src, nw = disasm_text(blob, base, name)
            else: src, nw = disasm_data(blob, base, name), 0
            open(p, 'w').write(src)
            print(g, name, len(blob), 'bytes;', 'words left as .word:', nw)
        got = assemble(open(p).read())
        ok = got == blob; bad += not ok
        print(g, name, 'rom 0x%X-0x%X' % (a, b), 'OK' if ok else 'MISMATCH')
    sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
