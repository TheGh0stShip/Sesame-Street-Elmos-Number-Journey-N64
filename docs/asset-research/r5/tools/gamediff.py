#!/usr/bin/env python3
"""gamediff.py LETTER_ROM NUMBER_ROM FUNCTIONS_JSON SRCROOT OUTDIR

Differences between the two programs.
1. Functions: twins share the hash in functions.json. Functions whose hash has no copy in the other game are listed
   with their library and description, aligned between the nearest twin anchors (so a changed function shows next to
   its counterpart in the other game).
2. Data: twin functions have identical code apart from relocations, so the i-th %hi/%lo address built in the Letter
   copy and the i-th in the Number copy name the same object. That pairs the .data/.rodata objects (object extents
   from agents data1/data2 MAP.tsv); each pair is compared: identical, pointers-only (every differing word is a RAM
   pointer on both sides), or content differs (count of differing bytes; sizes compared).
Writes OUTDIR/diff_functions.tsv and OUTDIR/diff_data.tsv (GAME_DIFF.md is written from them)."""
import sys, os, json, csv, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from seslib import *


def main(lrom, nrom, fj, srcroot, outdir):
    sesdir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    roms = {'letter': open(lrom, 'rb').read(), 'number': open(nrom, 'rb').read()}
    fs = {g: load_functions(fj, g) for g in roms}
    graph = {g: json.load(open(os.path.join(outdir, 'graph_%s.json' % g))) for g in roms}
    ut = {g: {} for g in roms}
    for g in roms:
        for f in fs[g]: ut[g].setdefault(f['hash'], []).append(f)
    twin = {}
    for h, l in ut['letter'].items():
        if len(l) == 1 and len(ut['number'].get(h, [])) == 1: twin[l[0]['vram']] = ut['number'][h][0]['vram']
    # unreachable/description lookups
    desc = {}
    for g in roms:
        p = os.path.join(outdir, 'UNREACHABLE_%s.tsv' % g)
        for r in csv.DictReader(open(p), delimiter='\t'):
            desc[(g, int(r['func'][5:], 16))] = r['description(matched C)']

    def d(g, v):
        if (g, v) in desc: return desc[(g, v)]
        pth = os.path.join(srcroot, g, 'src', 'func_%08X.c' % v)
        if os.path.exists(pth):
            m = re.search(r'/\*\s*([^*=][^*]*?)\s*\*/', open(pth, errors='replace').read())
            if m and 'unhandled' not in m.group(1): return ' '.join(m.group(1).split())[:140]
        return ''
    # 1. function differences, aligned on twin anchors
    with open(os.path.join(outdir, 'diff_functions.tsv'), 'w') as o:
        o.write('segment(after twin anchor L/N)\tgame\tfunc\tlibrary\tsize\treachable\tdescription\n')
        lonly = [f for f in fs['letter'] if len(ut['number'].get(f['hash'], [])) == 0]
        nonly = [f for f in fs['number'] if len(ut['letter'].get(f['hash'], [])) == 0]
        anchors = sorted(twin.items())
        inv = {n: l for l, n in twin.items()}
        def anchor_of(g, v):
            best = None
            for f in fs[g]:
                if f['vram'] > v: break
                if (g == 'letter' and f['vram'] in twin) or (g == 'number' and f['vram'] in inv): best = f['vram']
            if best is None: return 'start'
            return 'L%08X/N%08X' % ((best, twin[best]) if g == 'letter' else (inv[best], best))
        rows = []
        for g, lst in (('letter', lonly), ('number', nonly)):
            for f in lst:
                v = f['vram']; gj = graph[g]['%08X' % v]
                rows.append((anchor_of(g, v), g, v, gj['lib'], f['size'], 'yes' if gj['reach_any'] else 'NO', d(g, v)))
        rows.sort(key=lambda r: (r[0], r[1], r[2]))
        for r in rows:
            o.write('%s\t%s\tfunc_%08X\t%s\t%d\t%s\t%s\n' % r)
        open(os.path.join(outdir, 'diff_summary.txt'), 'w').write('%d\n' % len(twin))
        print('functions: letter-only', len(lonly), sum(f['size'] for f in lonly), 'B; number-only', len(nonly), sum(f['size'] for f in nonly), 'B; twins', len(twin))
    # 2. data pairing through twin functions
    code = {g: scan_code(roms[g], fs[g]) for g in roms}
    objs = {g: [] for g in roms}
    for f in ('data1', 'data2'):
        for r in csv.DictReader(open(os.path.join(sesdir, f, 'MAP.tsv')), delimiter='\t'):
            if r['class'] == 'pad': continue
            objs[r['game']].append((int(r['rom_start'], 16) + RAM2ROM, int(r['rom_end'], 16) + RAM2ROM, r['class'], r['description'][:70]))
    for g in objs: objs[g].sort()
    import bisect
    def obj_at(g, a):
        i = bisect.bisect_right(objs[g], (a, 1 << 40)) - 1
        if i >= 0 and objs[g][i][0] <= a < objs[g][i][1]: return objs[g][i]
        return None
    pair = {}
    for l, n in twin.items():
        al = [t for t, pc in sorted(code['letter'][l]['addrs'], key=lambda x: x[1])]
        an = [t for t, pc in sorted(code['number'][n]['addrs'], key=lambda x: x[1])]
        if len(al) != len(an): continue
        for x, y in zip(al, an):
            ol, on = obj_at('letter', x), obj_at('number', y)
            if ol and on and x - ol[0] == y - on[0]:
                pair.setdefault(ol, set()).add(on)
    lo = SEG['letter']['data'][0]
    def isptr(w): return 0x80000000 <= w < 0x80400000
    out = []
    paired_n = set()
    for ol, ons in sorted(pair.items()):
        for on in sorted(ons):
            paired_n.add(on)
            bl = roms['letter'][ol[0] - RAM2ROM:ol[1] - RAM2ROM]
            bn = roms['number'][on[0] - RAM2ROM:on[1] - RAM2ROM]
            if bl == bn: st = 'identical'
            else:
                n = min(len(bl), len(bn))
                diff = sum(1 for i in range(n) if bl[i] != bn[i]) + abs(len(bl) - len(bn))
                dw = [(int.from_bytes(bl[i:i + 4], 'big'), int.from_bytes(bn[i:i + 4], 'big')) for i in range(0, n - 3, 4)
                      if bl[i:i + 4] != bn[i:i + 4]]
                if len(bl) == len(bn) and all(isptr(a) and isptr(b) for a, b in dw) and n % 4 == 0 and ol[0] % 4 == 0:
                    st = 'pointers only (%d words relocated)' % len(dw)
                else:
                    st = 'content differs (%d of %d bytes%s)' % (diff, max(len(bl), len(bn)), '' if len(bl) == len(bn) else ', size %d vs %d' % (len(bl), len(bn)))
            out.append(('%08X' % ol[0], ol[1] - ol[0], ol[2], ol[3], '%08X' % on[0], on[1] - on[0], st))
    with open(os.path.join(outdir, 'diff_data.tsv'), 'w') as o:
        o.write('letter_ram\tletter_size\tclass\tletter_object\tnumber_ram\tnumber_size\tcomparison\n')
        for r in out: o.write('%s\t%d\t%s\t%s\t%s\t%d\t%s\n' % r)
        for g, other in (('letter', set(pair)), ('number', paired_n)):
            for ob in objs[g]:
                if ob not in other:
                    o.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (('%08X' % ob[0], ob[1] - ob[0], ob[2], ob[3], '-', '-', 'no twin-function pairing (letter object)') if g == 'letter'
                            else ('-', '-', ob[2], ob[3], '%08X' % ob[0], ob[1] - ob[0], 'no twin-function pairing (number object)')))
    from collections import Counter
    print('data pairs', len(out), Counter(r[6].split(' (')[0] for r in out))


if __name__ == '__main__':
    main(*sys.argv[1:6])
