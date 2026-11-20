B = '${N64DK_LAB}/sesassets/'
def rd(p): return [l.rstrip('\n').split('\t') for l in open(B + p) if l.strip()]
def wr(p, rows): open(B + p, 'w').write(''.join('\t'.join(r) + '\n' for r in rows))
def off(s): return int(s.split(':')[1], 16)
def fmt(f, v, w): return '%s:0x%0*X' % (f, w, v)
r1 = rd('r1/MAP.tsv')[1:]
def r1sub(game, f, a, b): return [r for r in r1 if r[0] == game and r[1].startswith(f + ':') and a <= off(r[1]) < b]
# lvlG bump
g = rd('lvlG/MAP.tsv'); out = [g[0]]
for r in g[1:]:
    if r[0] == 'number' and r[1].startswith('bump.bif:') and r[3] == 'unknown' and off(r[1]) in (0x262C0, 0x30250, 0x3A1E0, 0x44170):
        subs = r1sub('number', 'bump.bif', off(r[1]), off(r[2])); assert len(subs) == 2, r
        for q in subs:
            out.append([q[0], fmt('bump.bif', off(q[1]), 5), fmt('bump.bif', off(q[2]), 5), q[3], q[4], q[5] + ' (per r1: one of four palette variants of one 32x32 CI8 gradient tile; unused)', q[6], 'r1/tools/extract_r1.py'])
    else: out.append(r)
assert len(out) == len(g) + 4; wr('lvlG/MAP.tsv', out)
# lvlE manor (7 columns: format/description merged)
e = rd('lvlE/MAP.tsv'); out = [e[0]]
for r in e[1:]:
    if r[0] == 'letter' and r[1] == 'manor.bif:0x060B0':
        for q in r1sub('letter', 'manor.bif', 0x60B0, 0x6AB0):
            out.append([q[0], fmt('manor.bif', off(q[1]), 5), fmt('manor.bif', off(q[2]), 5), q[3], q[4] + '; ' + q[5], q[6], 'r1/tools/extract_r1.py'])
    else: out.append(r)
assert len(out) == len(e) + 1; wr('lvlE/MAP.tsv', out)
# lvlB model headers 0x24 -> 0x28
b = rd('lvlB/MAP.tsv'); rows = b[1:]; n = 0; fixed = []
idx = {(r[0], r[1].split(':')[0], off(r[1])): i for i, r in enumerate(rows) if ':' in r[1]}
drop = set()
for i, r in enumerate(rows):
    if r[3] != 'model_header' or ':' not in r[1]: continue
    f = r[1].split(':')[0]; a, e_ = off(r[1]), off(r[2])
    if e_ - a != 0x24: continue
    w = len(r[2].split(':')[1]) - 2
    r[2] = fmt(f, e_ + 4, w)
    r[4] = r[4] + '; u32 bbox ptr (+0x24; header is 0x28 bytes)'
    j = idx.get((r[0], f, e_))
    if j is not None:
        q = rows[j]; L = off(q[2]) - e_
        if L == 4: drop.add(j)
        else: q[1] = fmt(f, e_ + 4, w); fixed.append((r[0], q[1], q[3]))
    n += 1
rows = [r for i, r in enumerate(rows) if i not in drop]
wr('lvlB/MAP.tsv', [b[0]] + rows)
print('lvlB headers', n, 'dropped 4-byte rows', len(drop), 'shifted', fixed[:10], len(fixed))
