#!/usr/bin/env python3
"""draft_types.py ROM GAME REFS.tsv OUT_types.tsv

Builds the type table for data2's territory (second half of .data + all of .rodata) from the code
references found by scan_refs.py.  The ROM is only *inspected* (to decide string / zero padding /
code-pointer shape); the table written contains addresses, sizes, C types, names and evidence,
never data values.  Manual corrections live in OVERRIDES below (by address) so that re-running the
draft keeps them.

Columns: rom_start  size  ctype  name  evidence
ctype vocabulary (gen_data.py understands exactly these):
  char[]   NUL-terminated string(s) (several consecutive strings in one row are split by gen_data)
  f32 f64  float / double literals (arrays if size > element)
  u8 u16 u32 s16 s32   integer arrays (u32 emitted hex)
  jtbl     switch jump table: array of code labels func_X + 0xNN
  pad      zero alignment padding
  rsp_data RSP microcode DATA (DMEM image) - analysed by agent ucode
"""
import sys, struct, collections

ROM2RAM = 0x7FFFF400
TERR = {  # data2 territory RAM [start, rodata_start, end)
    'letter': (0x80072800, 0x800785B0, 0x8007BAB0),
    'number': (None, 0x80078DC0, 0x8007BF20),
}
NUMBER_DATA = (0x8006E31C, 0x80078DC0)

# Manual corrections: ram -> (size, ctype, name, evidence)
OVERRIDES = {
    # CORDIC arctangent table: s64 D_80078800[64], read by func_80067CE0 / func_80068410 (angle += tbl[i])
    'letter': {
        0x80072800: (None, 'u8', 'D_80072160_tail', 'tail of texture D_80072160 (starts in data1 half, 0x1000 bytes): '
                     'G_SETTIMG fmt=I + G_LOADBLOCK 2048 16-bit units in func_80012164'),
        0x80075E4C: (None, 'u32', 'D_80075E4C', 'addr+idx*4 then lw (s32 table) in func_80032818'),
        0x8007B8E0: (None, 'f64', 'D_8007B8E0_pows', 'addr+idx*8 then ldc1 in func_80066F20 (_Ldtob powers of ten)'),
        0x80078800: (0x200, 's64', 'D_80078800_atan_table', 's64[]@func_80067CE0,func_80068410 (source extern s64[])')},
    'number': {
        0x8007BD50: (None, 'f64', 'D_8007BD50_pows', 'addr+idx*8 in func_80068890 (_Ldtob powers of ten)'),
        0x80079010: (0x200, 's64', 'D_80079010_atan_table', 'addr@func_80069650,func_80069D80 (sibling of letter s64 table)')},
}
RSP_DATA = {  # identified by hand: (ram_start, ram_end, note) ; see REPORT.md
    'letter': [(0x80078A00, 0x80079200, 'F3DEX 1.23 DMEM data: task->list.ucode_data in func_8000B470'),
               (0x80079200, 0x800794B0, 'audio ucode DMEM data: req.unk_C in func_80050DD8 (with text D_8006C890)')],
    'number': [(0x80079210, 0x80079A10, 'F3DEX 1.23 DMEM data: task->list.ucode_data in func_8000BA3C'),
               (0x80079A10, 0x80079CC0, 'audio ucode DMEM data: addr in func_80052748')],
}

def printable(b):
    return b in (9, 10, 13) or 0x20 <= b < 0x7F

def string_len(rom, o, end):
    """length incl. NUL of a plausible C string at o, else 0"""
    n = 0
    while o + n < end and rom[o + n] != 0:
        if not printable(rom[o + n]):
            return 0
        n += 1
    if o + n >= end or n < 1:
        return 0
    return n + 1

