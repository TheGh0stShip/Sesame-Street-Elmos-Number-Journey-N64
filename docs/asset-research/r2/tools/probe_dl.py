"""probe: classify level-image display_list / vertex conflict pairs by the F3DEX evidence (lab survey tool)."""
import sys, os, collections, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.expanduser('${N64DK_LAB}/sesassets/ledger/tools'))
import merge_ledger as M, evidence as E


def level_evidence(G, fn, rows):
    roots = [((fn, r.a), {}) for r in rows if r.kind == 'display_list']
    roots += G.model_roots(fn, sorted({r.a for r in rows if r.kind == 'model_header'}))
    dls = G.dl_closure(roots, fn)
    objs = {(o, r['end']) for (s, o), r in dls.items() if s == fn and r['ok']}
    loads = set()
    for r in dls.values():
        for vs, vo, ln in r.get('rvtx', ()):
            if vs == fn: loads.add((vo, vo + ln))
    return dls, objs, set(E.vertex_arrays(dls, fn)), loads


def status(ev, kind, a, b):
    dls, objs, va, loads = ev
    if kind == 'display_list':
        if (a, b) in objs: return 'exact'
        return 'multi' if a in {x for x, _ in objs} and b in {y for _, y in objs} else 'other'
    if kind == 'vertex':
        if (a, b) in va: return 'array'
        if (a, b) in loads: return 'load'
        if any(x <= a and b <= y for x, y in va): return 'inside'
        if any(a <= x and y <= b for x, y in va): return 'spans'
        return 'unref'
    return '-'


if __name__ == '__main__':
    for game in M.GAMES:
        G = E.Game(game)
        rom, lvl, bad = M.load_maps(game, {k: len(v) for k, v in G.img.items()})
        ev = {fn: level_evidence(G, fn, rows) for fn, rows in lvl.items()}
        c = collections.Counter(); ex = {}
        for row in csv.reader(open(M.S + '/ledger/CONFLICTS_%s.tsv' % game), delimiter='\t'):
            if row[0] in ('space', 'ROM'): continue
            if row[8] not in ('display_list', 'vertex') or row[8] != row[14]: continue
            k = (row[8], row[4], status(ev[row[0]], row[8], int(row[5], 16), int(row[6], 16)), row[10],
                 status(ev[row[0]], row[8], int(row[11], 16), int(row[12], 16)))
            c[k] += 1; ex.setdefault(k, row[:3] + [row[5], row[6], row[11], row[12]])
        print(game)
        for k, v in c.most_common(): print('  ', v, k, ex[k])
