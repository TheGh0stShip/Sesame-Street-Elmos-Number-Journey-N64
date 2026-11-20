#!/usr/bin/env python3
"""rodata_split.py OBJECT TABLE: give part of an object's read-only data its own address.

In the original build several functions shared a translation unit, and the program's .rodata does not hold
every function's literals in the order its one-function source file emits them. TABLE (tools/rodata_split.tsv)
lists, for such an object, the section, the offset where a moved part starts, its length and the symbol
naming its address (the linker script defines it). Every HI16/LO16 pair of .text that points into the part
is re-targeted to that symbol with the addend reduced by the offset. Nothing else changes; objects not in
TABLE are left alone. The object is rewritten with its sections laid out again (contents unchanged)."""
import sys, struct, os
path, table = sys.argv[1], sys.argv[2]
key = os.path.basename(path)
parts = []
for l in open(table):
    if l.startswith('#') or not l.strip(): continue
    obj, sec, off, n, sym = l.split()
    if obj == key: parts.append((sec, int(off, 0), int(n, 0), sym))
if not parts: sys.exit(0)
b = open(path, 'rb').read()
shoff, = struct.unpack_from('>I', b, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('>HHH', b, 0x2E)
H = [list(struct.unpack_from('>IIIIIIIIII', b, shoff + i * shentsize)) for i in range(shnum)]
D = [bytearray(b[h[4]:h[4] + h[5]]) if h[1] != 8 else bytearray() for h in H]
def nm(i): s = D[shstrndx]; o = H[i][0]; return s[o:s.index(b'\0', o)].decode()
idx = {nm(i): i for i in range(shnum)}
symi = idx['.symtab']; stri = H[symi][6]
nsym = len(D[symi]) // 16
def sym(k): return struct.unpack_from('>IIIBBH', D[symi], k * 16)
secsym = {}
for k in range(nsym):
    name, val, size, info, other, shndx = sym(k)
    if info & 0xF == 3: secsym[shndx] = k                      # STT_SECTION
newsym = {}
for sec, off, n, s in parts:
    if s in newsym: continue
    o = len(D[stri]); D[stri] += s.encode() + b'\0'
    D[symi] += struct.pack('>IIIBBH', o, 0, 0, 0x10, 0, 0)     # GLOBAL NOTYPE UND
    newsym[s] = nsym; nsym += 1
H[symi][5] = len(D[symi])
text = idx['.text']; rel = idx['.rel.text']
R = [list(struct.unpack_from('>II', D[rel], k * 8)) for k in range(len(D[rel]) // 8)]
def word(off): return struct.unpack_from('>I', D[text], off)[0]
def put(off, w): struct.pack_into('>I', D[text], off, w)
pend = []; moved = 0
for r in R:
    t, s = r[1] & 0xFF, r[1] >> 8
    if t == 5: pend.append(r); continue
    if t == 6:
        his = [h for h in pend if h[1] >> 8 == s]; pend = [h for h in pend if h[1] >> 8 != s]
        lo = word(r[0]) & 0xFFFF; lo -= 0x10000 if lo & 0x8000 else 0
        for sec, off, n, name in parts:
            if secsym.get(idx[sec]) != s: continue
            ahl = ((word(his[0][0]) & 0xFFFF) << 16) + lo if his else lo
            if not off <= ahl < off + n: continue
            new = ahl - off
            put(r[0], (word(r[0]) & 0xFFFF0000) | (new & 0xFFFF))
            for h in his: put(h[0], (word(h[0]) & 0xFFFF0000) | (((new + 0x8000) >> 16) & 0xFFFF))
            for x in his + [r]: x[1] = (newsym[name] << 8) | (x[1] & 0xFF)
            moved += 1; break
D[rel] = bytearray(b''.join(struct.pack('>II', *r) for r in R))
# write: ELF header, sections in order, section headers
out = bytearray(b[:0x34]); pos = 0x34
for i, h in enumerate(H):
    if i == 0 or h[1] == 8: continue
    al = max(h[8], 1); pad = (-pos) % al; out += bytes(pad); pos += pad
    h[4] = pos; h[5] = len(D[i]); out += D[i]; pos += len(D[i])
pad = (-pos) % 4; out += bytes(pad); pos += pad
struct.pack_into('>I', out, 0x20, pos)
for h in H: out += struct.pack('>IIIIIIIIII', *h)
open(path, 'wb').write(out)
print('%s: %d references moved' % (key, moved))
