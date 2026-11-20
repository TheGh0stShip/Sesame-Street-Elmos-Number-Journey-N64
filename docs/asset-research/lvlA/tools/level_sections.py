#!/usr/bin/env python3
"""level_sections.py GAME LEVELDIR SRCDIR OUT.tsv
Maps the top level of each inflated .bif RAM image (base 0x80154000) by kind, from its own pointers:
F3DEX display lists (runs of commands ending in G_ENDDL 0xB8), vertex arrays (targets of G_VTX 0x04,
size from the command), texture/palette data (targets of G_SETTIMG 0xFD), pointer structures (other
words pointing into the image), and the rest (unreferenced data). Code references (D_80xxxxxx symbols
in the matched sources that fall in the image) are listed as roots."""
import sys, os, re, struct, glob
BASE = 0x80154000
game, ldir, sdir, outp = sys.argv[1:5]
refs = {}
for f in glob.glob(os.path.join(sdir, '*.c')):
    for m in set(re.findall(r'D_80(1[5-9A-D][0-9A-F]{4})\b', open(f).read())):
        a = int('80' + m, 16)
        if BASE <= a < BASE + 0x86800: refs.setdefault(a, set()).add(os.path.basename(f)[:-2])
DLOPS = {0x01,0x03,0x04,0x06,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
         0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xED,0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,0xC0}
out = open(outp, 'w')
out.write('file\tsection\toffset\tsize\tkind\tevidence\n')
for fn in sorted(glob.glob(os.path.join(ldir, '*.bin'))):
    b = open(fn, 'rb').read(); n = len(b); name = os.path.basename(fn)[:-4]
    kind = bytearray(n)  # 0 data,1 dl,2 vtx,3 tex,4 ptrstruct
    ev = {}
    inside = lambda a: BASE <= a < BASE + n
    # display lists: backward from each G_ENDDL, take the maximal run of valid commands
    ends = [o for o in range(0, n - 7, 8) if b[o] == 0xB8 and b[o+1:o+8] == bytes(7)]
    dls = []
    for e in ends:
        s = e
        while s >= 8 and b[s-8] in DLOPS and b[s-8] != 0xB8:
            op = b[s-8]; w1 = struct.unpack('>I', b[s-4:s])[0]
            if op in (0x01, 0x04, 0x06, 0xFD) and not inside(w1): break
            s -= 8
        dls.append((s, e + 8))
        for o in range(s, e + 8): kind[o] = 1
    vtx, tex = set(), set()
    for s, e in dls:
        for o in range(s, e, 8):
            op = b[o]; w0, w1 = struct.unpack('>II', b[o:o+8])
            if op == 0x04:
                l = (w0 & 0xFFFF) + 1; t = w1 - BASE; vtx.add(t)
                for x in range(t, min(n, t + l)):
                    if kind[x] == 0: kind[x] = 2
            elif op == 0xFD:
                tex.add(w1 - BASE)
    for t in sorted(tex):
        x = t
        while x < n and kind[x] == 0: kind[x] = 3; x += 1
    for o in range(0, n - 3, 4):
        if kind[o] == 0 and inside(struct.unpack('>I', b[o:o+4])[0]):
            for x in range(o, o + 4): kind[x] = 4
    names = {0: 'data', 1: 'display_lists', 2: 'vertices', 3: 'texture_or_palette', 4: 'pointer_struct'}
    evs = {0: 'not reached from any display list or pointer in the image',
           1: 'F3DEX command runs ending in G_ENDDL (0xB8)',
           2: 'target of G_VTX (0x04), length from the command',
           3: 'from a G_SETTIMG (0xFD) target up to the next classified byte',
           4: 'words holding absolute pointers into the image (0x80154000-based)'}
    o = 0; i = 0
    while o < n:
        k = kind[o]; e = o
        while e < n and kind[e] == k: e += 1
        r = sorted(a - BASE for a in refs if o <= a - BASE < e)
        extra = ('; code refs: ' + ', '.join('0x%X(%s)' % (x, '/'.join(sorted(refs[BASE + x]))[:60]) for x in r[:6])) if r else ''
        out.write('%s\t%d\t0x%X\t%d\t%s\t%s%s\n' % (name, i, o, e - o, names[k], evs[k], extra))
        o = e; i += 1
    print(name, n, {names[k]: kind.count(k) for k in names}, 'dls', len(dls), 'vtx', len(vtx), 'tex', len(tex))
