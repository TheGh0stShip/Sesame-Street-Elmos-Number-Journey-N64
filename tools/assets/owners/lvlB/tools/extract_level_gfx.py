#!/usr/bin/env python3
"""Graphics inside the gzip level files (.bif) of Elmo's Letter Adventure / Number Journey.

usage: extract_level_gfx.py ROM GAME OUTDIR          extract (PNG textures, F3DEX macro text, OBJ geometry, MAP.tsv rows)
       extract_level_gfx.py check ROM GAME OUTDIR    extract, then prove: PNG -> texel/TLUT bytes and
                                                     macro text -> display-list bytes equal the inflated file
GAME is letter or number. No ROM content is stored in this script: the file table comes from
../files/FILES_<game>.tsv (agent files) and everything else from the ROM.

Each .bif inflates to a prelinked RAM image loaded at 0x80154000 (func_8001D9D0 / Number func_8001E2CC,
gzip reader func_80013560 / func_800139D0); pointers are absolute. The shared `common` block is a raw
RAM image at 0x8024B000 and is used only to resolve references (it is not owned here).
"""
import os, re, sys, struct, zlib, json
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)  # snapshot of agent gfx's f3dex.py / n64tex.py / png.py (2026-11-19)
import f3dex, n64tex, png  # noqa: E402

LEVEL_BASE = 0x80154000
COMMON_BASE = 0x8024B000
FILES_DIR = os.path.join(HERE, '..', '..', 'files')   # (pa) was the lab's absolute sesassets/files path

