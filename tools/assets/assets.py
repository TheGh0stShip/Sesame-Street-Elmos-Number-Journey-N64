#!/usr/bin/env python3
"""assets.py: the one driver for every asset owner (extract: ROM -> files; encode: files -> bytes; rom: layout).

  assets.py extract ROM GAME ASSETS [OWNER ...]     read the ROM; write every owner's decoded files into ASSETS/<owner>/
  assets.py encode  GAME ASSETS PIECES OWNER        re-encode one owner from ASSETS alone -> PIECES/<owner>.pcs
  assets.py rspbin  GAME ASSETS OUTDIR             assemble the RSP microcode (ASSETS/rsp) into OUTDIR/*.bin (main segment)
  assets.py rom     GAME ASSETS PIECES MAIN GZIP OUT
                                                    lay every piece out by tables/layout_<GAME>.tsv, build and gzip the
                                                    16 level images, apply the stale-tail rule and the header CRCs

Owners and their codecs live in owners/<owner>/tools/ (copied unchanged from the asset agents' tools; the two
edits are marked "(pa)"). `encode` and `rom` never read the ROM: an audit hook refuses to open any file outside
this tree's assets/, build/, tools/ and the Python/binutils installation (the ROM path is never passed in).

Encoders (see README "Assets"):
  sink    the owner's own `check` mode, run by sink.py against a contentless ROM stand-in (fill 0x00 and 0xFF,
          captures must be identical): songs packA packB packC hud lvlC lvlD lvlF
  direct  the owner's codec functions called on its files: audio (ptr banks + VADPCM via g2's SDK-encoder port),
          speech (repack), sprA (encode), lvlB, lvlE, lvlG (read_item), lvlH (encode), r1, boot (ROM header fields
          + IPL3 assembly), rsp (RSP microcode assembly), tex (packC textures from PNG), rest (decode_rest forms)
Pieces: a list of (space, offset, bytes); space = 'ROM' or '<level>.bif' (offset inside the inflated image)."""
import sys, os, json, struct, pickle, subprocess, importlib.util, glob, hashlib

T = os.path.dirname(os.path.abspath(__file__))           # tools/assets
OW = os.path.join(T, 'owners')
TREE = os.path.dirname(os.path.dirname(T))
PY = sys.executable
SINK_OWNERS = ('songs', 'packA', 'packB', 'packC', 'hud', 'lvlC', 'lvlD', 'lvlF')
GAME_OWNERS = {'letter': ('lvlE', 'lvlF'), 'number': ('lvlG', 'lvlH')}
ALL = ('audio', 'adpcm', 'songs', 'speech', 'sprA', 'packA', 'packB', 'packC', 'tex', 'hud', 'boot', 'rsp',
       'lvlB', 'lvlC', 'lvlD', 'lvlE', 'lvlF', 'lvlG', 'lvlH', 'r1', 'rest')


def owners_of(game):
    other = GAME_OWNERS['number' if game == 'letter' else 'letter']
    return [o for o in ALL if o not in other]


def script(o, f): return os.path.join(OW, o, 'tools', f)


def load(path, name, drop_call=None):
    """import an owner's script as a module (without running its main)"""
    for m in ('png', 'n64tex', 'f3dex', 'gaps'): sys.modules.pop(m, None)
    sys.path.insert(0, os.path.dirname(path))
    try:
        src = open(path).read()
        if drop_call: src = src.replace(drop_call, '\n')
        g = {'__name__': name, '__file__': path}
        exec(compile(src, path, 'exec'), g)
    finally:
        sys.path.pop(0)
    return type('M', (), g)


