#!/usr/bin/env python3
"""rebuild_levels.py GAME OUTDIR [--owners lvlB,lvlC,...]
Rebuild each of the 8 inflated level images of GAME from the owners' EXTRACTED forms and compare it byte for byte
with the inflated original (lvlA/out/<game>/<name>.bif.bin).

Layer below r4's whole-ROM rebuild: r4 re-compressed lvlA's inflated copies; this builds those inflated images.
Every owner's extractor is run in its check direction through lprobe.py: the ROM it opens is a ProbeBytes, the
zlib it imports is a shim whose inflate of a level member returns a ProbeBytes in image space, and each equality
test of an image slice against the bytes the check re-encoded from its extracted files is captured. Only those
captured bytes (plus generated padding / ELF .comment strings) go into the rebuilt image; the original image is
read only to compare and to fill the listed exceptions.

The tiling comes from ../ledger/LEVELS_<game>.tsv. Each row is filled from its owner's captures first, then from
any other owner's ((other-owner)); `pad` rows are generated as zeros and `elf_comment` rows as repeated
"GCC: (GNU) 2.7.2\\0"; whatever is left is an exception (original bytes placed, reason recorded).
Outputs: OUTDIR/LEVELREBUILD_<game>.tsv, OUTDIR/LEVELSUMMARY_<game>.json, OUTDIR/<game>/<name>.bif.bin (lab only).
Prerequisite: lvlB's (g3-extended) extractor run in extract mode into OUTDIR/lvlB/<game> (see FORMAT.md)."""
import sys, os, re, json, hashlib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lprobe
L = os.path.expanduser('${N64DK_LAB}'); S = L + '/sesassets'
ROMS = {'letter': L + "/roms/Sesame Street - Elmo's Letter Adventure (USA).z64",
        'number': L + "/roms/Sesame Street - Elmo's Number Journey (USA).z64"}
GCC = b'GCC: (GNU) 2.7.2\0\0'   # each .comment entry is "\0GCC: (GNU) 2.7.2\0"; the row starts after the first NUL
CODE = {1: 'rebuilt-from-extracted', 2: 'rebuilt-from-extracted(other-owner)', 5: 'extracted-verbatim',
        6: 'extracted-verbatim(other-owner)', 7: 'generated', 9: 'exception'}
HEXWORDS = re.compile(r'^\s*([0-9A-Fa-f]{8}\s+[0-9A-Fa-f]{8}\s*)?$')


def jobs(game, outdir):
    rp = ROMS[game]
    j = {
        'lvlB': (S + '/lvlB/tools/extract_level_gfx.py', ['check', rp, game, os.path.join(outdir, 'lvlB', game)]),
        'lvlC': (S + '/lvlC/tools/extract_level_data.py', ['check', rp, game, S + '/lvlC/out/%s/json' % game]),
        'lvlD': (S + '/lvlD/tools/extract_level_misc.py', ['check', rp, game, S + '/lvlD/out/' + game]),
        'r1': (S + '/r1/tools/extract_r1.py', [rp, game, S + '/r1/out', 'check']),
    }
    if game == 'letter':
        j['lvlE'] = (S + '/lvlE/tools/extract_level_rest.py', ['check', rp, game, S + '/lvlE/out'])
        j['lvlF'] = (S + '/lvlF/tools/extract_level_rest.py', [rp, game, S + '/lvlF/out', 'check'])
    else:
        j['lvlG'] = (S + '/lvlG/tools/extract_level_rest.py', [rp, game, S + '/lvlG/out', 'check'])
        j['lvlH'] = (S + '/lvlH/tools/extract_level_rest.py', [rp, game, S + '/lvlH/out', 'check'])
    return j


_jc = {}
def form_of(owner, path):
    """'verbatim' when the extracted file the capture was re-encoded from is a raw dump: a .bin, a JSON whose
    payload is a hex string, a text file of hex word pairs, or lvlF's display-list word lists; else 'decoded'."""
    if not path: return 'decoded'
    if path.endswith('.bin'): return 'verbatim'
    if path.endswith('.json'):
        if path not in _jc:
            try: j = json.load(open(path))
            except Exception: j = None
            v = 'decoded'
            if isinstance(j, dict):
                for d in (j, j.get('data')):
                    if isinstance(d, dict) and d.get('hex'): v = 'verbatim'
                if owner == 'lvlF' and j.get('cls') == 'display_list': v = 'verbatim'
            _jc[path] = v
        return _jc[path]
    if path.endswith('.txt') and owner in ('lvlE', 'lvlG', 'lvlH'):
        if path not in _jc:
            _jc[path] = 'verbatim' if all(HEXWORDS.match(l) for l in open(path)) else 'decoded'
        return _jc[path]
    return 'decoded'