# ---------------------------------------------------------------- invertible F3DEX macro text
# op -> (macro name, [(field, word, shift, width, scale)]).  Every argument is a raw bitfield (divided by
# scale); a command whose set bits are not all covered (or a field not divisible by scale) is written as
# gsDPWord(w0, w1), so the text always reassembles to the exact bytes.
A = ('addr', 1, 0, 32, 1)
OPS = {
    0x01: ('gsSPMatrix', [A, ('param', 0, 16, 8, 1), ('len', 0, 0, 16, 1)]),
    0x03: ('gsDma1p_G_MOVEMEM', [A, ('index', 0, 16, 8, 1), ('len', 0, 0, 16, 1)]),
    0x04: ('gsSPVertex', [A, ('n', 0, 10, 6, 1), ('v0', 0, 16, 8, 2), ('len_m1', 0, 0, 10, 1)]),
    0x06: ('gsSPDisplayList', [A, ('branch', 0, 16, 8, 1)]),
    0xB1: ('gsSP2Triangles', [('a0', 0, 16, 8, 2), ('b0', 0, 8, 8, 2), ('c0', 0, 0, 8, 2),
                              ('a1', 1, 16, 8, 2), ('b1', 1, 8, 8, 2), ('c1', 1, 0, 8, 2), ('flag', 1, 24, 8, 1)]),
    0xBF: ('gsSP1Triangle', [('a', 1, 16, 8, 2), ('b', 1, 8, 8, 2), ('c', 1, 0, 8, 2), ('flag', 1, 24, 8, 1)]),
    0xB6: ('gsSPClearGeometryMode', [('mode', 1, 0, 32, 1)]),
    0xB7: ('gsSPSetGeometryMode', [('mode', 1, 0, 32, 1)]),
    0xB8: ('gsSPEndDisplayList', []),
    0xB9: ('gsSPSetOtherMode_L', [('shift', 0, 8, 8, 1), ('len', 0, 0, 8, 1), ('data', 1, 0, 32, 1)]),
    0xBA: ('gsSPSetOtherMode_H', [('shift', 0, 8, 8, 1), ('len', 0, 0, 8, 1), ('data', 1, 0, 32, 1)]),
    0xBB: ('gsSPTexture', [('s', 1, 16, 16, 1), ('t', 1, 0, 16, 1), ('level', 0, 11, 3, 1), ('tile', 0, 8, 3, 1), ('on', 0, 0, 8, 1)]),
    0xBC: ('gsMoveWd', [('index', 0, 0, 8, 1), ('offset', 0, 8, 16, 1), ('data', 1, 0, 32, 1)]),
    0xBD: ('gsSPPopMatrix', [('which', 1, 0, 32, 1)]),
    0xBE: ('gsSPCullDisplayList', [('v0x40', 0, 0, 24, 1), ('vnx40', 1, 0, 32, 1)]),
    0xB3: ('gsImmp1_RDPHALF_2', [('data', 1, 0, 32, 1)]),
    0xB4: ('gsImmp1_RDPHALF_1', [('data', 1, 0, 32, 1)]),
    0xC0: ('gsSPNoOp', []),
    0xE4: ('gsSPTextureRectangle', [('lrx', 0, 12, 12, 1), ('lry', 0, 0, 12, 1), ('tile', 1, 24, 3, 1), ('ulx', 1, 12, 12, 1), ('uly', 1, 0, 12, 1)]),
    0xE6: ('gsDPLoadSync', []), 0xE7: ('gsDPPipeSync', []), 0xE8: ('gsDPTileSync', []), 0xE9: ('gsDPFullSync', []),
    0xED: ('gsDPSetScissor', [('ulx', 0, 12, 12, 1), ('uly', 0, 0, 12, 1), ('mode', 1, 24, 8, 1), ('lrx', 1, 12, 12, 1), ('lry', 1, 0, 12, 1)]),
    0xEF: ('gsDPSetOtherMode', [('hi', 0, 0, 24, 1), ('lo', 1, 0, 32, 1)]),
    0xF0: ('gsDPLoadTLUTCmd', [('tile', 1, 24, 3, 1), ('count_x4', 1, 12, 12, 1)]),
    0xF2: ('gsDPSetTileSize', [('uls', 0, 12, 12, 1), ('ult', 0, 0, 12, 1), ('tile', 1, 24, 3, 1), ('lrs', 1, 12, 12, 1), ('lrt', 1, 0, 12, 1)]),
    0xF3: ('gsDPLoadBlock', [('uls', 0, 12, 12, 1), ('ult', 0, 0, 12, 1), ('tile', 1, 24, 3, 1), ('lrs', 1, 12, 12, 1), ('dxt', 1, 0, 12, 1)]),
    0xF4: ('gsDPLoadTile', [('uls', 0, 12, 12, 1), ('ult', 0, 0, 12, 1), ('tile', 1, 24, 3, 1), ('lrs', 1, 12, 12, 1), ('lrt', 1, 0, 12, 1)]),
    0xF5: ('gsDPSetTile', [('fmt', 0, 21, 3, 1), ('siz', 0, 19, 2, 1), ('line', 0, 9, 9, 1), ('tmem', 0, 0, 9, 1), ('tile', 1, 24, 3, 1),
                           ('pal', 1, 20, 4, 1), ('cmt', 1, 18, 2, 1), ('maskt', 1, 14, 4, 1), ('shiftt', 1, 10, 4, 1),
                           ('cms', 1, 8, 2, 1), ('masks', 1, 4, 4, 1), ('shifts', 1, 0, 4, 1)]),
    0xF6: ('gsDPFillRectangle', [('lrx', 0, 12, 12, 1), ('lry', 0, 0, 12, 1), ('ulx', 1, 12, 12, 1), ('uly', 1, 0, 12, 1)]),
    0xF7: ('gsDPSetFillColor', [('color', 1, 0, 32, 1)]),
    0xF8: ('gsDPSetFogColor', [('rgba', 1, 0, 32, 1)]),
    0xF9: ('gsDPSetBlendColor', [('rgba', 1, 0, 32, 1)]),
    0xFA: ('gsDPSetPrimColor', [('m', 0, 8, 8, 1), ('l', 0, 0, 8, 1), ('rgba', 1, 0, 32, 1)]),
    0xFB: ('gsDPSetEnvColor', [('rgba', 1, 0, 32, 1)]),
    0xFC: ('gsDPSetCombine', [('muxs0', 0, 0, 24, 1), ('muxs1', 1, 0, 32, 1)]),
    0xFD: ('gsDPSetTextureImage', [('fmt', 0, 21, 3, 1), ('siz', 0, 19, 2, 1), ('width_m1', 0, 0, 12, 1), A]),
    0xFE: ('gsDPSetDepthImage', [A]),
    0xFF: ('gsDPSetColorImage', [('fmt', 0, 21, 3, 1), ('siz', 0, 19, 2, 1), ('width_m1', 0, 0, 12, 1), A]),
}
BYNAME = {v[0]: (k, v[1]) for k, v in OPS.items()}