# ------------------------------------------------------------------ layout table
def layout(game):
    """tables/layout_<game>.tsv: '#member name start end image_size', '#segment name start end', rows
    space start end owner kind encoder; '#leftover start end' rows."""
    mem, seg, rows, left = [], {}, [], []
    for l in open(os.path.join(T, 'tables', 'layout_%s.tsv' % game)):
        c = l.rstrip('\n').split('\t')
        if c[0] == '#member': mem.append((int(c[2], 16), int(c[3], 16), c[1], int(c[4])))
        elif c[0] == '#segment': seg[c[1]] = (int(c[2], 16), int(c[3], 16))
        elif c[0] == '#leftover': left.append((int(c[1], 16), int(c[2], 16)))
        elif c[0] == '#rom_size': seg['rom'] = (0, int(c[1], 16))
        elif not c[0].startswith('#') and c[0] != 'space':
            rows.append((c[0], int(c[1], 16), int(c[2], 16), c[3], c[4], c[5]))
    return mem, seg, rows, left


# ------------------------------------------------------------------ guard: no ROM access while encoding
def seal(extra=()):
    allowed = [os.path.join(TREE, d) for d in ('assets', 'build', 'tools')] + list(extra) + \
              [sys.prefix, sys.base_prefix, sys.exec_prefix, '/usr/lib/python3', '/usr/lib/python3.', '/dev/null',
               '/tmp', '/proc', '/etc', '/usr/share/zoneinfo', os.path.dirname(os.path.dirname(PY))]
    allowed += [p for p in sys.path if p]
    bu = os.environ.get('BINUTILS')
    if bu: allowed.append(bu)
    allowed = [os.path.realpath(a) for a in allowed]

    def hook(ev, args):
        if ev == 'open' and args and isinstance(args[0], (str, bytes, os.PathLike)):
            p = os.path.realpath(os.fsdecode(args[0]))
            if not any(p == a or p.startswith(a.rstrip('/') + '/') or p.startswith(a) for a in allowed):
                raise PermissionError('assets.py encode: refusing to open %s (outside the tree)' % p)
            if p.endswith(('.z64', '.n64', '.v64')) and not p.startswith(os.path.realpath(TREE) + '/build'):
                raise PermissionError('assets.py encode: refusing to open a ROM image %s' % p)
    sys.addaudithook(hook)


# ------------------------------------------------------------------ extract
def extract(rom, game, A, only=None):
    rom = os.path.abspath(rom); A = os.path.abspath(A); os.makedirs(A, exist_ok=True)
    tb = os.path.join(T, 'tables')
    steps = {
        'songs': [script('songs', 'extract_songs.py'), rom, game, A + '/songs'],
        'packA': [script('packA', 'extract_packA.py'), rom, game, A + '/packA'],
        'packB': [script('packB', 'extract_packB.py'), rom, game, A + '/packB'],
        'packC': [script('packC', 'extract_packC.py'), rom, game, A + '/packC'],
        'hud': [script('hud', 'extract_hud.py'), rom, game, A + '/hud'],
        'lvlB': [script('lvlB', 'extract_level_gfx.py'), rom, game, A + '/lvlB'],
        'lvlC': [script('lvlC', 'extract_level_data.py'), rom, game, A + '/lvlC'],
        'lvlD': [script('lvlD', 'extract_level_misc.py'), rom, game, A + '/lvlD'],
        'lvlE': [script('lvlE', 'extract_level_rest.py'), rom, game, A + '/lvlE'],
        'lvlF': [script('lvlF', 'extract_level_rest.py'), rom, game, A + '/lvlF'],
        'lvlG': [script('lvlG', 'extract_level_rest.py'), rom, game, A + '/lvlG'],
        'lvlH': [script('lvlH', 'extract_level_rest.py'), rom, game, A + '/lvlH'],
        'r1': [script('r1', 'extract_r1.py'), rom, game, A + '/r1'],
        'audio': [script('audio', 'extract_audio.py'), rom, A + '/audio'],
        'adpcm': [script('g2', 'adpcm_g2.py'), 'extract', rom, A + '/audio', A + '/adpcm'],
        'speech': [script('speech', 'extract_speech.py'), rom, A + '/speech'],
        'sprA': [script('sprA', 'extract_sprites.py'), rom, game, A + '/sprA'],
        'boot': [script('g2', 'boot_g2.py'), 'extract', rom, A + '/boot'],
        'rsp': [script('g2', 'rsp_g2.py'), 'extract', rom, os.path.join(tb, 'ucode_%s.json' % game), A + '/rsp'],
        'tex': [script('g2', 'tex_g2.py'), 'extract', rom, game, os.path.join(tb, 'tex_territory_%s.tsv' % game), A + '/tex'],
    }
    for o in owners_of(game):
        if only and o not in only: continue
        if o == 'rest':
            extract_rest(rom, game, A); continue
        if o == 'lvlG': extract_lvlG_extra(rom, game, A)
        print('extract', o, flush=True)
        r = subprocess.run([PY] + steps[o], cwd=os.path.dirname(steps[o][0]))
        if r.returncode: print('extract %s: exit code %d (its own self-test; encode decides)' % (o, r.returncode), flush=True)


