#!/usr/bin/env python3
"""Classify and extract the level-image bytes that neither lvlB (graphics) nor lvlC (objects/BSP) cover.

usage: extract_level_rest.py ROM GAME OUTDIR [check|map]
  GAME   number (maze ski river bonus123) or letter (garden space planet bonusabc; untested here)
  extract (default): OUTDIR/GAME/<level>/... JSON per structure, PNG per texture, items.json
  check: re-serialise every item from its decoded JSON form and compare with the original bytes
  map:   print MAP.tsv rows (territory tiling) to stdout
The .bif files are inflated from the ROM (offsets from ../../files/FILES_<game>.tsv).
All structures are ready-made RAM images at 0x80154000 (file offset = address - 0x80154000).
"""
import sys, os, struct, zlib, json, gzip, io

HERE = os.path.dirname(os.path.abspath(__file__))
SES = os.path.normpath(os.path.join(HERE, '..', '..'))
BASE = 0x80154000
LEVELS = {'number': ['maze', 'ski', 'river', 'bonus123'], 'letter': ['garden', 'space', 'planet', 'bonusabc']}
# code-built animated texture frame tables: (game, level) -> [(table address, entries, function)]
TEXANIM = {('number', 'maze'): [(0x8019F170, 8, 'func_80025DF0')],
           ('number', 'river'): [(0x801B3C90, 13, 'func_80025AD0')]}

FORMATS = {
    'anim_header': 'u16 hdr_end, u16 data_off, u8 ticks, u8 nkey, u16 flags, u8 keys[nkey], zero pad',
    'anim_vertices': 'u32[nvert][nkey] packed z:10 (bits 31..22) y:11 (21..11) x:11 (10..0), signed, vertex-major',
    'anim_tail': 'stale words after an animation, < one vertex row',
    'vertex_order': 'u16 runs {n, m, m x u16 index}, ends at n=0 or m=0',
    'bounds': 'f32[6] big-endian',
    'model_header_tail': 'u32 pointer (model header +0x24)',
    'model_header': '0x28-byte model header (lvlB layout)',
    'texanim_table': 'u32 pointers to 0x220-byte frames',
    'palette': 'TLUT RGBA5551 big-endian',
    'texture': 'N64 texels (format in description)',
    'vertices': 'F3DEX Vtx, 16 bytes each',
    'display_list': 'F3DEX 1.23 GBI commands, 8 bytes each',
    'coll_quad': '0x50: u32 type=3, u32 0, u32 mask, u32 0, f32 nx,ny,nz,d, f32[4][3] corners',
    'coll_quad_set': 'u32 ptr[n], u32 n, u32 pointer to ptr[0]',
    'texture_block_unused': 'raw bytes',
    'pad': 'zero bytes',
    'unknown': 'raw bytes',
}

MODEL_FUNCS = {'func_80023A38', 'func_8003D3D0', 'func_800408D0', 'func_8002C800'}

def u16(d, o): return struct.unpack_from('>H', d, o)[0]
def u32(d, o): return struct.unpack_from('>I', d, o)[0]

# ---------------------------------------------------------------- input
def load_bifs(rom, game):
    r = open(rom, 'rb').read()
    out = {}
    for l in open(os.path.join(SES, 'files', f'FILES_{game}.tsv')):
        p = l.rstrip('\n').split('\t')
        if len(p) < 6 or p[4] != 'gzip_level': continue
        name = p[5].split('.bif')[0]
        if name in LEVELS[game]:
            out[name] = zlib.decompressobj(31).decompress(r[int(p[1], 16):int(p[2], 16)])  # first member only
    return out

def covered(game):
    cov = {}
    models = {}
    for path, skip in ((os.path.join(SES, 'lvlB', 'MAP.tsv'), {'level_data'}), (os.path.join(SES, 'lvlC', 'MAP.tsv'), set())):
        for l in open(path):
            p = l.rstrip('\n').split('\t')
            if len(p) < 4 or p[0] != game or p[3] in skip: continue
            f, s = p[1].split(':'); e = int(p[2].split(':')[1], 16); s = int(s, 16)
            f = f.replace('.bif', '')
            cov.setdefault(f, []).append((s, e))
            if p[3] in ('model_header', 'bsp_leaf', 'display_list'): models.setdefault(f, []).append((p[3], s))
    return cov, models