def fb_lvlG(game, rows, have, orig):
    """lvlG's check classifies its territory afresh; for 3 rows its current classifier disagrees with the MAP.tsv the
    ledger uses (it reads unknown_<off>.bin instead of mesh_bbox_<off>.json, and a tail at +4). For ledger rows of
    lvlG that the check did not capture (or captured only from a raw .bin), call lvlG's own decoder read_item() on the extracted file of that row."""
    g = {'__name__': 'lvlG', '__file__': S + '/lvlG/tools/extract_level_rest.py'}
    exec(compile(open(g['__file__']).read(), g['__file__'], 'exec'), g)
    out = []
    for r in rows:
        if r['owner'] != 'lvlG' or r['kind'] == 'pad': continue
        h = have.get(r['file'])
        if h and all(x == 1 for x in h[1][r["s"]:r["e"]]): continue   # already re-encoded from a decoded form
        lvl = r['file'].replace('.bif', ''); stem = os.path.join(S, 'lvlG/out', lvl, '%s_%05X' % (r['cls'], r['s']))
        f = [x for x in (stem + '.json', stem + '.bin', stem + '.txt', stem + '_index.png') if os.path.exists(x)]
        if not f: continue
        b = g['read_item'](S + '/lvlG/out', lvl, (r['s'], r['e'], r['cls']))
        out.append((r['file'], r['s'], b, f[0]))
    return out
FALLBACK = {'lvlG': fb_lvlG}