def lvlG_extra(game):
    """tables/lvlG_extra_<game>.tsv: ledger rows of lvlG that its current classifier draws differently (Number bump
    0x3EB08 mesh_bbox, 0x3EB44 mesh_header_tail; g3 REPORT): written and read with lvlG's own write_item/read_item"""
    p = os.path.join(T, 'tables', 'lvlG_extra_%s.tsv' % game)
    if not os.path.exists(p): return []
    return [(c[0], int(c[1], 16), int(c[2], 16), c[3]) for c in (l.rstrip('\n').split('\t') for l in open(p)) if not c[0].startswith('#')]


def extract_lvlG_extra(rom, game, A):
    rows = lvlG_extra(game)
    if not rows: return
    M = load(script('lvlG', 'extract_level_rest.py'), 'lvlG'); DR = DRmod()
    mem = {n: (s, e) for s, e, n, _ in layout(game)[0]}; data = open(rom, 'rb').read()
    for sp, a, b, cls in rows:
        d = DR.inflate(data, *mem[sp]); lvl = sp[:-4]
        os.makedirs(os.path.join(A, 'lvlG', lvl), exist_ok=True)
        pl = list(struct.unpack('>6f', d[a:b])) if cls == 'mesh_bbox' else None
        M.write_item(os.path.join(A, 'lvlG'), lvl, d, (a, b, cls, '', '', '', pl))


def DRmod():
    return load(script('r6', 'decode_rest.py'), 'decode_rest')


def extract_rest(rom, game, A):
    DR = DRmod(); d = os.path.join(A, 'rest'); os.makedirs(d, exist_ok=True)
    items = json.load(open(os.path.join(T, 'tables', 'rest_plan_%s.json' % game)))
    mem = {n: (s, e) for s, e, n, _ in layout(game)[0]}
    data = open(rom, 'rb').read(); imgs = {}
    for it in items:
        if it['kind'] == 'r1': continue            # r1's own files (owner r1) cover these
        if it['where'] == 'rom': src = data
        else:
            if it['where'] not in imgs: imgs[it['where']] = DR.inflate(data, *mem[it['where']])
            src = imgs[it['where']]
        DR.decode_item(it, src[it['start']:it['end']], d)
    json.dump([it for it in items if it['kind'] != 'r1'], open(os.path.join(d, 'index.json'), 'w'), indent=0)
    print('extract rest', len(items), 'items')


# ------------------------------------------------------------------ encoders
def sink_argv(o, game, A, R):
    return {'songs': ['check', R, game, A + '/songs'], 'packA': ['check', R, game, A + '/packA'],
            'packB': [R, game, A + '/packB', 'check'], 'packC': ['check', R, game, A + '/packC'],
            'hud': ['check', R, game, A + '/hud'], 'lvlC': ['check', R, game, A + '/lvlC'],
            'lvlD': ['check', R, game, A + '/lvlD'], 'lvlF': [R, game, A + '/lvlF', 'check']}[o]


