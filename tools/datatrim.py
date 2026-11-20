#!/usr/bin/env python3
"""datatrim.py OBJECT TABLE: cut the assembler's end-of-section padding from a unit's own .rodata/.data.
KMC GNU as rounds every section to 16 bytes. In the program image a unit's literals are followed directly by the
next unit's (or by program data), so the padding must go for the linker to place the next input at its address
(the .text counterpart is trimtext.py). TABLE (tools/datatrim.tsv, data/datatrim.tsv) gives object, section, size and optionally
the section alignment (KMC as gives every section 16; a data/ run starts at an address aligned only to its
strictest object). Only zero bytes past SIZE with no symbol or relocation in them are removed. Objects not in
TABLE are left alone."""
import sys, struct, os
path, table = sys.argv[1], sys.argv[2]
key = os.path.basename(path); want = {}
for l in open(table):
    if l.startswith('#') or not l.strip(): continue
    c = l.split()
    if c[0] == key: want[c[1]] = (int(c[2], 0), int(c[3], 0) if len(c) > 3 else None)
if not want: sys.exit(0)
b = bytearray(open(path, 'rb').read())
shoff, = struct.unpack_from('>I', b, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('>HHH', b, 0x2E)
def sh(i): return list(struct.unpack_from('>IIIIIIIIII', b, shoff + i * shentsize))
H = [sh(i) for i in range(shnum)]
def name(h): o = H[shstrndx][4] + h[0]; return b[o:b.index(b'\0', o)].decode()
idx = {name(h): i for i, h in enumerate(H)}
for sec, (n, al) in want.items():
    i = idx[sec]; h = H[i]
    if al: h[8] = min(h[8], al); struct.pack_into('>IIIIIIIIII', b, shoff + i * shentsize, *h)
    if h[5] <= n: continue
    assert not any(b[h[4] + n:h[4] + h[5]]), '%s %s: non-zero bytes past 0x%X' % (key, sec, n)
    for j, r in enumerate(H):                       # no symbol at or past n, no relocation of that section past n
        if r[1] == 2:
            for k in range(0, r[5], 16):
                _, v, _, _, _, shn = struct.unpack_from('>IIIBBH', b, r[4] + k)
                assert not (shn == i and v >= n and v > 0), '%s: symbol inside the padding' % key
        if r[1] == 9 and r[7] == i:
            for k in range(0, r[5], 8):
                off, = struct.unpack_from('>I', b, r[4] + k); assert off < n, '%s: relocation inside the padding' % key
    h[5] = n; struct.pack_into('>IIIIIIIIII', b, shoff + i * shentsize, *h)
open(path, 'wb').write(b)
