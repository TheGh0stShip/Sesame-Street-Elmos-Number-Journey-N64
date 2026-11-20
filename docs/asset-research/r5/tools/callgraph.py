#!/usr/bin/env python3
"""callgraph.py LETTER_ROM NUMBER_ROM FUNCTIONS_JSON SRCROOT FILESDIR OUTDIR

Builds, for both games, the call graph from the machine code and decides reachability from the entry point.
Edges: jal/j targets, branches leaving a function, %hi/%lo function addresses built in code (thread entries,
callbacks), and function addresses stored as words in .data/.rodata or inside the loaded asset images
(level images after inflation, common block, menu files). A data word is attributed to the data object that
contains it (object start = nearest lower address that code builds); an edge then runs from every function
that builds that object's address to the pointed-to function. Data words whose object no code builds are roots.

Writes OUTDIR/graph_<game>.json and OUTDIR/UNREACHABLE_<game>.tsv.
SRCROOT is .../sesverify/v16 (SRCROOT/<game>/src/func_X.c gives the one-line description).
FILESDIR is .../sesassets/files (FILES_<game>.tsv gives the asset file ranges)."""
import sys, os, re, json, bisect
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from seslib import *

# letter library ranges (address of first function of each run); number is mapped through twin hashes
LIB_LETTER = [(0x80000400, 'game'), (0x8004C500, 'libmus'), (0x80054D30, 'libultra'),
              (0x80067B60, 'libm/libc/libgcc'), (0x80069430, 'GSM')]


def lib_letter(v):
    lab = 'game'
    for a, l in LIB_LETTER:
        if v >= a: lab = l
    return lab


def desc(srcroot, game, v):
    p = os.path.join(srcroot, game, 'src', 'func_%08X.c' % v)
    if not os.path.exists(p): return ''
    txt = open(p, errors='replace').read()
    for m in re.finditer(r'/\*(.*?)\*/|//(.*)', txt, re.S):
        s = (m.group(1) or m.group(2) or '').strip().strip('*').strip()
        s = ' '.join(s.split())
        if s and not s.startswith('===') and 'unhandled instructions' not in s and not s.startswith('0x'):
            return s[:160]
    return ''


def asset_images(rom, game, filesdir):
    """yield (name, ram_base, bytes) for every image the game loads into RAM wholesale."""
    for a, b, cls, note in files_table(os.path.join(filesdir, 'FILES_%s.tsv' % game)):
        if cls == 'gzip_level':
            yield 'gz@%06X' % a, 0x80154000, gunzip_member(rom, a, b)
        elif cls == 'gfx_pack':
            base = 0x8024B000 if note.startswith('common') else 0x80154000
            yield 'raw@%06X' % a, base, rom[a:b]


def build(rom, game, fs, filesdir):
    starts = {f['vram']: f for f in fs}
    own = owner_finder(fs)
    code = scan_code(rom, fs)
    edges = {v: set() for v in starts}      # caller -> callees
    why = {v: [] for v in starts}           # callee -> list of reasons
    built = {}                              # address -> set of functions building it
    for v, c in code.items():
        for t in c['calls']:
            o = own(t)
            if o is not None and o != v and t == o:
                edges[v].add(o); why[o].append('call:%08X' % v)
            elif o is not None and o != v:
                edges[v].add(o); why[o].append('branch-into:%08X' % v)
        for t, pc in c['addrs']:
            built.setdefault(t, set()).add(v)
            if t in starts and t != v:
                edges[v].add(t); why[t].append('addr:%08X' % v)
    # fall-through: a function whose last control transfer is not an unconditional jump runs into the next one
    fl = sorted(starts)
    for i, v in enumerate(fl[:-1]):
        f = starts[v]; nxt = fl[i + 1]
        if v + f['size'] != nxt: continue
        ws = [int.from_bytes(rom[o:o + 4], 'big') for o in range(f['rom'], f['rom'] + f['size'], 4)]
        while ws and ws[-1] == 0: ws.pop()
        def is_uncond(w):
            op = w >> 26
            return (op == 2 or (op == 0 and (w & 63) == 8) or (op == 4 and (w >> 16) & 0x3FF == 0)
                    or (op == 1 and ((w >> 16) & 31) == 1 and (w >> 21) & 31 == 0) or w == 0x42000018)
        uncond = bool(ws) and (is_uncond(ws[-1]) or (len(ws) > 1 and is_uncond(ws[-2])))
        if not uncond:
            edges[v].add(nxt); why[nxt].append('falls-through-from:%08X' % v)
    # data words
    dlo, dhi = SEG[game]['data'][0], SEG[game]['rodata'][1]
    objs = sorted(a for a in built if dlo <= a < dhi)
    roots = {ENTRY}
    rootwhy = {ENTRY: ['entry point']}
    dptr = {}
    for a, w in data_words(rom, game):
        if w in starts:
            i = bisect.bisect_right(objs, a) - 1
            obj = objs[i] if i >= 0 else None
            dptr.setdefault(w, []).append((a, obj))
            users = built.get(obj, set()) if obj is not None else set()
            if users:
                for u in users:
                    edges[u].add(w)
                why[w].append('data:%08X(obj %08X)' % (a, obj))
            else:
                roots.add(w); rootwhy.setdefault(w, []).append('data-root:%08X' % a)
    aptr = {}
    for name, base, img in asset_images(rom, game, filesdir):
        for o in range(0, len(img) - 3, 4):
            w = int.from_bytes(img[o:o + 4], 'big')
            if w in starts:
                aptr.setdefault(w, []).append('%s+%X' % (name, o))
    for w, l in aptr.items():
        roots.add(w); rootwhy.setdefault(w, []).append('asset-ptr:' + l[0])
    # reachability (entry only, then entry + every data/asset pointer root)
    def walk(rs):
        seen, st = set(), list(rs)
        while st:
            v = st.pop()
            if v in seen or v not in starts: continue
            seen.add(v); st.extend(edges[v])
        return seen
    reach_entry = walk({ENTRY})
    reach_all = walk(roots)
    return dict(starts=starts, edges=edges, why=why, roots=roots, rootwhy=rootwhy, reach_entry=reach_entry,
                reach_all=reach_all, dptr=dptr, aptr=aptr, built=built, code=code)