SINK_SCRIPT = {'songs': 'extract_songs.py', 'packA': 'extract_packA.py', 'packB': 'extract_packB.py',
               'packC': 'extract_packC.py', 'hud': 'extract_hud.py', 'lvlC': 'extract_level_data.py',
               'lvlD': 'extract_level_misc.py', 'lvlF': 'extract_level_rest.py'}


def enc_sink(o, game, A):
    import sink
    mem, seg, _, _ = layout(game)
    runs = []
    for fill in (0x00, 0xFF):
        log, code, out, err = sink.run_check(script(o, SINK_SCRIPT[o]), sink_argv(o, game, A, sink.FAKE_ROM), seg['rom'][1], mem, fill)
        if err: sys.exit('%s check crashed: %s' % (o, err))
        runs.append(log)
    if runs[0] != runs[1]:
        sys.exit('%s: captures depend on the ROM stand-in content (fill 00 vs FF): not standalone' % o)
    return [(sp, a, b) for sp, a, b in runs[0]]


def vsink():
    import sink
    log = []
    return log, (lambda size: sink.VBytes(bytes(size), 0, 'ROM', log))


def enc_audio(game, A):
    """pointer banks from ptrbank.json (audio's check_ptrbank, its comparison captured), VADPCM frames from WAV +
    book (g2's SDK encoder port + per-frame header field), sample tails, the wave-bank signature (format magic)"""
    import sink
    M = load(script('audio', 'extract_audio.py'), 'extract_audio')
    G = load(script('g2', 'adpcm_g2.py'), 'adpcm_g2')
    g = [v for v in M.GAMES.values() if v['name'] == game][0]
    out = []
    for bi, (po, wo) in enumerate(zip(g['ptr'], g['wbk'])):
        log = []; vrom = sink.VBytes(bytes(wo), 0, 'ROM', log)
        dump = json.load(open(os.path.join(A, 'audio', game, 'bank%d' % (bi + 1), 'ptrbank.json')))
        M.check_ptrbank(vrom, po, wo, dump)
        out += [(sp, a, b) for sp, a, b in log if a == po]
        out.append(('ROM', wo, b'N64 WaveTables \0'))
    hd = json.load(open(os.path.join(A, 'adpcm', game, 'headers.json')))['samples']
    ao = os.path.join(A, 'audio')
    for js, side in G.samples(ao, game):
        key = os.path.relpath(js, os.path.join(ao, game))[:-5]
        b = G.generate(G.read_wav(js[:-5] + '.wav'), side, hd.get(key, {}))
        out.append(('ROM', side['rom'], b + bytes.fromhex(side.get('tail_hex', ''))))
    return out


def enc_speech(game, A):
    M = load(script('speech', 'extract_speech.py'), 'extract_speech')
    out = []
    for p in sorted(glob.glob(os.path.join(A, 'speech', '%s_*.params.json' % game))):
        j = json.load(open(p))
        b, _ = M.repack(dict(frames=j['frames'], rate=j['rate'], words=j['words']), j['runs'], struct.unpack('>I', j['magic'].encode())[0])
        out.append(('ROM', j['rom'], b))
    return out


def enc_sprA(game, A):
    M = load(script('sprA', 'extract_sprites.py'), 'extract_sprites', drop_call='\nmain()')
    return [('ROM', int(os.path.basename(p)[:6], 16), M.encode(json.load(open(p))))
            for p in sorted(glob.glob(os.path.join(A, 'sprA', game, '*.json')))]


