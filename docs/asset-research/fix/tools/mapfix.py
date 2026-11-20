import sys
B = '${N64DK_LAB}/sesassets/'
def rd(p): return [l.rstrip('\n').split('\t') for l in open(B + p) if l.strip()]
def wr(p, rows): open(B + p, 'w').write(''.join('\t'.join(r) + '\n' for r in rows))
h = lambda s: int(s, 16)
r1 = rd('r1/MAP.tsv'); hdr, body = r1[0], r1[1:]
# regroup r1's Number 0x1717E0..0x1724A0 rows into six whole 0x220 objects
new = []; grp = [r for r in body if r[0] == 'number' and ':' not in r[1] and 0x1717E0 <= h(r[1]) < 0x1724A0]
pals = [r for r in grp if r[3] == 'palette']; texs = [r for r in grp if r[3] == 'texture']
assert len(pals) == 6 and len(texs) == 6
blocks = []
for k in range(6):
    a = 0x1717E0 + k * 0x220
    p = list(pals[k]); t = list(texs[k])
    p[1], p[2] = '0x%06X' % a, '0x%06X' % (a + 0x20); t[1], t[2] = '0x%06X' % (a + 0x20), '0x%06X' % (a + 0x220)
    p[5] = p[5].replace('; the rest of the TLUT is zero and claimed as pad by packC', ' (whole 0x20 TLUT, zero entries included)')
    t[5] = t[5].split(';')[0] + ' (whole 0x200 texel block, zero texels included)'
    blocks += [p, t]
out = []
for r in body:
    if r in grp: continue
    out.append(r)
    if r[0] == 'number' and r[1] == '0x1717D8': out += blocks
wr('r1/MAP.tsv', [hdr] + out)
r1n = {(r[0], r[1]): r for r in [hdr] + out}
# packC
pc = rd('packC/MAP.tsv'); ph, pb = pc[0], pc[1:]
def r1rows(game, a, b): return [r for r in out if r[0] == game and ':' not in r[1] and a <= h(r[1]) < b]
repl = {('number', 0x1717D8, 0x1724A0)}
repl |= {('letter', a, b) for a, b in [(0x14440C, 0x144410), (0x152044, 0x152050), (0x15C598, 0x15C5A0), (0x15FF58, 0x15FF60), (0x16035C, 0x160360), (0x196CE0, 0x196F00), (0x1BF3D0, 0x1BF6F0)]}
res = []; done = set()
for r in pb:
    hit = [x for x in repl if r[0] == x[0] and x[1] <= h(r[1]) < x[2]]
    if not hit: res.append(r); continue
    x = hit[0]
    assert h(r[2]) <= x[2], r
    if x in done: continue
    done.add(x)
    for q in r1rows(x[0], x[1], x[2]):
        res.append([q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7]])
        if q[1] == '0x1BF3D0':
            res.append(['letter', '0x1BF5D0', '0x1BF6B0', 'dl', 'f3dex_dl', 'F3DEX display lists of the last common model (RAM 0x802FD4E0-0x802FD5C0; wrapper at 0x802FD5B0) [r1/gfx]', 'model header RAM 0x802FD5D8 +0x08', 'gfx/tools/extract_gfx.py'])
assert done == repl, repl - done
# tiling check of the replaced spans
for g, a, b in repl:
    rs = sorted((h(r[1]), h(r[2])) for r in res if r[0] == g and a <= h(r[1]) < b)
    assert rs[0][0] == a and rs[-1][1] == b and all(rs[i][1] == rs[i + 1][0] for i in range(len(rs) - 1)), (g, hex(a), rs)
wr('packC/MAP.tsv', [ph] + res)
print('packC rows', len(pb), '->', len(res))