# ---------------------------------------------------------------- recognisers
def anim_hdr(d, o):
    if o + 8 > len(d): return None
    he, do, ticks, nk, fl = u16(d, o), u16(d, o + 2), d[o + 4], d[o + 5], u16(d, o + 6)
    hi = he & 0xFF00; he &= 0xFF   # high byte of word 0 is 0 except in one river anim (0x20); never read
    if hi not in (0, 0x2000): return None
    if nk < 2 or he != 8 + nk or do != (he & ~7) + 8 or fl & 0x7FFF: return None
    if o + do > len(d): return None
    keys = list(d[o + 8:o + 8 + nk])
    if keys[0] != 0 or any(keys[i] >= keys[i + 1] for i in range(nk - 1)) or keys[-1] > ticks: return None
    if any(d[o + he:o + do]): return None
    return dict(ticks=ticks, nkey=nk, flags=fl, keys=keys, data_off=do, hi=hi)

GBI_OK = {0x01, 0x03, 0x04, 0x06, 0xB1, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xE4, 0xE6, 0xE7,
          0xE8, 0xE9, 0xEE, 0xEF, 0xF0, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0x05, 0xBF}

def parse_dl(d, o, lim):
    """F3DEX command run from o ending with gsSPEndDisplayList (0xB8) before lim; returns end or None."""
    p = o; n = 0
    while p + 8 <= lim:
        op = d[p]
        if op not in GBI_OK: return None
        p += 8; n += 1
        if op == 0xB8: return p if n >= 2 else None
    return None

