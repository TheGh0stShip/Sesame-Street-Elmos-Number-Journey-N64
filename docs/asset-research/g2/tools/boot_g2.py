#!/usr/bin/env python3
"""ROM header (0x00-0x40) as named fields and IPL3 (0x40-0x1000) as labelled VR4300 assembly.

usage: boot_g2.py extract ROM OUTDIR   -> OUTDIR/<game>/header.json, OUTDIR/<game>/ipl3.s
       boot_g2.py check   ROM OUTDIR   -> header.json -> 64 bytes (CRC1/CRC2 recomputed over the ROM's
                                          0x1000-0x101000), ipl3.s -> GNU as + ld + objcopy -> 4032 bytes;
                                          both compared with the ROM.
The IPL3 runs from SP DMEM at 0xA4000040. Disassembly: rabbitizer (lab venv), pseudo-instructions off, every
branch/jump target inside the block gets a label. Any word that the assembler does not encode back to the same
word is written as `.word` (with the disassembly as a comment); the count is printed.
"""
import sys, os, json, struct, subprocess, zlib, tempfile

BIN = os.path.expanduser('${N64DK_LAB}/toolroot/mips-binutils/usr/bin/mips-linux-gnu-')
IPL_VRAM = 0xA4000040
ENV = dict(os.environ, LD_LIBRARY_PATH=os.path.expanduser('${N64DK_LAB}/toolroot/mips-binutils/usr/lib/x86_64-linux-gnu'))

FIELDS = [  # off, size, name, kind
    (0x00, 4, 'pi_bsd_dom1_config', 'u32'), (0x04, 4, 'clock_rate', 'u32'), (0x08, 4, 'boot_address', 'u32'),
    (0x0C, 4, 'libultra_release', 'u32'), (0x10, 4, 'crc1', 'crc'), (0x14, 4, 'crc2', 'crc'),
    (0x18, 8, 'reserved_18', 'zero'), (0x20, 20, 'internal_name', 'ascii'), (0x34, 7, 'reserved_34', 'zero'),
    (0x3B, 1, 'media_category', 'ascii'), (0x3C, 2, 'game_id', 'ascii'), (0x3E, 1, 'region', 'ascii'),
    (0x3F, 1, 'revision', 'u8')]

def rol(v, n): return ((v << n) | (v >> (32 - n))) & 0xFFFFFFFF if n else v

def crc6102(rom):
    t1 = t2 = t3 = t4 = t5 = t6 = 0xF8CA4DDC
    words = struct.unpack_from('>%dI' % 0x40000, rom, 0x1000)
    for d in words:
        r = (t6 + d) & 0xFFFFFFFF
        if r < t6: t4 = (t4 + 1) & 0xFFFFFFFF
        t6 = r; t3 ^= d
        r = rol(d, d & 31); t5 = (t5 + r) & 0xFFFFFFFF
        t2 = t2 ^ r if t2 > d else t2 ^ t6 ^ d
        t1 = (t1 + (t5 ^ d)) & 0xFFFFFFFF
    return t6 ^ t4 ^ t3, t5 ^ t2 ^ t1

def game_of(rom): return 'letter' if b'Letter' in rom[0x20:0x34] else 'number'

def header_fields(rom):
    out = []
    for off, n, name, kind in FIELDS:
        b = rom[off:off + n]
        f = dict(off=off, size=n, name=name, kind=kind)
        if kind in ('u32', 'u8'): f['value'] = '0x%X' % int.from_bytes(b, 'big')
        elif kind == 'ascii': f['value'] = b.decode('ascii')
        elif kind == 'zero': assert b == bytes(n), name
        out.append(f)
    return out

def header_bytes(fields, rom):
    h = bytearray(0x40)
    c1, c2 = crc6102(rom)
    for f in fields:
        o, n = f['off'], f['size']
        if f['kind'] in ('u32', 'u8'): h[o:o + n] = int(f['value'], 16).to_bytes(n, 'big')
        elif f['kind'] == 'ascii': h[o:o + n] = f['value'].encode('ascii')
        elif f['kind'] == 'crc': h[o:o + n] = struct.pack('>I', c1 if f['name'] == 'crc1' else c2)
    return bytes(h)