def enc_boot(game, A):
    B = load(script('g2', 'boot_g2.py'), 'boot_g2')
    j = json.load(open(os.path.join(A, 'boot', game, 'header.json')))
    hdr = bytearray(0x40)
    for f in j['fields']:
        o, n = f['off'], f['size']
        if f['kind'] in ('u32', 'u8'): hdr[o:o + n] = int(f['value'], 16).to_bytes(n, 'big')
        elif f['kind'] == 'ascii': hdr[o:o + n] = f['value'].encode('ascii')
    # crc fields stay zero here; `rom` computes them over the finished image
    ipl = B.assemble(open(os.path.join(A, 'boot', game, 'ipl3.s')).read(), B.IPL_VRAM, 'ipl3')
    return [('ROM', 0, bytes(hdr)), ('ROM', 0x40, ipl)]


def enc_rsp(game, A):
    R = load(script('g2', 'rsp_g2.py'), 'rsp_g2')
    u = json.load(open(os.path.join(T, 'tables', 'ucode_%s.json' % game)))
    return [('ROM', int(u[key]['rom'], 16), R.assemble(open(os.path.join(A, 'rsp', game, name + '.s')).read()))
            for key, name, base, kind in R.BLOBS]


def enc_tex(game, A):
    for m in ('png', 'n64tex'): sys.modules.pop(m, None)
    sys.path.insert(0, os.path.join(OW, 'gfx', 'tools'))
    import n64tex, png
    sys.path.pop(0)
    out = []
    for it in json.load(open(os.path.join(A, 'tex', game, 'index.json')))['items']:
        t, _ = n64tex.encode(it['fmt'], it['bits'], png.open_png(os.path.join(A, 'tex', game, it['name'] + '.png')))
        tl = it.get('tail')
        if tl: t += bytes(tl['zero']) if 'zero' in tl else bytes.fromhex(tl['hex'])
        out.append(('ROM', int(it['rom_start'], 16), t))
    return out


def enc_rest(game, A):
    DR = DRmod(); d = os.path.join(A, 'rest'); out = []
    for it in json.load(open(os.path.join(d, 'index.json'))):
        b, _ = DR.encode_item(it, d, game)
        out.append(('ROM' if it['where'] == 'rom' else it['where'], it['start'], b))
    return out


def enc_r1(game, A):
    for m in ('png', 'n64tex'): sys.modules.pop(m, None)
    sys.path.insert(0, os.path.join(OW, 'gfx', 'tools'))
    import n64tex, png
    sys.path.pop(0)
    d = os.path.join(A, 'r1', game); out = []
    for o in json.load(open(os.path.join(d, 'index.json'))):
        s, e = int(o['start'], 16), int(o['end'], 16)
        if o['kind'] == 'pad': b = bytes(e - s)
        elif o['kind'] == 'citex':
            t, tl = n64tex.encode('CI', 4 if '_CI4_' in o['file'] else 8, png.open_png(os.path.join(d, o['file'])))
            b = tl + t
        elif o['kind'] == 'vtx': b = b''.join(struct.pack('>hhhHhhBBBB', *r) for r in json.load(open(os.path.join(d, o['file']))))
        elif o['kind'] == 'bbox': j = json.load(open(os.path.join(d, o['file']))); b = struct.pack('>6f', *(j['min'] + j['max']))
        elif o['kind'] == 'mhdr': b = struct.pack('>10I', *[int(x, 16) for x in json.load(open(os.path.join(d, o['file'])))['words']])
        out.append(('ROM' if o['where'] == 'common' else o['where'], s, b))
    return out


