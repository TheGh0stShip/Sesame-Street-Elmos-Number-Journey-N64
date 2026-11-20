"""survey.py ROM GAME SRCDIR LO HI : list D_ symbols in [LO,HI) RAM with declared types and users,
plus words in the range that point into the range (pointer-derived object starts). Prints no data values
except addresses."""
import sys, re, os, collections
rom, game, src, lo, hi = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4],16), int(sys.argv[5],16)
B = open(rom,'rb').read()
OFF = 0x7FFFF400
syms = collections.defaultdict(lambda: {'decl':set(),'users':set()})
for fn in sorted(os.listdir(src)):
    t = open(os.path.join(src,fn)).read()
    for m in re.finditer(r'D_(8[0-9A-F]{7})', t):
        a = int(m.group(1),16)
        if lo <= a < hi: syms[a]['users'].add(fn[:-2])
    for line in t.splitlines():
        if line.startswith('extern') or line.startswith('struct'):
            for m in re.finditer(r'D_(8[0-9A-F]{7})', line):
                a=int(m.group(1),16)
                if lo<=a<hi: syms[a]['decl'].add(line.strip())
ptr = collections.defaultdict(list)
for a in range(lo & ~3, hi, 4):
    w = int.from_bytes(B[a-OFF:a-OFF+4],'big')
    if lo <= w < hi: ptr[w].append(a)
for a in sorted(set(syms)|set(ptr)):
    s = syms.get(a, {'decl':set(),'users':set()})
    print('%08X\tusers=%s\tptrfrom=%s' % (a, ','.join(sorted(s['users'])[:6]), ','.join('%X'%p for p in ptr.get(a,[])[:4])))
    for d in sorted(s['decl']): print('\t\t', d)