def dis(w0, w1):
    op = w0 >> 24
    if op in OPS:
        name, fl = OPS[op]
        ws = [w0 & 0xFFFFFF, w1]; cov = [0, 0]; args = []
        ok = True
        for (_, wi, sh, wd, sc) in fl:
            m = ((1 << wd) - 1) << sh
            v = (ws[wi] & m) >> sh
            cov[wi] |= m
            if v % sc:
                ok = False
            args.append(v // sc)
        if ok and (ws[0] & ~cov[0]) == 0 and (ws[1] & ~cov[1]) == 0:
            return '%s(%s)' % (name, ', '.join(('0x%08X' % a) if f[3] == 32 else str(a) for a, f in zip(args, fl)))
    return 'gsDPWord(0x%08X, 0x%08X)' % (w0, w1)


def asm(line):
    m = re.match(r'\s*(\w+)\((.*)\)', line)
    name, a = m.group(1), [int(x, 0) for x in m.group(2).split(',') if x.strip()]
    if name == 'gsDPWord':
        return struct.pack('>II', a[0], a[1])
    op, fl = BYNAME[name]
    ws = [op << 24, 0]
    for v, (_, wi, sh, wd, sc) in zip(a, fl):
        ws[wi] |= ((v * sc) & ((1 << wd) - 1)) << sh
    return struct.pack('>II', *ws)


# ---------------------------------------------------------------- inputs
def file_table(game):
    rows = []
    for l in open(os.path.join(FILES_DIR, 'FILES_%s.tsv' % game)):
        f = l.rstrip('\n').split('\t')
        if f[0] == 'id':
            continue
        rows.append(f)
    return rows


LEVEL_REFS = set()


def load_refs(game):
    p = os.path.join(os.path.dirname(FILES_DIR), 'LEVEL_REFS_%s.tsv' % game)
    if os.path.exists(p):
        for l in open(p):
            if l.startswith('0x'):
                LEVEL_REFS.add(int(l.split('\t')[0], 16))


def load(rom, game):
    load_refs(game)
    d = open(rom, 'rb').read()
    rows = file_table(game)
    levels = []
    common = None
    prog_end = int(rows[0][1], 16)
    for f in rows:
        s, e = int(f[1], 16), int(f[2], 16)
        if f[4] == 'gzip_level':
            o = zlib.decompressobj(31)
            levels.append((f[5].split(':')[0].strip(), s, o.decompress(d[s:e])))
        elif 'common' in f[5][:8]:
            common = d[s:e]
    return d, levels, common, d[0x1000:prog_end]


# ---------------------------------------------------------------- analysis of one level
def analyse(name, data, common, prog):
    n = len(data)
    lend = LEVEL_BASE + n

    def resolve(a, ln):
        if LEVEL_BASE <= a and a + ln <= lend:
            return data[a - LEVEL_BASE:a - LEVEL_BASE + ln]
        if common and COMMON_BASE <= a and a + ln <= COMMON_BASE + len(common):
            return common[a - COMMON_BASE:a - COMMON_BASE + ln]
        return None

    def dl_end(a):
        """a valid DL start: every command plausible up to gsSPEndDisplayList (or a branch); returns end addr."""
        pc = a
        for _ in range(4096):
            r = resolve(pc, 8)
            if r is None:
                return None
            w0, w1 = struct.unpack('>II', r)
            if not f3dex.plausible(w0, w1):
                return None
            if w0 >> 24 == 0xB8 or (w0 >> 24 == 0x06 and (w0 >> 16) & 0xFF):
                return pc + 8
            pc += 8
        return None

    # roots: 8-aligned words pointing into the level, from the level itself and the program image
    cand = set()
    for blob in (data, prog):
        for i in range(0, len(blob) - 3, 4):
            w = struct.unpack_from('>I', blob, i)[0]
            if LEVEL_BASE <= w < lend and w % 8 == 0:
                cand.add(w)
    # plus the addresses the code builds (LEVEL_REFS_<game>.tsv, computed from the ROM by the coordinator)
    cand |= {r for r in LEVEL_REFS if LEVEL_BASE <= r < lend and r % 8 == 0}
    roots = {}
    for a in sorted(cand):
        e = dl_end(a)
        if e is None:
            continue
        # require at least one geometry/texture command so that zero-words and stray data are not taken
        ops = {resolve(p, 1)[0] for p in range(a, e, 8)}
        if ops & {0x04, 0x06, 0xFD, 0xB1, 0xBF}:
            roots[a] = e
    # follow gsSPDisplayList targets
    todo = list(roots)
    while todo:
        a = todo.pop()
        for p in range(a, roots[a], 8):
            w0, w1 = struct.unpack('>II', resolve(p, 8))
            if w0 >> 24 == 0x06 and LEVEL_BASE <= w1 < lend and w1 not in roots:
                e = dl_end(w1)
                if e:
                    roots[w1] = e; todo.append(w1)
    called = set()
    for a, e in roots.items():
        for p in range(a, e, 8):
            w0, w1 = struct.unpack('>II', resolve(p, 8))
            if w0 >> 24 == 0x06:
                called.add(w1)
    # merge DL ranges into blocks
    blocks = []
    for a in sorted(roots):
        e = roots[a]
        if blocks and a < blocks[-1][1]:
            blocks[-1][1] = max(blocks[-1][1], e); blocks[-1][2].append(a)
        else:
            blocks.append([a, e, [a]])

    # model records (read by func_80024C1C / func_8002BADC / func_80029D48 / func_80034B64 / func_8003E220 ...):
    # +0x08 display list, +0x0C segment 3 base (vertices), +0x1C segment 4 base (textures, TLUTs);
    # the code emits gsSPSegment(3), gsSPSegment(4) (0xBC000C06 / 0xBC001006), the matrix and gsSPDisplayList.
    def inrng(w):
        return LEVEL_BASE <= w < lend or (common and COMMON_BASE <= w < COMMON_BASE + len(common))
    models = {}
    for i in range(0, n - 0x20, 4):
        dl, v3, x4 = struct.unpack_from('>I', data, i + 8)[0], struct.unpack_from('>I', data, i + 12)[0], struct.unpack_from('>I', data, i + 0x1C)[0]
        if dl in roots and inrng(v3) and (x4 == 0 or inrng(x4)):
            models[LEVEL_BASE + i] = (dl, v3, x4)
    segs_for = {}
    for m, (dl, v3, x4) in models.items():
        segs_for.setdefault(dl, []).append({3: v3, 4: x4} if x4 else {3: v3})
    # trace every DL: textures, palettes, vertices, triangles
    textures, tluts, vtxs = {}, {}, {}
    geom = {}
    runs = [(a, None) for a in sorted(roots) if a not in segs_for] + [(a, sg) for a in sorted(segs_for) for sg in segs_for[a]]
    for a, sg in runs:
        t = f3dex.Tracer(resolve, sg)
        t.run(a)
        for (va, cnt) in t.vtx:
            vtxs[va] = max(vtxs.get(va, 0), cnt)
        for tx in t.textures:
            if tx.get('row') is not None:
                # LoadTile of a sub-rectangle: record the whole rows touched
                key = tx['addr']
                rec = dict(fmt=tx['fmt'], bits=tx['bits'], w=tx['row'], h=tx['y'] + tx['h'])
            else:
                key = tx['addr']
                rec = dict(fmt=tx['fmt'], bits=tx['bits'], w=tx['w'], h=tx['h'])
            if 'tlut' in tx:
                rec['tlut'] = tx['tlut'][0]
                tl = tx['tlut']
                tluts[tl[0]] = max(tluts.get(tl[0], 0), tl[1])
            old = textures.get(key)
            if old is None or old['w'] * old['h'] * old['bits'] < rec['w'] * rec['h'] * rec['bits']:
                textures[key] = rec
        if a not in called and (a, sg is None) not in geom:
            geom[(a, sg is None)] = (t.tris, t.textures)
    return dict(models=models, resolve=resolve, roots=roots, blocks=blocks, textures=textures, tluts=tluts, vtxs=vtxs, geom=geom, n=n)


def tex_bytes(rec):
    return rec['w'] * rec['h'] * rec['bits'] // 8


# ---------------------------------------------------------------- writers
def extract(rom, game, outd, check=False):
    d, levels, common, prog = load(rom, game)
    os.makedirs(outd, exist_ok=True)
    maprows, report, bad = [], [], []
    for (fname, romstart, data) in levels:
        A = analyse(fname, data, common, prog)
        res = A['resolve']
        ld = os.path.join(outd, fname.replace('.bif', ''))
        for sub in ('tex', 'dl'):
            os.makedirs(os.path.join(ld, sub), exist_ok=True)
        objs = []  # (start, end, class, format, desc, ref, extractor)
        ins = lambda a: LEVEL_BASE <= a < LEVEL_BASE + len(data)
        # display lists
        dlcount = 0; words = 0
        for (s, e, ents) in A['blocks']:
            lines = []
            for p in range(s, e, 8):
                if p in A['roots']:
                    lines.append('dl_%08X:' % p)
                w0, w1 = struct.unpack('>II', res(p, 8))
                t = dis(w0, w1)
                words += t.startswith('gsDPWord')
                lines.append('    ' + t)
            if not check:
                open(os.path.join(ld, 'dl', 'dl_%08X.txt' % s), 'w').write('\n'.join(lines) + '\n')
            dlcount += len(ents)
            if check:  # (g3) reassemble from the macro text file written by extract, not from the in-memory lines
                lines = open(os.path.join(ld, 'dl', 'dl_%08X.txt' % s)).read().splitlines()
                b = b''.join(asm(l) for l in lines if not l.endswith(':'))
                if b != res(s, e - s):
                    bad.append('%s dl %08X' % (fname, s))
            objs.append((s, e, 'display_list', 'F3DEX 1.23 GBI commands, 8 bytes each',
                         '%d entry point(s) %s' % (len(ents), ' '.join('%08X' % x for x in ents[:4]) + (' ...' if len(ents) > 4 else '')),
                         'absolute RAM pointer (level data / program tables / gsSPDisplayList)', 'dl/dl_%08X.txt' % s))
        # model records
        for ma in sorted(A['models']):
            objs.append((ma, ma + 0x24, 'model_header', 'u32 flags; u16 nvtx; u16; u32 dl; u32 seg3 vtx base; u32; u32; u32; u32 seg4 tex base; u32 tex size',
                         'dl %08X seg3 %08X seg4 %08X' % A['models'][ma], 'object/level tables, read by func_80024C1C family', 'MAP only'))
        # vertices
        for va, cnt in sorted(A['vtxs'].items()):
            if ins(va):
                objs.append((va, va + 16 * cnt, 'vertices', 'Vtx: s16 x,y,z,flag; s16 s,t; u8 r,g,b,a (16 bytes)',
                             '%d vertices' % cnt, 'gsSPVertex', 'level.obj'))
        # model records as JSON word lists (added by agent g3; pointers are kept as RAM addresses)
        os.makedirs(os.path.join(ld, 'mh'), exist_ok=True)
        for ma in sorted(A['models']):
            raw = res(ma, 0x24)
            if raw is None:
                continue
            mp_ = os.path.join(ld, 'mh', 'mh_%08X.json' % ma)
            if not check:
                json.dump(['0x%08X' % w for w in struct.unpack('>9I', raw)], open(mp_, 'w'))
            elif struct.pack('>9I', *[int(w, 16) for w in json.load(open(mp_))]) != raw:
                bad.append('%s model %08X' % (fname, ma))
        # vertex arrays, lossless text form (added by agent g3): one line per Vtx "x y z flag s t r g b a"
        os.makedirs(os.path.join(ld, 'vtx'), exist_ok=True)
        for va, cnt in sorted(A['vtxs'].items()):
            raw = res(va, 16 * cnt) if ins(va) else None
            if raw is None:
                continue
            vp = os.path.join(ld, 'vtx', 'vtx_%08X.txt' % va)
            if not check:
                open(vp, 'w').write(''.join('%d %d %d %d %d %d %d %d %d %d\n' % struct.unpack('>hhhHhhBBBB', raw[i:i + 16])
                                            for i in range(0, len(raw), 16)))
            else:
                vb = b''.join(struct.pack('>hhhHhhBBBB', *[int(x) for x in l.split()]) for l in open(vp) if l.strip())
                if vb != raw:
                    bad.append('%s vtx %08X' % (fname, va))
        # palettes
        for ta, cnt in sorted(A['tluts'].items()):
            if ins(ta):
                objs.append((ta, ta + 2 * cnt, 'palette', 'TLUT RGBA5551 big-endian', '%d colours' % cnt,
                             'gsDPSetTextureImage + gsDPLoadTLUTCmd', 'embedded in CI PNG'))
        # textures
        ntex = 0
        for ta, rec in sorted(A['textures'].items()):
            if not ins(ta) or rec['fmt'] not in ('RGBA', 'CI', 'IA', 'I'):
                continue
            nb = tex_bytes(rec)
            raw = res(ta, nb)
            if raw is None:
                continue
            pal = None
            if rec['fmt'] == 'CI':
                pn = 16 if rec['bits'] == 4 else 256
                pal = res(rec['tlut'], 2 * pn) if rec.get('tlut') else None
            im = n64tex.decode(rec['fmt'], rec['bits'], rec['w'], rec['h'], raw, pal)
            pn = 'tex_%08X_%s%d_%dx%d.png' % (ta, rec['fmt'], rec['bits'], rec['w'], rec['h'])
            if not check:  # (g3) check reads back the PNG written by extract
                im.save(os.path.join(ld, 'tex', pn))
            ntex += 1
            if check:
                im2 = png.open_png(os.path.join(ld, 'tex', pn))
                tb, tl = n64tex.encode(rec['fmt'], rec['bits'], im2)
                if tb != raw:
                    bad.append('%s tex %08X' % (fname, ta))
                if pal and tl[:len(pal)] != pal:
                    bad.append('%s tlut %08X' % (fname, rec['tlut']))
            objs.append((ta, ta + nb, 'texture', '%s%d %dx%d' % (rec['fmt'], rec['bits'], rec['w'], rec['h']),
                         'palette %08X' % rec['tlut'] if rec.get('tlut') else '', 'gsDPSetTextureImage', 'tex/' + pn))
        # geometry OBJ
        write_obj(ld, A, res)
        # tile the file: priority dl > vertices > palette > texture; gaps are level data or padding
        prio = {'display_list': 0, 'model_header': 1, 'vertices': 2, 'palette': 3, 'texture': 4}
        objs.sort(key=lambda o: (o[0], prio[o[2]]))
        cover = []
        pos = LEVEL_BASE
        overl = 0
        for o in sorted(objs, key=lambda o: (prio[o[2]], o[0])):
            pass
        # simple sweep: keep objects in address order, clip overlaps (lower priority loses its overlapped head)
        taken = bytearray(len(data))
        final = []
        for o in sorted(objs, key=lambda o: (prio[o[2]], o[0])):
            s, e = o[0] - LEVEL_BASE, min(o[1], LEVEL_BASE + len(data)) - LEVEL_BASE
            if any(taken[s:e]):
                overl += 1
                if all(taken[s:e]):
                    continue
                # keep the untaken sub-runs
                i = s
                while i < e:
                    if taken[i]:
                        i += 1; continue
                    j = i
                    while j < e and not taken[j]:
                        j += 1
                    final.append((i, j) + o[2:6] + ('%s (overlaps another object)' % o[6],))
                    taken[i:j] = b'\1' * (j - i); i = j
                continue
            taken[s:e] = b'\1' * (e - s)
            final.append((s, e) + o[2:])
        final.sort()
        rows = []
        p = 0
        comment = data.find(b'\0GCC: (GNU)')
        def gap(a, b):
            out = []
            while a < b:
                if comment >= 0 and a <= comment + 1 < b and comment + 1 > a:
                    out += gap_one(a, comment + 1); a = comment + 1; continue
                out += gap_one(a, b); break
            return out
        def gap_one(a, b):
            if comment >= 0 and a >= comment + 1:
                return [(a, b, 'comment', 'ELF .comment strings "GCC: (GNU) 2.7.2" (linker leftover, never read)', '', 'none', '')]
            if not any(data[a:b]):
                return [(a, b, 'pad', 'zero', '', 'none', '')]
            return [(a, b, 'level_data', 'non-graphics level structures (owned by agent lvlA)', '', 'see lvlA', '')]
        for o in final:
            if o[0] > p:
                rows += gap(p, o[0])
            rows.append(o); p = o[1]
        if p < len(data):
            rows += gap(p, len(data))
        counts = {}
        for r in rows:
            c = counts.setdefault(r[2], [0, 0]); c[0] += 1; c[1] += r[1] - r[0]
            maprows.append('%s\t%s:0x%05X\t%s:0x%05X\t%s\t%s\t%s\t%s\t%s' % (game, fname, r[0], fname, r[1], r[2], r[3], r[4], r[5], r[6]))
        report.append(dict(level=fname, rom=hex(romstart), size=len(data), dl_entries=dlcount, dl_blocks=len(A['blocks']),
                           dpword_fallbacks=words, png=ntex, overlaps_clipped=overl,
                           bytes={k: v[1] for k, v in counts.items()}, objects={k: v[0] for k, v in counts.items()},
                           common_textures=sum(1 for a in A['textures'] if not ins(a))))
    open(os.path.join(outd, 'MAP.tsv'), 'w').write('\n'.join(maprows) + '\n')
    json.dump(report, open(os.path.join(outd, 'report.json'), 'w'), indent=1)
    for r in report:
        print(json.dumps(r))
    if check:
        print('CHECK', 'OK' if not bad else 'FAIL %d' % len(bad), bad[:20])
        return 0 if not bad else 1
    return 0


def write_obj(ld, A, res):
    out = ['# level geometry: every top-level display list, vertices in model space (matrices not applied)',
           'mtllib level.mtl']
    mtl = []
    vi = 1
    seen_mat = set()
    for (a, _), (tris, texs) in sorted(A['geom'].items()):
        if not tris:
            continue
        out.append('g dl_%08X' % a)
        cur = None
        for (vs, ti) in tris:
            if None in vs:
                continue
            tex = texs[ti] if ti is not None and ti < len(texs) else None
            mat = 'tex_%08X' % tex['addr'] if tex else 'untextured'
            if mat != cur:
                out.append('usemtl ' + mat); cur = mat
                if mat not in seen_mat:
                    seen_mat.add(mat)
                    mtl.append('newmtl %s\nKd 1 1 1' % mat)
                    if tex and tex['addr'] in A['textures']:
                        r = A['textures'][tex['addr']]
                        mtl.append('map_Kd tex/tex_%08X_%s%d_%dx%d.png' % (tex['addr'], r['fmt'], r['bits'], r['w'], r['h']))
            tw = tex['w'] if tex else 32; th = tex['h'] if tex else 32
            for (va, i) in vs:
                b = res(va + 16 * i, 16)
                if b is None:
                    b = bytes(16)
                x, y, z, _, s, t = struct.unpack('>hhhHhh', b[:12])
                out.append('v %d %d %d %.4f %.4f %.4f' % (x, y, z, b[12] / 255, b[13] / 255, b[14] / 255))
                out.append('vt %.5f %.5f' % (s / 32.0 / tw, 1 - t / 32.0 / th))
            out.append('f %d/%d %d/%d %d/%d' % (vi, vi, vi + 1, vi + 1, vi + 2, vi + 2))
            vi += 3
    open(os.path.join(ld, 'level.obj'), 'w').write('\n'.join(out) + '\n')
    open(os.path.join(ld, 'level.mtl'), 'w').write('\n'.join(mtl) + '\n')


if __name__ == '__main__':
    a = sys.argv[1:]
    if a and a[0] == 'check':
        sys.exit(extract(a[1], a[2], a[3], check=True))
    sys.exit(extract(a[0], a[1], a[2]))