def enc_lvlB(game, A):
    """lvlB's files: dl/dl_<addr>.txt (lvlB asm()), mh/mh_<addr>.json, vtx/vtx_<addr>.txt, tex/tex_<addr>_*.png
    (n64tex.encode: texels at addr, TLUT at the palette address MAP.tsv gives). Level addresses only."""
    M = load(script('lvlB', 'extract_level_gfx.py'), 'lvlB')
    n64tex, png = sys.modules['n64tex'], sys.modules['png']
    base = M.LEVEL_BASE; mem = {n: isz for s, e, n, isz in layout(game)[0]}
    rows = [l.rstrip('\n').split('\t') for l in open(os.path.join(A, 'lvlB', 'MAP.tsv'))]
    pal_of = {}; pal_len = {}
    for r in rows:
        f = r[1].split(':')[0]; a = int(r[1].split(':')[1], 16); b = int(r[2].split(':')[1], 16)
        if r[3] == 'texture' and r[5].startswith('palette '): pal_of[(f, r[7])] = int(r[5].split()[1], 16)
        if r[3] == 'palette': pal_len[(f, a)] = int(r[5].split()[0]) * 2
    out = []
    for lvl in sorted(os.listdir(os.path.join(A, 'lvlB'))):
        ld = os.path.join(A, 'lvlB', lvl)
        if not os.path.isdir(ld): continue
        sp = lvl + '.bif'; size = mem[sp]

        def put(addr, b):
            if base <= addr < base + size: out.append((sp, addr - base, b[:base + size - addr]))
        for p in sorted(glob.glob(os.path.join(ld, 'dl', 'dl_*.txt'))):
            lines = open(p).read().splitlines()
            put(int(os.path.basename(p)[3:11], 16), b''.join(M.asm(l) for l in lines if not l.endswith(':')))
        for p in sorted(glob.glob(os.path.join(ld, 'mh', 'mh_*.json'))):
            put(int(os.path.basename(p)[3:11], 16), struct.pack('>9I', *[int(w, 16) for w in json.load(open(p))]))
        for p in sorted(glob.glob(os.path.join(ld, 'vtx', 'vtx_*.txt'))):
            put(int(os.path.basename(p)[4:12], 16), b''.join(struct.pack('>hhhHhhBBBB', *[int(x) for x in l.split()]) for l in open(p) if l.strip()))
        for p in sorted(glob.glob(os.path.join(ld, 'tex', 'tex_*.png'))):
            fn = os.path.basename(p); parts = fn[:-4].split('_')
            fmt = parts[2].rstrip('0123456789'); bits = int(parts[2][len(fmt):])
            tb, tl = n64tex.encode(fmt, bits, png.open_png(p))
            put(int(parts[1], 16), tb)
            pa = pal_of.get((sp, 'tex/' + fn))
            if pa is not None and (sp, pa - base) in pal_len: put(pa, tl[:pal_len[(sp, pa - base)]])
    return out


def enc_lvlE(game, A):
    """lvlE: the item list is its MAP.part.tsv; each item re-packed from its JSON with lvlE's own pack functions
    (anim_pack, dup_pack, words_pack; pad/palette/hex forms as in its check)"""
    M = load(script('lvlE', 'extract_level_rest.py'), 'lvlE')
    out = []
    for l in open(os.path.join(A, 'lvlE', 'MAP.part.tsv')):
        c = l.rstrip('\n').split('\t')
        if len(c) < 4: continue
        name = c[1].split('.bif:')[0]; a = int(c[1].split(':')[1], 16); cls = c[3]
        j = json.load(open(os.path.join(A, 'lvlE', name, '%05X_%s.json' % (a, cls))))
        if cls == 'morph_anim': b = M.anim_pack(j)
        elif cls == 'vertex_dup_list': b = M.dup_pack(j)
        elif cls == 'pad': b = bytes.fromhex(j['hex']) if j['hex'] else bytes(j['len'])
        elif cls == 'tex_palette': b = struct.pack('>%dH' % len(j['rgba5551']), *j['rgba5551'])
        elif cls in ('tex_image', 'unknown', 'texload_dl'): b = bytes.fromhex(j['hex'])
        else: b = M.words_pack(j)
        out.append((name + '.bif', a, b))
    return out