def disasm_ipl3(blob, forced_words=()):
    import rabbitizer as R
    R.config.pseudos_enablePseudos = False
    R.config.regNames_vr4300Cop0NamedRegisters = False
    R.config.misc_unknownInstrComment = False
    n = len(blob) // 4
    ins = [R.Instruction(struct.unpack_from('>I', blob, 4 * i)[0], IPL_VRAM + 4 * i) for i in range(n)]
    labels = {}
    for i in ins:
        t = None
        if i.isBranch(): t = i.getBranchVramGeneric()
        elif i.isJumpWithAddress(): t = i.getInstrIndexAsVram() | (i.vram & 0xF0000000)
        if t is not None and IPL_VRAM <= t < IPL_VRAM + len(blob) and t % 4 == 0:
            labels[t] = 'L_%08X' % t
    lines = ['# IPL3 (CIC-NUS-6102 boot stage 3), runs from SP DMEM at 0x%08X; %d words' % (IPL_VRAM, n),
             '.set noreorder', '.set noat', '.set nomacro', '.text', '.globl ipl3_entry', 'ipl3_entry:']
    # code ends at the first word that is not a valid instruction (0xA4000B70 in the 6102 IPL3); the rest is
    # a data table, written as typed words
    dstart = next((k for k, i in enumerate(ins) if not i.isValid()), n)
    for k, i in enumerate(ins):
        if k == dstart:
            lines.append('ipl3_data:    # 0x%08X..0x%08X: data (%d bytes)' % (i.vram, IPL_VRAM + len(blob), len(blob) - 4 * k))
            ws = [x.getRaw() for x in ins[k:]]
            for j in range(0, len(ws), 4):
                lines.append('    .word ' + ', '.join('0x%08X' % w for w in ws[j:j + 4]))
            break
        if i.vram in labels: lines.append(labels[i.vram] + ':')
        raw = i.getRaw()
        if not i.isValid() or k in forced_words:
            lines.append('    .word 0x%08X    # %s' % (raw, i.disassemble().strip() if i.isValid() else 'data'))
            continue
        imm = None
        if i.isBranch(): imm = labels.get(i.getBranchVramGeneric())
        elif i.isJumpWithAddress():
            t = i.getInstrIndexAsVram() | (i.vram & 0xF0000000)
            imm = labels.get(t, '0x%08X' % t)
        txt = i.disassemble(immOverride=imm) if imm else i.disassemble()
        lines.append('    %-40s # %08X: %08X' % (txt, i.vram, raw))
    return '\n'.join(lines) + '\n'

def assemble(src, base, tag):
    d = tempfile.mkdtemp(prefix='g2as_')
    s, o, e, b, ld = (os.path.join(d, x) for x in (tag + '.s', tag + '.o', tag + '.elf', tag + '.bin', tag + '.ld'))
    open(s, 'w').write(src)
    open(ld, 'w').write('SECTIONS { . = 0x%X; .text : { *(.text) } /DISCARD/ : { *(*) } }\n' % base)
    subprocess.run([BIN + 'as', '-EB', '-march=vr4300', '-mabi=32', '-mno-shared', '-call_nonpic', '-o', o, s], check=True, env=ENV)
    subprocess.run([BIN + 'ld', '-EB', '-T', ld, '-o', e, o], check=True, env=ENV)
    subprocess.run([BIN + 'objcopy', '-O', 'binary', '-j', '.text', e, b], check=True, env=ENV)
    return open(b, 'rb').read()

def main():
    mode, romp, od = sys.argv[1:4]
    rom = open(romp, 'rb').read(); g = game_of(rom); gd = os.path.join(od, g); os.makedirs(gd, exist_ok=True)
    ipl = rom[0x40:0x1000]
    if mode == 'extract':
        json.dump(dict(game=g, cic='6102', ipl3_crc32='%08X' % zlib.crc32(ipl), fields=header_fields(rom)),
                  open(os.path.join(gd, 'header.json'), 'w'), indent=1)
        forced = set()
        for _ in range(3):
            src = disasm_ipl3(ipl, forced)
            b = assemble(src, IPL_VRAM, 'ipl3')
            bad = {k for k in range(len(ipl) // 4) if b[4 * k:4 * k + 4] != ipl[4 * k:4 * k + 4]}
            if not bad - forced: break
            forced |= bad
        open(os.path.join(gd, 'ipl3.s'), 'w').write(src)
        print(g, 'ipl3 words', len(ipl) // 4, 'as .word (not round-tripping as instructions):', len(forced))
    hj = json.load(open(os.path.join(gd, 'header.json')))
    hb = header_bytes(hj['fields'], rom)
    ib = assemble(open(os.path.join(gd, 'ipl3.s')).read(), IPL_VRAM, 'ipl3')
    ok = (hb == rom[:0x40], ib == ipl)
    print(g, 'header', 'OK' if ok[0] else 'MISMATCH', 'ipl3', 'OK' if ok[1] else 'MISMATCH', len(ib))
    sys.exit(0 if all(ok) else 1)

if __name__ == '__main__':
    main()