def main(lrom, nrom, fj, srcroot, filesdir, outdir):
    os.makedirs(outdir, exist_ok=True)
    allf = json.load(open(fj))
    hand = {}
    for line in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'descriptions.tsv')):
        if line.startswith('#') or not line.strip(): continue
        g_, a_, d_ = line.rstrip('\n').split('\t')
        hand[(g_, int(a_, 16))] = d_
    lhash = {f['vram']: f['hash'] for f in allf if f['game'] == 'letter'}
    hand_by_hash = {lhash[a]: d for (g_, a), d in hand.items() if g_ == 'letter' and a in lhash}
    letter_lib = {}
    for f in allf:
        if f['game'] == 'letter': letter_lib[f['hash']] = lib_letter(f['vram'])
    for game, rp in (('letter', lrom), ('number', nrom)):
        rom = open(rp, 'rb').read()
        fs = load_functions(fj, game)
        g = build(rom, game, fs, filesdir)
        lib, prev = {}, 'game'
        for f in fs:
            l = lib_letter(f['vram']) if game == 'letter' else letter_lib.get(f['hash'], prev)
            if f['vram'] == ENTRY: l = 'boot'
            lib[f['vram']] = l; prev = l
        js = {'%08X' % v: {'size': g['starts'][v]['size'], 'cls': g['starts'][v]['cls'], 'lib': lib[v],
                           'hash': g['starts'][v]['hash'],
                           'callees': sorted('%08X' % x for x in g['edges'][v]),
                           'refs_in': g['why'][v], 'root': g['rootwhy'].get(v, []),
                           'reach_entry': v in g['reach_entry'], 'reach_any': v in g['reach_all']}
              for v in g['starts']}
        json.dump(js, open(os.path.join(outdir, 'graph_%s.json' % game), 'w'), indent=0)
        with open(os.path.join(outdir, 'UNREACHABLE_%s.tsv' % game), 'w') as o:
            o.write('func\tlibrary\tcls\tsize\tstatus\treferenced_by(unreachable code only)\tdescription(matched C)\n')
            n = {}
            for v in sorted(g['starts']):
                if v in g['reach_all']: continue
                st = 'unreferenced' if not g['why'][v] else 'referenced only from unreachable code'
                o.write('func_%08X\t%s\t%s\t%d\t%s\t%s\t%s\n' % (v, lib[v], g['starts'][v]['cls'],
                        g['starts'][v]['size'], st, ','.join(g['why'][v]) or '-',
                        desc(srcroot, game, v) or hand.get((game, v)) or hand_by_hash.get(g['starts'][v]['hash'], '')))
                k = lib[v]; n[k] = (n.get(k, (0, 0))[0] + 1, n.get(k, (0, 0))[1] + g['starts'][v]['size'])
        print(game, len(fs), 'functions; reachable from entry', len(g['reach_entry']), '; with data/asset roots',
              len(g['reach_all']), '; roots', len(g['roots']), '; asset ptrs', len(g['aptr']), '; unreachable by lib', n)


if __name__ == '__main__':
    main(*sys.argv[1:7])