def dl_texrefs(d, s, e):
    """(address, bytes, kind, fmt info) for every texture/TLUT load in a DL."""
    out = []; img = None
    for p in range(s, e, 8):
        w0, w1 = u32(d, p), u32(d, p + 4)
        op = w0 >> 24
        if op == 0xFD: img = (w1, (w0 >> 21) & 7, (w0 >> 19) & 3)
        elif op == 0xF3 and img:
            n = ((w1 >> 12) & 0xFFF) + 1
            out.append(('block', img[0], n * (1 << img[2]) // 2, img))
        elif op == 0xF0 and img:
            n = ((w1 >> 14) & 0x3FF) + 1
            out.append(('tlut', img[0], n * 2, img))
    return out

def order_list(d, o, lim):
    """u16 vertex-order list: {n, n sequential} {m, m u16 indices} ... 0. Returns (end, unique, total)."""
    p = o; uniq = 0; tot = 0
    while p + 2 <= lim:
        n = u16(d, p); p += 2
        if n == 0: return p, uniq, tot
        uniq += n; tot += n
        if p + 2 > lim: return None
        m = u16(d, p); p += 2
        if m == 0: return p, uniq, tot
        p += 2 * m; tot += m
    return None

# ---------------------------------------------------------------- classifier
class Level:
    def __init__(s, game, name, d, cov, models):
        s.game, s.name, s.d = game, name, d
        s.own = bytearray(len(d))          # 1 = lvlB/lvlC territory
        for a, b in cov: s.own[a:b] = b'\1' * (b - a)
        s.models = sorted(o for c, o in models if c == 'model_header')
        s.leaves = sorted(o for c, o in models if c == 'bsp_leaf')
        s.dlstarts = set(o for c, o in models if c == 'display_list')
        s.items = []                       # dicts with s, e, cls, desc, ref
        s.mine = bytearray(len(d))         # 1 = classified here
        s.occ = bytearray(s.own); s.walked = set()

    def free(s, a, b): return 0 <= a < b <= len(s.d) and s.occ.find(1, a, b) == -1

    def nxt(s, o):
        e = s.occ.find(1, o); return len(s.d) if e < 0 else e

    def add(s, a, b, cls, desc, ref, **kw):
        if not s.free(a, b): return False
        s.mine[a:b] = b'\1' * (b - a); s.occ[a:b] = b'\1' * (b - a)
        it = dict(s=a, e=b, cls=cls, desc=desc, ref=ref); it.update(kw); s.items.append(it); return True

    def add_clip(s, a, b, cls, desc, ref, **kw):
        o = a
        while o < b:
            if not s.free(o, o + 1): o += 1; continue
            e = min(s.nxt(o), b)
            part = '' if (o, e) == (a, b) else f' [bytes 0x{o - a:X}-0x{e - a:X} of 0x{b - a:X}; rest covered by lvlB/lvlC]'
            s.add(o, e, cls, desc + part, ref, **kw)
            o = e

    def res(s, w, seg):
        if w >> 24 in seg and seg[w >> 24] is not None: return s.off(seg[w >> 24] + (w & 0xFFFFFF))
        return s.off(w)

    def do_dl(s, o, why, depth=0, seg=None, scan=False):
        """Walk a display list (any owner); classify the free bytes of it and of what it loads."""
        d = s.d; L = len(d); seg = seg or {}
        key = (o, tuple(sorted(seg.items())))
        if depth > 8 or key in s.walked: return
        if scan:   # free-space scan: must end with B8 inside free space, or run into a lvlB list
            lim = s.nxt(o); e = parse_dl(d, o, lim)
            if e is None:
                if lim in s.dlstarts and (lim - o) % 8 == 0 and all(d[p] in GBI_OK for p in range(o, lim, 8)): e = parse_dl(d, o, L)
                if e is None: return
        else:
            e = parse_dl(d, o, L)
            if e is None: return
        s.walked.add(key)
        s.add_clip(o, e, 'display_list', 'F3DEX display list' + (' not reached from any drawn list' if scan else ''), why)
        tag = f'DL 0x{o + BASE:08X}'
        img = None; tlut = None; pend = []
        cmds = [(u32(d, p), u32(d, p + 4)) for p in range(o, e, 8)]
        for w0, w1 in cmds:
            op = w0 >> 24
            if op == 0xFD: img = (w1, (w0 >> 21) & 7, (w0 >> 19) & 3)
            elif op == 0xF0 and img:
                t = s.res(img[0], seg); n = ((w1 >> 14) & 0x3FF) + 1
                if t is not None:
                    tlut = (t, n); s.add_clip(t, t + 2 * n, 'palette', f'TLUT RGBA5551, {n} colours', tag)
            elif op == 0xF3 and img:
                t = s.res(img[0], seg); n = ((w1 >> 12) & 0xFFF) + 1
                if t is not None: pend.append([t, n * (1 << img[2]) // 2, None, None, None, tlut])
            elif op == 0xF5 and (w1 >> 24) & 7 == 0:
                for pt in pend:
                    if pt[2] is None: pt[2] = ((w0 >> 21) & 7, (w0 >> 19) & 3)
            elif op == 0xF2:
                for pt in pend:
                    if pt[3] is None: pt[3] = ((((w1 >> 12) & 0xFFF) >> 2) + 1, ((w1 & 0xFFF) >> 2) + 1)
            elif op == 0x04:
                t = s.res(w1, seg); n = (w0 >> 10) & 0x3F
                if t is not None: s.add_clip(t, t + 16 * n, 'vertices', f'F3DEX Vtx[{n}] (s16 x,y,z, flag, s16 s,t, rgba)', tag + ' gsSPVertex')
            elif op == 0x06:
                t = s.res(w1, seg)
                if t is not None: s.do_dl(t, tag + ' gsSPDisplayList', depth + 1, seg)
        for t, nb, fs, wh, _, pal in pend:
            fmt, siz = fs or (2, 0)
            name = {0: 'RGBA', 2: 'CI', 3: 'IA', 4: 'I'}.get(fmt, 'F%d' % fmt) + str(4 << siz)
            w, h = wh or (0, 0)
            s.add_clip(t, t + nb, 'texture', f'{name} {w}x{h}', tag, fmt=name, w=w, h=h,
                       pal=pal[0] if pal and fmt == 2 else None, paln=pal[1] if pal and fmt == 2 else 0)

    def off(s, ptr):
        o = ptr - BASE
        return o if 0 <= o < len(s.d) else None

    def run(s, refs):
        d = s.d; L = len(d)
        uniq_of = {}
        for o, rl in refs.items():   # model headers named by the code (actor / map object / type 11 / camera handlers)
            if o % 4 or o in s.models or o + 0x28 > L: continue
            if not any(r.split(':')[0] in MODEL_FUNCS for r in rl): continue
            if u16(d, o + 4) and s.off(u32(d, o + 8)) is not None and s.off(u32(d, o + 0xC)) is not None and u32(d, o) >> 24 == 0x20:
                s.models.append(o); s.add_clip(o, o + 0x24, 'model_header', 'model header (0x28 bytes; lvlB layout)', ' '.join(rl))
        s.models.sort()
        # 1. model header tail word (+0x24) and what +0x14 / +0x24 point at
        for h in s.models:
            s.add(h + 0x24, h + 0x28, 'model_header_tail', 'model header +0x24: pointer to the 6-float bounds block',
                  'model header (lvlB model_header)')
            ol = s.off(u32(d, h + 0x14)); bb = s.off(u32(d, h + 0x24))
            if ol is not None and u32(d, h + 0x14):
                r = order_list(d, ol, L)
                if r:
                    uniq_of[h] = r[1]
                    s.add(ol, r[0], 'vertex_order', f'u16 vertex order list: {r[1]} animated vertices -> {u16(d, h + 4)} Vtx '
                          f'(model 0x{h + BASE:08X})', f'model 0x{h + BASE:08X} +0x14 (func_80000450)', model=h)
            if bb is not None and u32(d, h + 0x24):
                s.add(bb, bb + 0x18, 'bounds', 'f32[6] model bounds (xmin,ymin,zmin,xmax,ymax,zmax)',
                      f'model 0x{h + BASE:08X} +0x24', model=h)
        s.uniq_of = uniq_of
        # 2. code-built animated texture tables
        for tab, n, fn in TEXANIM.get((s.game, s.name), []):
            t = s.off(tab)
            ptrs = [u32(d, t + 4 * i) for i in range(n)]
            s.add(t, t + 4 * n, 'texanim_table', f'u32[{n}] frame pointers (palette16 + CI4 32x32)', f'{fn} (D_{tab:08X})')
            for i, p in enumerate(ptrs):
                o = s.off(p)
                s.add(o, o + 0x20, 'palette', 'TLUT RGBA5551, 16 colours (animated texture frame)', f'D_{tab:08X}[{i}] ({fn})', tex=o + 0x20)
                s.add(o + 0x20, o + 0x220, 'texture', 'CI4 32x32 animated texture frame', f'D_{tab:08X}[{i}] + 0x20 ({fn})',
                      fmt='CI4', w=32, h=32, pal=o, paln=16)
        # 4. display lists: BSP leaf lists first (+0x10, vertices +0x14), then any free 8-aligned list
        for lf in s.leaves:
            dl = s.off(u32(d, lf + 0x10))
            if dl is not None: s.do_dl(dl, f'bsp_leaf 0x{lf + BASE:08X} +0x10 (DrawWorld func_80025E4C)')
        for h in s.models:   # model lists: segment 3 = vertex block (+0x0C), segment 4 = texture block (+0x1C)
            dl = s.off(u32(d, h + 8))
            if dl is not None: s.do_dl(dl, f'model 0x{h + BASE:08X} +0x08', 0, {3: u32(d, h + 0xC) or None, 4: u32(d, h + 0x1C) or None})
        for o in range(0, L - 8, 8):
            if s.free(o, o + 8): s.do_dl(o, ' '.join(refs.get(o, [])) or 'no static reference', scan=True)
        # 3. morph animations: every 4-aligned header in free space
        heads = [o for o in range(0, L - 8, 4) if anim_hdr(d, o) and s.free(o, o + 8)]
        hs = set(heads)
        for o in heads:
            if not s.free(o, o + 8): continue
            a = anim_hdr(d, o); st = o + a['data_off']; row = 4 * a['nkey']
            lim = s.nxt(st)
            nh = [h for h in heads if st < h < lim]
            if nh: lim = nh[0]
            nv = (lim - st) // row
            ref = ' '.join(refs.get(o, [])) or 'none found (see REPORT)'
            s.add(o, st, 'anim_header', f'morph animation header: {a["nkey"]} keys, {a["ticks"]} ticks, flags 0x{a["flags"]:04X}', ref)
            s.add(st, st + nv * row, 'anim_vertices', f'morph animation: {nv} vertices x {a["nkey"]} keys, z:10 y:11 x:11 bits from MSB',
                  f'anim 0x{o + BASE:08X} (func_8000092C / func_80000450)', anim=o, nvert=nv)
            if lim > st + nv * row:
                s.add(st + nv * row, lim, 'anim_tail', 'packer slack after an animation (less than one vertex row; never read)',
                      f'after anim 0x{o + BASE:08X}')
        # 4b. type-3 collision quads grouped as {shape ptrs[n]; u32 n; u32 &ptrs}
        for p in range(8, L - 8, 4):
            n = u32(d, p)
            if not 1 <= n <= 64 or u32(d, p + 4) != BASE + p - 4 * n: continue
            sh = [s.off(u32(d, p - 4 * n + 4 * i)) for i in range(n)]
            if any(x is None or u32(d, x) != 3 for x in sh): continue
            for x in sh:
                s.add_clip(x, x + 0x50, 'coll_quad', 'collision shape type 3: planar quad (normal, d, 4 corners)',
                           f'quad set 0x{p + BASE:08X}; tested by func_80042880 when linked (mask 0 here)')
            s.add_clip(p - 4 * n, p + 8, 'coll_quad_set', f'u32[{n}] quad pointers, u32 count, u32 pointer to the array',
                       ' '.join(refs.get(p, [])) or 'no static reference found')
        # 4c. bytes of a model's vertex (seg 3) / texture (seg 4) block that its display list never loads
        for h in s.models:
            t4, n4 = s.off(u32(d, h + 0x1C)), u32(d, h + 0x20)
            if t4 is None or not u32(d, h + 0x1C): continue
            o = t4
            while o < min(t4 + n4, L):
                if not s.free(o, o + 1): o += 1; continue
                e = min(s.nxt(o), t4 + n4)
                why = f'model 0x{h + BASE:08X} texture block (seg 4 +0x{o - t4:X}), not loaded by its display list'
                if e - o == 0x220:
                    s.add(o, o + 0x20, 'palette', 'TLUT RGBA5551, 16 colours (unused frame)', why, tex=o + 0x20)
                    s.add(o + 0x20, e, 'texture', 'CI4 32x32 (unused frame: same layout as the animated frames)', why,
                          fmt='CI4', w=32, h=32, pal=o, paln=16)
                else:
                    s.add(o, e, 'texture_block_unused', f'{e - o} bytes inside a model texture block that no load reaches', why)
                o = e
        # 5. pointer tables: >=2 consecutive words pointing into the image
        o = 0
        while o < L - 8:
            n = 0
            while s.free(o + 4 * n, o + 4 * n + 4) and s.off(u32(d, o + 4 * n)) is not None and u32(d, o + 4 * n) & 3 == 0: n += 1
            if n >= 2:
                s.add(o, o + 4 * n, 'ptr_table', f'u32[{n}] pointers into the level image', ' '.join(refs.get(o, [])) or 'pointer table')
                o += 4 * n
            else: o += 4
        # 6. the rest: zero runs = pad, else unknown
        o = 0
        while o < L:
            if not s.free(o, o + 1): o += 1; continue
            e = s.nxt(o)
            seg = d[o:e]
            if not any(seg): s.add(o, e, 'pad', 'zero bytes', 'none')
            elif e - o == 0x28 and any(seg[:2]) and not any(seg[2:]):
                why = 'no reference (same TLUT16 + 8-byte CI4 16x1 strip layout as the referenced pair that follows)'
                s.add(o, o + 0x20, 'palette', 'TLUT RGBA5551, 16 colours (unreferenced)', why, tex=o + 0x20)
                s.add(o + 0x20, e, 'texture', 'CI4 16x1 (unreferenced)', why, fmt='CI4', w=16, h=1, pal=o, paln=16)
            else: s.add(o, e, 'unknown', 'unidentified', ' '.join(refs.get(o, [])) or 'none')
            o = e
        s.items.sort(key=lambda i: i['s'])
        s.anim_report = []
        for it in s.items:
            if it['cls'] == 'anim_vertices':
                ms = [f'0x{h + BASE:08X}' for h, u in uniq_of.items() if u == it['nvert']]
                s.anim_report.append((it['anim'], it['nvert'], ms))

def dl_tile_size(d, s, e):
    w = h = 0
    for p in range(s, e, 8):
        if d[p] == 0xF2:
            w1 = u32(d, p + 4)
            w = (((w1 >> 12) & 0xFFF) >> 2) + 1; h = ((w1 & 0xFFF) >> 2) + 1
    return w, h

# ---------------------------------------------------------------- serialisation
def rgba5551(c): return ((c >> 11 & 31) * 255 // 31, (c >> 6 & 31) * 255 // 31, (c >> 1 & 31) * 255 // 31, 255 if c & 1 else 0)

def decode(d, it):
    a, b = it['s'], it['e']; c = it['cls']; raw = d[a:b]
    if c in ('model_header_tail', 'ptr_table', 'texanim_table'):
        return {'words': [u32(raw, i) for i in range(0, len(raw), 4)]}
    if c == 'bounds':
        return {'floats': [struct.unpack_from('>f', raw, i)[0] for i in range(0, 24, 4)]}
    if c == 'vertex_order':
        return {'u16': [u16(raw, i) for i in range(0, len(raw), 2)]}
    if c == 'anim_header':
        h = anim_hdr(d, a); h['hdr_end'] = u16(raw, 0) & 0xFF; return h
    if c == 'anim_vertices':
        nk = (b - a) // 4 // it['nvert']; out = []
        for v in range(it['nvert']):
            row = []
            for k in range(nk):
                w = u32(raw, (v * nk + k) * 4)
                sx = lambda x, n: x - (1 << n) if x & (1 << (n - 1)) else x
                row.append([sx(w & 0x7FF, 11), sx((w >> 11) & 0x7FF, 11), sx(w >> 22, 10)])
            out.append(row)
        return {'nkey': nk, 'verts': out}
    if c == 'display_list':
        return {'cmds': [['%08X' % u32(raw, i), '%08X' % u32(raw, i + 4)] for i in range(0, len(raw), 8)]}
    if c == 'palette':
        return {'rgba5551': [u16(raw, i) for i in range(0, len(raw), 2)]}
    if c == 'texture':
        bits = int(it['fmt'].lstrip('RGBACI'))
        if bits == 4: idx = [x for byte in raw for x in (byte >> 4, byte & 15)]
        elif bits == 8: idx = list(raw)
        elif bits == 16: idx = [u16(raw, i) for i in range(0, len(raw), 2)]
        else: idx = [u32(raw, i) for i in range(0, len(raw), 4)]
        return {'fmt': it['fmt'], 'w': it['w'], 'h': it['h'], 'texels': idx}
    if c == 'pad': return {'zero': len(raw)}
    return {'hex': raw.hex()}

def encode(j, c):
    if 'words' in j: return b''.join(struct.pack('>I', w) for w in j['words'])
    if 'floats' in j: return b''.join(struct.pack('>f', f) for f in j['floats'])
    if 'u16' in j: return b''.join(struct.pack('>H', w) for w in j['u16'])
    if c == 'anim_header':
        out = struct.pack('>HHBBH', j['hdr_end'] | j['hi'], j['data_off'], j['ticks'], j['nkey'], j['flags']) + bytes(j['keys'])
        return out + bytes(j['data_off'] - len(out))
    if 'verts' in j:
        return b''.join(struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF)) for row in j['verts'] for x, y, z in row)
    if 'cmds' in j: return b''.join(bytes.fromhex(a + b) for a, b in j['cmds'])
    if 'rgba5551' in j: return b''.join(struct.pack('>H', w) for w in j['rgba5551'])
    if 'texels' in j:
        t = j['texels']
        bits = int(j['fmt'].lstrip('RGBACI'))
        if bits == 4: return bytes((t[i] << 4) | t[i + 1] for i in range(0, len(t), 2))
        if bits == 8: return bytes(t)
        return b''.join(struct.pack('>H' if bits == 16 else '>I', w) for w in t)
    if 'zero' in j: return bytes(j['zero'])
    return bytes.fromhex(j['hex'])

def texel_rgba(fmt, v, pal):
    if fmt.startswith('CI'): return pal[v] if v < len(pal) else (0, 0, 0, 0)
    if fmt == 'RGBA16': return rgba5551(v)
    if fmt == 'RGBA32': return (v >> 24, v >> 16 & 255, v >> 8 & 255, v & 255)
    if fmt == 'IA4': i = (v >> 1) * 255 // 7; return (i, i, i, 255 if v & 1 else 0)
    if fmt == 'IA8': i = (v >> 4) * 17; return (i, i, i, (v & 15) * 17)
    if fmt == 'IA16': return (v >> 8, v >> 8, v >> 8, v & 255)
    if fmt == 'I4': return (v * 17,) * 3 + (255,)
    if fmt == 'I8': return (v, v, v, 255)
    return (v & 255,) * 3 + (255,)

def write_png(path, d, it):
    """PNG for viewing only (the JSON keeps the exact texels)."""
    j = decode(d, it); w, h = it['w'], it['h']
    if not w or not h or len(j['texels']) != w * h: return False
    pal = [rgba5551(u16(d, it['pal'] + 2 * i)) for i in range(it['paln'])] if it.get('pal') is not None else []
    px = [bytes(texel_rgba(it['fmt'], v, pal)) for v in j['texels']]
    raw = b''.join(b'\0' + b''.join(px[y * w:(y + 1) * w]) for y in range(h))
    def chunk(t, b): return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xFFFFFFFF)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 6, 0, 0, 0)) +
                           chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))
    return True