def enc_lvlG(game, A):
    M = load(script('lvlG', 'extract_level_rest.py'), 'lvlG')
    out = []
    for l in list(open(os.path.join(A, 'lvlG', 'MAP.tsv')))[1:]:
        c = l.rstrip('\n').split('\t')
        lvl = c[1].split('.bif:')[0]; a = int(c[1].split(':')[1], 16); b = int(c[2].split(':')[1], 16)
        out.append((lvl + '.bif', a, M.read_item(os.path.join(A, 'lvlG'), lvl, (a, b, c[3]))))
    for sp, a, b, cls in lvlG_extra(game):          # after the tool's own rows: these win where they overlap
        out.append((sp, a, M.read_item(os.path.join(A, 'lvlG'), sp[:-4], (a, b, cls))))
    return out


def enc_lvlH(game, A):
    M = load(script('lvlH', 'extract_level_rest.py'), 'lvlH')
    out = []; root = os.path.join(A, 'lvlH', game)
    for lvl in sorted(os.listdir(root)):
        ip = os.path.join(root, lvl, 'items.json')
        if not os.path.exists(ip): continue
        for it in json.load(open(ip)):
            j = json.load(open(os.path.join(root, lvl, '%05X_%s.json' % (it['s'], it['cls']))))['data']
            out.append((lvl + '.bif', it['s'], M.encode(j, it['cls'])))
    return out


DIRECT = {'audio': enc_audio, 'speech': enc_speech, 'sprA': enc_sprA, 'boot': enc_boot, 'rsp': enc_rsp,
          'tex': enc_tex, 'rest': enc_rest, 'r1': enc_r1, 'lvlB': enc_lvlB, 'lvlE': enc_lvlE, 'lvlG': enc_lvlG,
          'lvlH': enc_lvlH}


def encode(game, A, P, owner):
    A = os.path.abspath(A); os.makedirs(P, exist_ok=True)
    seal(extra=[os.path.abspath(P)])
    if owner == 'adpcm': return          # adpcm's headers.json is an input of `audio`
    pcs = enc_sink(owner, game, A) if owner in SINK_OWNERS else DIRECT[owner](game, A)
    with open(os.path.join(P, owner + '.pcs.tmp'), 'wb') as f: pickle.dump(pcs, f)
    os.replace(os.path.join(P, owner + '.pcs.tmp'), os.path.join(P, owner + '.pcs'))
    print('encode %s: %d pieces, %d bytes' % (owner, len(pcs), sum(len(b) for _, _, b in pcs)))


def rspbin(game, A, out):
    """(pi) the RSP microcode for the main segment (data/rsp.s): assembled from ASSETS/rsp/GAME/*.s, no ROM access"""
    A = os.path.abspath(A); os.makedirs(out, exist_ok=True)
    seal(extra=[os.path.abspath(out)])
    R = load(script('g2', 'rsp_g2.py'), 'rsp_g2')
    for key, name, base, kind in R.BLOBS:
        b = R.assemble(open(os.path.join(A, 'rsp', game, name + '.s')).read())
        open(os.path.join(out, name + '.bin'), 'wb').write(b)
        print('rspbin %s: %d bytes' % (name, len(b)))


# ------------------------------------------------------------------ ROM layout
class Space:
    def __init__(s, n): s.b = bytearray(n); s.have = bytearray(n)
    def put(s, a, b):
        b = bytes(b)[:max(0, len(s.b) - a)]; s.b[a:a + len(b)] = b; s.have[a:a + len(b)] = b'\1' * len(b)