def main():
    romp, game, refsp, outp = sys.argv[1:5]
    rom = open(romp, 'rb').read()
    t0, r0, t1 = TERR[game]
    refs = collections.defaultdict(list)
    for ln in open(refsp):
        a, kind, fn, ia, extra = ln.rstrip('\n').split('\t')
        refs[int(a, 16)].append((kind, fn, extra))
    if t0 is None:
        # Number: midpoint of .data at a referenced symbol boundary nearest the middle
        d0, d1 = NUMBER_DATA
        mid = (d0 + d1) // 2
        t0 = min((a for a in refs if d0 < a < d1 and a % 4 == 0), key=lambda a: abs(a - mid))
    rows = []
    def emit(a, size, ctype, name, ev):
        if size > 0:
            rows.append((a, size, ctype, name, ev))
    starts = sorted(a for a in refs if t0 <= a < t1)
    rsp = RSP_DATA[game]
    cuts = set(starts) | {t0, r0, t1}
    for s, e, _ in rsp:
        cuts |= {s, e}
    # jump tables first: determine their extents
    jt = {}
    for a in starts:
        ks = refs[a]
        j = [k for k in ks if k[0] == 'jtbl']
        if j:
            n = 0
            ex = j[0][2]
            if ex.startswith('n='):
                n = int(ex[2:])
            else:
                fn = int(j[0][1][5:], 16)
                while True:
                    w = struct.unpack('>I', rom[a - ROM2RAM + 4*n: a - ROM2RAM + 4*n + 4])[0]
                    if fn <= w < fn + 0x4000:
                        n += 1
                    else:
                        break
            jt[a] = n * 4
            cuts.add(a + n * 4)
    cuts = sorted(c for c in cuts if t0 <= c <= t1)
    for a, b in zip(cuts, cuts[1:]):
        inrsp = [x for x in rsp if x[0] <= a < x[1]]
        if inrsp:
            emit(a, b - a, 'rsp_data', 'rsp_data_%08X' % a, inrsp[0][2])
            continue
        ks = refs.get(a, [])
        kinds = sorted({k[0] for k in ks})
        fns = sorted({k[1] for k in ks})
        ev = ','.join(kinds) + '@' + ','.join(fns[:4]) + ('+%d' % (len(fns) - 4) if len(fns) > 4 else '')
        o, oe = a - ROM2RAM, b - ROM2RAM
        name = 'D_%08X' % a
        if a in OVERRIDES[game]:
            size, ct, nm, evd = OVERRIDES[game][a]
            size = size or (b - a)
            emit(a, size, ct, nm, evd)
            a += size
            o = a - ROM2RAM
            ks, kinds, ev = [], [], ''
            if a >= b:
                continue
        if 'jtbl' in kinds:
            emit(a, jt[a], 'jtbl', 'jtbl_%08X' % a, ev)
            continue
        size = b - a
        if not ks:
            fill_unref(rom, a, b, rows, emit)
            continue
        base = {k.rstrip('[]') for k in kinds}
        if base & {'ldc1', 'sdc1'} and size >= 8 and a % 8 == 0:
            n = 8 * (size // 8) if 'ldc1[]' in kinds or 'sdc1[]' in kinds else 8
            emit(a, n, 'f64', name, ev); fill_unref(rom, a + n, b, rows, emit); continue
        if base & {'lwc1', 'swc1'} and a % 4 == 0:
            n = 4 * (size // 4) if any(k.endswith('[]') for k in kinds) else 4
            emit(a, n, 'f32', name, ev); fill_unref(rom, a + n, b, rows, emit); continue
        if 'addr' in kinds or 'ptr' in kinds:
            sl = string_len(rom, o, oe)
            if sl >= 2 or (sl == 1 and a >= r0 and 'ptr' in kinds):
                fill_strings(rom, a, b, rows, emit, name, ev)
                continue
        if base & {'lw', 'sw'} or (a % 4 == 0 and size % 4 == 0):
            if a % 4 == 0:
                n = size - size % 4
                emit(a, n, 's32' if 'lw' in base and a >= r0 else 'u32', name, ev)
                fill_unref(rom, a + n, b, rows, emit); continue
        if base & {'lh', 'lhu', 'sh'} and a % 2 == 0:
            n = size - size % 2
            emit(a, n, 's16' if 'lh' in base else 'u16', name, ev)
            fill_unref(rom, a + n, b, rows, emit); continue
        emit(a, size, 'u8', name, ev)
    with open(outp, 'w') as f:
        f.write('# data2 type table for %s: rom_start size ctype name evidence (no data values)\n' % game)
        f.write('# territory RAM %08X-%08X (ROM %X-%X); .rodata starts RAM %08X\n'
                % (t0, t1, t0 - ROM2RAM, t1 - ROM2RAM, r0))
        for a, size, ct, nm, ev in rows:
            f.write('%X\t%X\t%s\t%s\t%s\n' % (a - ROM2RAM, size, ct, nm, ev))
    print(game, len(rows), 'objects; territory', hex(t0), hex(t1))

def fill_strings(rom, a, b, rows, emit, name, ev):
    o = a - ROM2RAM
    oe = b - ROM2RAM
    p = o
    while p < oe:
        sl = string_len(rom, p, oe)
        if sl == 0:
            break
        p += sl
    # allow zero padding after the strings to the next 4-byte boundary inside the run
    emit(a, p - o, 'char[]', name, ev)
    fill_unref(rom, p + ROM2RAM, b, rows, emit)

def fill_unref(rom, a, b, rows, emit):
    """unreferenced bytes: zero runs -> pad, strings -> char[] (unref), else words/bytes"""
    while a < b:
        o = a - ROM2RAM
        if rom[o] == 0:
            e = a
            while e < b and rom[e - ROM2RAM] == 0:
                e += 1
            # zero run: pad only up to alignment-looking lengths; long zero runs inside data are u8
            emit(a, e - a, 'pad', 'pad_%08X' % a, 'zero, no reference')
            a = e
            continue
        sl = string_len(rom, o, b - ROM2RAM)
        if sl >= 4:
            p = o
            while True:
                s2 = string_len(rom, p, b - ROM2RAM)
                if s2 < 2:
                    break
                p += s2
            emit(a, p - o, 'char[]', 'unref_str_%08X' % a, 'no code reference')
            a = p + ROM2RAM
            continue
        # non-zero non-string: up to next zero-aligned stop, as u8 / u32
        e = a + 1
        while e < b and not (rom[e - ROM2RAM] == 0 and e % 4 == 0 and
                             all(rom[x - ROM2RAM] == 0 for x in range(e, min(e + 4, b)))):
            e += 1
        ct = 'u32' if a % 4 == 0 and (e - a) % 4 == 0 else 'u8'
        emit(a, e - a, ct, 'unref_%08X' % a, 'no code reference')
        a = e

main()