# ---------------------------------------------------------------- refs from the program
def code_refs(game):
    r = {}
    for l in open(os.path.join(SES, f'LEVEL_REFS_{game}.tsv')):
        p = l.rstrip('\n').split('\t')
        if not p[0].startswith('0x') or not p[1].startswith('level_image'): continue
        r.setdefault(int(p[2], 16), []).extend(p[3].split())
    return r

def pointer_refs(d):
    r = {}
    for i in range(0, len(d) - 3, 4):
        w = u32(d, i) - BASE
        if 0 <= w < len(d): r.setdefault(w, []).append(f'ptr@0x{i:X}')
    return r

def analyse(rom, game):
    bifs = load_bifs(rom, game); cov, models = covered(game); cr = code_refs(game); out = {}
    for name in LEVELS[game]:
        d = bifs[name]; pr = pointer_refs(d)
        refs = {o: cr.get(o, []) + pr.get(o, [])[:4] for o in set(cr) | set(pr)}
        lv = Level(game, name, d, cov.get(name, []), models.get(name, [])); lv.run(refs); out[name] = lv
    return out

def main():
    if len(sys.argv) < 4: print(__doc__); sys.exit(1)
    rom, game, outdir = sys.argv[1:4]; mode = sys.argv[4] if len(sys.argv) > 4 else 'extract'
    lvs = analyse(rom, game); bad = 0
    if mode == 'map':
        for name, lv in lvs.items():
            for it in lv.items:
                ex = f'tools/extract_level_rest.py -> out/{game}/{name}/{it["s"]:05X}_{it["cls"]}.json' + ('/.png' if it['cls'] == 'texture' else '')
                print('\t'.join([game, f'{name}.bif:0x{it["s"]:05X}', f'{name}.bif:0x{it["e"]:05X}', it['cls'], FORMATS[it['cls']], it['desc'], it['ref'], ex]))
        return
    for name, lv in lvs.items():
        od = os.path.join(outdir, game, name); os.makedirs(od, exist_ok=True)
        idx = []
        tot = {}
        for it in lv.items:
            j = decode(lv.d, it); tot[it['cls']] = tot.get(it['cls'], 0) + it['e'] - it['s']
            if mode == 'check':
                # (g3) re-encode from the JSON file written by extract, not from the in-memory decode
                jf = json.load(open(os.path.join(od, f'{it["s"]:05X}_{it["cls"]}.json')))['data']
                if encode(jf, it['cls']) != lv.d[it['s']:it['e']]:
                    bad += 1; print('MISMATCH', name, hex(it['s']), it['cls'])
                continue
            fn = f'{it["s"]:05X}_{it["cls"]}'
            meta = {k: v for k, v in it.items()}
            json.dump(dict(meta=meta, data=j), open(os.path.join(od, fn + '.json'), 'w'))
            if it['cls'] == 'texture': write_png(os.path.join(od, fn + '.png'), lv.d, it)
            idx.append(meta)
        if mode != 'check': json.dump(idx, open(os.path.join(od, 'items.json'), 'w'), indent=0)
        # coverage proof: own + mine tiles the file
        gaps = sum(1 for i in range(len(lv.d)) if not lv.own[i] and not lv.mine[i])
        for a, nv, ms in lv.anim_report:
            print(f'  anim 0x{a + BASE:08X} nvert {nv} models with that many animated vertices: {" ".join(ms) or "NONE"}')
        print(name, 'items', len(lv.items), 'gaps', gaps, ' '.join(f'{k}={v}' for k, v in sorted(tot.items())))
        bad += gaps
    print('check', 'FAILED' if bad else 'OK') if mode == 'check' else None
    sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