def rom(game, A, P, main, gz, outp):
    """MAIN = the linked main segment (code + .data + .rodata, ROM 0x1000 to the end of .rodata; data/ and src/)"""
    seal(extra=[os.path.dirname(os.path.abspath(main)), os.path.dirname(os.path.abspath(gz))])
    mem, seg, rows, left = layout(game)
    N = seg['rom'][1]; sizes = {n: isz for s, e, n, isz in mem}; sizes['ROM'] = N
    enc = {}

    def space(e, sp):
        if e not in enc:
            enc[e] = {}
            if e in ('code', 'data'): pcs = [('ROM', seg['code'][0], open(main, 'rb').read())]
            else: pcs = pickle.load(open(os.path.join(P, e + '.pcs'), 'rb'))
            for s_, a, b in pcs:
                if s_ not in enc[e]: enc[e][s_] = Space(sizes[s_])
                enc[e][s_].put(a, b)
        return enc[e].get(sp)
    if os.path.getsize(main) != seg['data'][1] - seg['code'][0]: sys.exit('main segment: %s has the wrong size' % main)
    imgs = {n: Space(isz) for n, isz in sizes.items()}
    GCC = b'GCC: (GNU) 2.7.2\0\0'
    gzip_rows = []
    for sp, s, e, owner, kind, en in rows:
        X = imgs[sp]
        if en == 'zero': X.put(s, bytes(e - s)); continue
        if en == 'comment': X.put(s, (GCC * ((e - s) // len(GCC) + 1))[:e - s]); continue
        if en == 'gzip': gzip_rows.append((s, e)); continue
        if en == 'leftover': continue
        Y = space(en, sp)
        if Y is None or Y.have[s:e].count(0):
            sys.exit('%s 0x%X-0x%X (%s %s): encoder %s does not cover it' % (sp, s, e, owner, kind, en))
        X.put(s, Y.b[s:e])
    # level images -> gzip members (GNU gzip 1.2.4 SMALL_MEM, -6, FNAME = image name, mtime 0, OS 0)
    for s, e, name, isz in mem:
        img = imgs[name]
        if img.have.count(0): sys.exit('%s: %d bytes not covered by the layout' % (name, img.have.count(0)))
        m = regzip(bytes(img.b), name, gz)
        if len(m) != e - s: sys.exit('%s: gzip member is %d bytes, layout says %d' % (name, len(m), e - s))
        imgs['ROM'].put(s, m)
        print('level', name, len(img.b), 'sha1', hashlib.sha1(img.b).hexdigest(), 'member', len(m))
    R = imgs['ROM']
    # stale tails: file padding written from an 8 KiB buffer = the bytes 0x2000 earlier (after everything else)
    for s, e in left: R.put(s, bytes(R.b[s - 0x2000:e - 0x2000]))
    if R.have.count(0): sys.exit('ROM: %d bytes not covered by the layout' % R.have.count(0))
    B = load(script('g2', 'boot_g2.py'), 'boot_g2')
    c1, c2 = B.crc6102(bytes(R.b)); R.put(0x10, struct.pack('>II', c1, c2))
    open(outp, 'wb').write(R.b)
    print('wrote', outp, len(R.b), 'bytes sha1', hashlib.sha1(R.b).hexdigest())


def regzip(raw, name, gz):
    """g1's regzip recipe, fed through stdin (no temporary file): the deflate stream does not depend on the name"""
    out = subprocess.run([gz, '-6', '-c'], input=raw, capture_output=True, check=True).stdout
    q = out.find(b'\0', 10) + 1 if out[3] & 8 else 10
    import zlib
    return (b'\x1f\x8b\x08\x08' + struct.pack('<I', 0) + b'\x00\x00' + name.encode() + b'\0' + out[q:-8] +
            struct.pack('<II', zlib.crc32(raw) & 0xFFFFFFFF, len(raw) & 0xFFFFFFFF))


if __name__ == '__main__':
    a = sys.argv[1:]
    sys.path.insert(0, T)
    if a[0] == 'extract': extract(a[1], a[2], a[3], a[4:] or None)
    elif a[0] == 'encode': encode(a[1], a[2], a[3], a[4])
    elif a[0] == 'rom': rom(*a[1:7])
    elif a[0] == 'rspbin': rspbin(a[1], a[2], a[3])
    elif a[0] == 'owners': print(' '.join(o for o in owners_of(a[1]) if o != 'adpcm'))
    else: sys.exit(__doc__)