def load_rows(game):
    rows = []
    for l in open(S + '/ledger/LEVELS_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if c[0] == 'file': continue
        rows.append(dict(file=c[0], s=int(c[1], 16), e=int(c[2], 16), cls=c[3], desc=c[5], owner=c[6], kind=c[8]))
    return rows


def members(game):
    m = []
    for l in open(S + '/ledger/LEDGER_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if c[5] == 'lvlA' and c[2] == 'gzip_level':
            m.append((int(c[0], 16), int(c[1], 16), re.search(r'(\w+\.bif)', c[4]).group(1)))
    return m


def reason(row, ran):
    o, cls, kind = row['owner'], row['cls'], row['kind']
    if o not in ran: return 'owner %s has no check mode run here' % o
    if ran[o]['exit'] == 'crash': return 'owner %s check crashed before reaching this range: %s' % (o, ran[o]['error'])
    if o == 'lvlB' and kind == 'model_header': return 'lvlB model headers are MAP-only (no extracted form)'
    if kind == 'pad': return 'padding with non-zero (stale) bytes and no owner capture'
    if kind == 'elf_comment': return 'comment bytes differ from the generated GCC strings'
    return 'owner %s check does not re-encode this range from an extracted file' % o


def main():
    game, outdir = sys.argv[1], sys.argv[2]
    only = None
    if '--owners' in sys.argv: only = sys.argv[sys.argv.index('--owners') + 1].split(',')
    os.makedirs(os.path.join(outdir, game), exist_ok=True)
    mem = members(game); rows = load_rows(game)
    orig = {n: open(S + '/lvlA/out/%s/%s.bin' % (game, n), 'rb').read() for _, _, n in mem}
    # per owner: image -> (buf, have) ; have: 0 none, 1 decoded, 5 verbatim
    cap = {}; ran = {}
    for owner, (script, argv) in jobs(game, outdir).items():
        if only and owner not in only: continue
        log, opened, code, out, err = lprobe.run_script(script, argv, ROMS[game], mem)
        n = nb = 0
        # unequal tests first, equal ones painted over them: a check may also compare a slice with a constant
        # (r1 counts zero palette entries with pal[i:i+2] == b'\0\0'); such side tests must not hide its real test
        for sp, a, b, eq, last in sorted(log, key=lambda t: t[3]):
            if sp not in orig: continue
            c = cap.setdefault(owner, {}).setdefault(sp, (bytearray(len(orig[sp])), bytearray(len(orig[sp]))))
            e = min(a + len(b), len(orig[sp]))
            if e <= a: continue
            c[0][a:e] = b[:e - a]
            c[1][a:e] = bytes([5 if form_of(owner, last) == 'verbatim' else 1]) * (e - a)
            n += 1; nb += e - a
        if owner in FALLBACK:
            fb = FALLBACK[owner](game, rows, cap.get(owner, {}), orig)
            for sp, a, b, last in fb:
                c = cap.setdefault(owner, {}).setdefault(sp, (bytearray(len(orig[sp])), bytearray(len(orig[sp]))))
                c[0][a:a + len(b)] = b; c[1][a:a + len(b)] = bytes([5 if form_of(owner, last) == 'verbatim' else 1]) * len(b)
            n += len(fb); nb += sum(len(x[2]) for x in fb)
            fbn = [(sp, '0x%05X' % a) for sp, a, b, last in fb]
        else: fbn = []
        ran[owner] = dict(encoder_fallback=fbn, exit=code, error=err, image_captures=n, captured_bytes=nb, files_opened=len(set(opened)),
                          tail=out.strip().splitlines()[-4:])
        print(owner, json.dumps(ran[owner]), flush=True)
    owners = list(cap)
    out_rows = []; summary = dict(game=game, owners=ran, images={})
    for _, _, name in mem:
        O = orig[name]; N = len(O)
        img = bytearray(N); src = bytearray(N)   # src code per byte
        R = [r for r in rows if r['file'] == name]
        for r in R:
            s, e = r['s'], r['e']
            if r['kind'] == 'pad':
                img[s:e] = bytes(e - s); src[s:e] = bytes([7]) * (e - s)
            elif r['kind'] == 'elf_comment':
                g = (GCC * ((e - s) // len(GCC) + 1))[:e - s]
                img[s:e] = g; src[s:e] = bytes([7]) * (e - s)
            order = [r['owner']] + [o for o in owners if o != r['owner']]
            for o in reversed(order):        # owner painted last = wins
                if o not in cap or name not in cap[o]: continue
                buf, have = cap[o][name]
                i = s
                while i < e:
                    if not have[i]: i += 1; continue
                    j = i
                    while j < e and have[j] == have[i]: j += 1
                    if not (src[i] == 7 and O[i:j] == img[i:j]):   # generated bytes that are right stay generated
                        code = have[i] + (1 if o != r['owner'] else 0)
                        img[i:j] = buf[i:j]; src[i:j] = bytes([code]) * (j - i)
                    i = j
            # generated bytes that are wrong and that no owner rebuilt -> exception
            for i in range(s, e):
                if src[i] == 7 and img[i] != O[i]: src[i] = 0
        # exceptions: original bytes placed
        for i in range(N):
            if src[i] == 0: src[i] = 9; img[i] = O[i]
        open(os.path.join(outdir, game, name + '.bin'), 'wb').write(img)
        st = dict(size=N, equal=bytes(img) == O, sha1_orig=hashlib.sha1(O).hexdigest(), sha1_rebuilt=hashlib.sha1(img).hexdigest(),
                  bytes={}, unequal={})
        for r in R:
            s, e = r['s'], r['e']; i = s
            while i < e:
                c = src[i]; eq = img[i] == O[i]; j = i
                while j < e and src[j] == c and (img[j] == O[j]) == eq: j += 1
                note = reason(r, ran) if c == 9 else ''
                out_rows.append([name, '0x%05X' % i, '0x%05X' % j, str(j - i), r['owner'], r['cls'], r['kind'], CODE[c],
                                 'yes' if eq else 'no', note])
                st['bytes'][CODE[c]] = st['bytes'].get(CODE[c], 0) + j - i
                if not eq: st['unequal'][CODE[c]] = st['unequal'].get(CODE[c], 0) + j - i
                i = j
        tiled = sum(r['e'] - r['s'] for r in R)
        st['ledger_bytes'] = tiled
        st['rebuilt'] = sum(v for k, v in st['bytes'].items() if k.startswith('rebuilt'))
        st['verbatim'] = sum(v for k, v in st['bytes'].items() if k.startswith('extracted-verbatim'))
        st['generated'] = st['bytes'].get('generated', 0)
        st['excepted'] = st['bytes'].get('exception', 0)
        st['equal_without_exceptions'] = st['equal'] and st['excepted'] == 0
        summary['images'][name] = st
        print(name, json.dumps({k: st[k] for k in ('size', 'equal', 'rebuilt', 'verbatim', 'generated', 'excepted', 'unequal')}), flush=True)
    with open(os.path.join(outdir, 'LEVELREBUILD_%s.tsv' % game), 'w') as f:
        f.write('file\tstart\tend\tbytes\towner\tclass\tkind\tsource\tequal\tnote\n')
        for r in out_rows: f.write('\t'.join(r) + '\n')
    tot = {}
    for st in summary['images'].values():
        for k in ('size', 'rebuilt', 'verbatim', 'generated', 'excepted'): tot[k] = tot.get(k, 0) + st[k]
    tot['images_equal'] = sum(st['equal'] for st in summary['images'].values())
    summary['total'] = tot
    json.dump(summary, open(os.path.join(outdir, 'LEVELSUMMARY_%s.json' % game), 'w'), indent=1)
    print('TOTAL', json.dumps(tot))


if __name__ == '__main__':
    main()
