#!/usr/bin/env python3
"""Decode the bytes of the Elmo .bif level images that neither lvlB (graphics) nor lvlC (objects/BSP) cover.

usage: extract_level_rest.py ROM GAME OUTDIR [LEVEL ...]        -> OUTDIR/<level>/... + OUTDIR/MAP.part.tsv
       extract_level_rest.py check ROM GAME OUTDIR [LEVEL ...]  -> re-serialise every structure from the
                                                                  written JSON/PNG and compare with the bytes
GAME is letter or number. LEVEL defaults to all eight levels of the game.
Coverage of the other agents is read from ../../lvlB/MAP.tsv and ../../lvlC/MAP.tsv (relative to this file);
no ROM bytes are stored anywhere but OUTDIR.  See ../FORMAT.md.
"""
import sys, os, json, struct, zlib, collections

HERE = os.path.dirname(os.path.abspath(__file__))
SES = os.path.normpath(os.path.join(HERE, '..', '..'))
BASE = 0x80154000
LEVELS = {
    'letter': [('streetab', 0x1BF6F0, 0x204D00), ('chute', 0x204D00, 0x227BA0), ('manor', 0x227BA0, 0x243E20),
               ('orchard', 0x243E20, 0x2604B0), ('garden', 0x2604B0, 0x28FDB0), ('space', 0x28FDB0, 0x2ACFA0),
               ('planet', 0x2ACFA0, 0x2D1FB0), ('bonusabc', 0x2D1FB0, 0x2F9B60)],
    'number': [('street', 0x177680, 0x1BCE90), ('dunk', 0x1BCE90, 0x1D6CA0), ('bump', 0x1D6CA0, 0x206CC0),
               ('mansion', 0x206CC0, 0x22EC10), ('maze', 0x22EC10, 0x25AF80), ('ski', 0x25AF80, 0x27ADC0),
               ('river', 0x27ADC0, 0x2B5120), ('bonus123', 0x2B8FD0, 0x2E6FC0)],
}

def inflate(rom, s, e):
    return zlib.decompressobj(31).decompress(rom[s:e])

def u32(d, o): return struct.unpack_from('>I', d, o)[0]
def u16(d, o): return struct.unpack_from('>H', d, o)[0]

def load_cov(game, name):
    cov = []
    for src in ('lvlB', 'lvlC'):
        for l in open(os.path.join(SES, src, 'MAP.tsv')):
            f = l.rstrip('\n').split('\t')
            if f[0] != game or len(f) < 4: continue
            if src == 'lvlB' and f[3] == 'level_data': continue
            fn, s = f[1].split(':')
            if fn != name + '.bif': continue
            cov.append((int(s, 16), int(f[2].split(':')[1], 16), src + ':' + f[3]))
    return sorted(cov)

def gaps_of(cov, size):
    out, p = [], 0
    for s, e, c in cov:
        if s > p: out.append((p, s))
        p = max(p, e)
    if p < size: out.append((p, size))
    return out

# ---------------------------------------------------------------- morph animation (sprA format)
def anim_hdr(d, o, e):
    if o + 8 > e or o & 3: return None
    he, do, t, nk, fl = struct.unpack_from('>HHBBH', d, o)
    if nk < 2 or he != 8 + nk or do & 7 or do < he or do > he + 8 or fl not in (0, 0x8000): return None
    if o + do + 4 * nk > e: return None
    ks = d[o + 8:o + 8 + nk]
    if ks[0] != 0 or any(ks[i] >= ks[i + 1] for i in range(nk - 1)) or ks[-1] > t: return None
    if any(d[o + he:o + do]): return None
    return do, nk

def anim_json(d, s, e):
    he, do, t, nk, fl = struct.unpack_from('>HHBBH', d, s)
    nv = (e - s - do) // (4 * nk)
    verts = []
    for i in range(nv):
        row = []
        for k in range(nk):
            w = u32(d, s + do + (i * nk + k) * 4)
            x = w & 0x7FF; x = x - 0x800 if x & 0x400 else x
            y = (w >> 11) & 0x7FF; y = y - 0x800 if y & 0x400 else y
            z = w >> 22; z = z - 0x400 if z & 0x200 else z
            row.append([x, y, z])
        verts.append(row)
    return {'hdr_end': he, 'data_off': do, 'ticks': t, 'nkey': nk, 'flags': fl,
            'keys': list(d[s + 8:s + 8 + nk]), 'nvert': nv, 'verts': verts}

def anim_pack(j):
    b = bytearray(struct.pack('>HHBBH', j['hdr_end'], j['data_off'], j['ticks'], j['nkey'], j['flags']))
    b += bytes(j['keys']); b += bytes(j['data_off'] - len(b))
    for row in j['verts']:
        for x, y, z in row:
            b += struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF))
    return bytes(b)

# ---------------------------------------------------------------- vertex duplication list (mesh header +0x14)
def dup_list_end(d, o, e, total):
    p, t = o, 0
    while t < total:
        if p + 4 > e: return None
        n, m = struct.unpack_from('>HH', d, p); p += 4 + 2 * m; t += n + m
    return p if p <= e and t == total else None

def dup_json(d, s, e):
    p, groups = s, []
    while p < e:
        n = u16(d, p); p += 2
        m = u16(d, p); p += 2
        groups.append({'new': n, 'dup': [u16(d, p + 2 * i) for i in range(m)]}); p += 2 * m
    return {'groups': groups}

def dup_pack(j):
    b = bytearray()
    for g in j['groups']:
        b += struct.pack('>HH', g['new'], len(g['dup'])) + struct.pack('>%dH' % len(g['dup']), *g['dup'])
    return bytes(b)

# ---------------------------------------------------------------- generic word structures
def f32(w): return struct.unpack('>f', struct.pack('>I', w))[0]
def words_json(d, s, e, floats=False):
    ws = [u32(d, o) for o in range(s, e, 4)]
    j = {'words': ['%08X' % w for w in ws]}
    if floats: j['f32'] = [f32(w) for w in ws]
    return j
def words_pack(j): return b''.join(struct.pack('>I', int(w, 16)) for w in j['words'])

# ---------------------------------------------------------------- texture-loading display lists
SIZB = {0: 0.5, 1: 1, 2: 2, 3: 4}
def walk_dl(d, o, e):
    """Return (end, textures) for a DL starting with gsDPPipeSync and ending with gsSPEndDisplayList."""
    p, tex, img, tlut, fmt, siz = o, [], None, None, None, None
    while p + 8 <= e:
        w0, w1 = u32(d, p), u32(d, p + 4); c = w0 >> 24; p += 8
        if c not in (0xE7, 0xBA, 0xB9, 0xFD, 0xE8, 0xF5, 0xE6, 0xF0, 0xF3, 0xF2, 0xB8, 0xFC, 0xFB, 0xB6, 0xB7, 0xE5):
            return None
        if c == 0xFD: img = (w1, (w0 >> 21) & 7, (w0 >> 19) & 3)
        elif c == 0xF0 and img:
            tlut = (img[0], ((w1 >> 14) & 0x3FF) + 1); img = None
        elif c == 0xF5 and ((w1 >> 24) & 7) != 7: fmt, siz = (w0 >> 21) & 7, (w0 >> 19) & 3
        elif c == 0xF3 and img: timg = img
        elif c == 0xF2 and fmt is not None:
            w = ((w1 >> 12) & 0xFFF) // 4 + 1; h = (w1 & 0xFFF) // 4 + 1
            tex.append({'image': img[0] if img else timg[0], 'fmt': fmt, 'siz': siz, 'w': w, 'h': h,
                        'tlut': tlut[0] if tlut else None, 'ncol': tlut[1] if tlut else 0})
            img = None
        if c == 0xB8: return p, tex
    if p == e and p - o >= 16:
        if img and img[1] == 0 and img[2] == 2:  # palette image set, TLUT load continues past the gap
            tex.append({'image': None, 'tlut': img[0], 'ncol': 256, 'fmt': None, 'siz': None, 'w': 0, 'h': 0})
        return p, tex
    return None

def rgba5551(v): return ((v >> 11 & 31) * 255 // 31, (v >> 6 & 31) * 255 // 31, (v >> 1 & 31) * 255 // 31, 255 if v & 1 else 0)

def write_png(path, d, s, t, pal):
    w, h = t['w'], t['h']
    if t['fmt'] == 2:
        if t['siz'] == 1: idx = d[s:s + w * h]
        else: idx = bytes(x for b in d[s:s + w * h // 2] for x in (b >> 4, b & 15))
        cols = [rgba5551(v) for v in pal]
        px = [cols[i] if i < len(cols) else (0, 0, 0, 0) for i in idx]
    elif t['fmt'] == 0 and t['siz'] == 2:
        px = [rgba5551(u16(d, s + 2 * i)) for i in range(w * h)]
    else:
        return
    raw = b''.join(b'\0' + bytes(c for p in px[y * w:(y + 1) * w] for c in p) for y in range(h))
    def chunk(t, b): return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xFFFFFFFF)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 6, 0, 0, 0))
                          + chunk(b'IDAT', zlib.compress(raw)) + chunk(b'IEND', b''))

# ---------------------------------------------------------------- classifier
def classify(d, cov):
    size = len(d)
    gaps = gaps_of(cov, size)
    ingap = lambda o, n=1: any(s <= o and o + n <= e for s, e in gaps)
    claim = []  # (s, e, cls, note)
    # 1. model header tail word (+0x24 = bbox pointer) and what the header points to
    for s, e, c in cov:
        if c != 'lvlB:model_header' or e - s != 0x24: continue
        if ingap(e, 4):
            claim.append((e, e + 4, 'model_header_tail', 'model header +0x24: u32 pointer to bounding box (read by func_8000092C)'))
            bb = u32(d, e) - BASE
            if 0 <= bb < size and ingap(bb, 24):
                claim.append((bb, bb + 24, 'bbox', 'f32 min x,y,z, max x,y,z (model header +0x24; func_8000092C uses the centre)'))
        vl = u32(d, s + 0x14) - BASE
        if 0 <= vl < size and ingap(vl, 2):
            g = [x for x in gaps if x[0] <= vl < x[1]][0]
            ve = dup_list_end(d, vl, g[1], u16(d, s + 4))
            if ve: claim.append((vl, ve, 'vertex_dup_list', 'u16 groups {n new, m dup, m indices} until sum(n+m) = model header u16 +4 (model header +0x14; func_80000450)'))
    # 2. quad collider lists: {u32 n; u32 ptr->ptr[n]} with records of 0x50 bytes
    for gs, ge in gaps:
        for o in range((gs + 3) & ~3, ge - 7, 4):
            n, pa = u32(d, o), u32(d, o + 4) - BASE
            if not (1 <= n <= 64 and 0 <= pa < size and pa + 4 * n <= o + 4 and ingap(pa, 4 * n)): continue
            recs = [u32(d, pa + 4 * i) - BASE for i in range(n)]
            if not all(0 <= r < size and ingap(r, 0x50) and r % 4 == 0 for r in recs): continue
            claim.append((pa, pa + 4 * n, 'collider_ptrs', 'u32 pointers to the n 0x50-byte collider quads'))
            claim.append((o, o + 8, 'collider_list', 'u32 n; u32 pointer to pointer array (code: func_80023118 spD4)'))
            for r in recs: claim.append((r, r + 0x50, 'collider_quad', '0x50: u32 kind, f32 plane, 4 corners (func_80023118)'))
    # 3. unreached texture-loading DLs and their textures
    texs = []
    for gs, ge in gaps:
        o = (gs + 7) & ~7
        while o + 8 <= ge:
            if u32(d, o) == 0xE7000000 and u32(d, o + 4) == 0:
                r = walk_dl(d, o, ge)
                if r:
                    claim.append((o, r[0], 'texload_dl', 'F3DEX texture/palette load sub-list (no gsSPDisplayList reaches it; called by code)'))
                    texs += r[1]; o = r[0]; continue
            o += 8
    seen = set()
    for t in texs:
        if t['tlut'] is not None:
            p = t['tlut'] - BASE
            if (p, 'pal') not in seen and 0 <= p < size:
                seen.add((p, 'pal')); claim.append((p, p + 2 * t['ncol'], 'tex_palette', 'TLUT RGBA5551 x%d' % t['ncol']))
        if t['image'] is None: continue
        i = t['image'] - BASE; n = int(t['w'] * t['h'] * SIZB[t['siz']])
        if (i, 'img') not in seen and 0 <= i < size:
            seen.add((i, 'img')); claim.append((i, i + n, 'tex_image', 'fmt %d siz %d %dx%d' % (t['fmt'], t['siz'], t['w'], t['h'])))
    # 4. morph animations
    for gs, ge in gaps:
        o = (gs + 3) & ~3
        while o < ge - 8:
            h = anim_hdr(d, o, ge)
            if not h: o += 4; continue
            do, nk = h; q = o + do
            nxt = ge
            for r in range(q, ge - 8, 4):
                if anim_hdr(d, r, ge): nxt = r; break
            for s2, e2, c2, _ in claim:
                if o < s2 < nxt: nxt = s2
            nv = (nxt - o - do) // (4 * nk)
            claim.append((o, o + do + 4 * nv * nk, 'morph_anim', 'keyframed vertex morph animation (sprA format; func_8000092C/func_80000450)'))
            o = o + do + 4 * nv * nk
    # clip claims to gaps, resolve overlaps (first claim wins), fill rest
    claim = sorted(set((s, e, c, n) for s, e, c, n in claim if ingap(s, e - s)))
    out, used = [], []
    for s, e, c, n in claim:
        if any(not (e <= a or s >= b) for a, b in used): continue
        used.append((s, e)); out.append((s, e, c, n))
    for gs, ge in gaps:
        p = gs
        for s, e, c, n in sorted(x for x in out if gs <= x[0] < ge) + [(ge, ge, None, None)]:
            if s > p:
                seg = d[p:s]
                if not any(seg): out.append((p, s, 'pad', 'zero bytes'))
                elif s - p < 4 and (s % 4 == 0 or s % 8 == 0) and c in ('morph_anim', 'bbox', 'collider_quad', 'collider_list', 'texload_dl'):
                    out.append((p, s, 'pad', 'alignment bytes (non-zero)'))
                else:
                    out.append((p, s, 'unknown', 'not referenced by any pointer in the level image, the code roots (LEVEL_REFS) or a decoded structure; looks like a 0x200-byte RGBA5551 palette (first 0x30 bytes zero) followed by 8-bit indices (CI8 64x32?)' if s - p == 2560 else 'unreferenced'))
            p = max(p, e)
    return sorted(out), texs

UNKNOWN_NOTES = {}

def run(argv):
    check = argv[0] == 'check'
    if check: argv = argv[1:]
    rom, game, outdir = open(argv[0], 'rb').read(), argv[1], argv[2]
    want = argv[3:]
    os.makedirs(outdir, exist_ok=True)
    maplines, bad = [], 0
    for name, s, e in LEVELS[game]:
        if want and name not in want: continue
        d = inflate(rom, s, e)
        cov = load_cov(game, name)
        items, texs = classify(d, cov)
        ld = os.path.join(outdir, name); os.makedirs(ld, exist_ok=True)
        texinfo = {t['image'] - BASE: t for t in texs if t['image']}
        pals = {}
        for t in texs:
            if t['tlut'] and t['image']: pals[t['image'] - BASE] = [u16(d, t['tlut'] - BASE + 2 * i) for i in range(t['ncol'])]
        for a, b, c, note in items:
            fn = os.path.join(ld, '%05X_%s' % (a, c))
            raw = d[a:b]
            if c == 'morph_anim': J, pk = (lambda: anim_json(d, a, b)), anim_pack
            elif c == 'vertex_dup_list': J, pk = (lambda: dup_json(d, a, b)), dup_pack
            elif c in ('bbox', 'collider_quad'): J, pk = (lambda: words_json(d, a, b, True)), words_pack
            elif c in ('pad',): J, pk = (lambda: {'len': b - a, 'hex': raw.hex() if any(raw) else None}), \
                (lambda j: bytes.fromhex(j['hex']) if j['hex'] else bytes(j['len']))
            elif c == 'tex_palette': J, pk = (lambda: {'rgba5551': [u16(d, x) for x in range(a, b, 2)]}), \
                (lambda j: struct.pack('>%dH' % len(j['rgba5551']), *j['rgba5551']))
            elif c == 'tex_image' or c == 'unknown' or c == 'texload_dl':
                J, pk = (lambda: {'hex': raw.hex()}), (lambda j: bytes.fromhex(j['hex']))
            else: J, pk = (lambda: words_json(d, a, b)), words_pack
            if check:
                j = json.load(open(fn + '.json'))
                if pk(j) != raw: bad += 1; print('MISMATCH', name, hex(a), c)
                continue
            json.dump(J(), open(fn + '.json', 'w'))
            if c == 'tex_image' and a in texinfo: write_png(fn + '.png', d, a, texinfo[a], pals.get(a, []))
            if c == 'morph_anim':
                j = anim_json(d, a, b)
                with open(fn + '_k0.obj', 'w') as f:
                    for x, y, z in (r[0] for r in j['verts']): f.write('v %d %d %d\n' % (x, y, z))
            maplines.append('%s\t%s.bif:0x%05X\t%s.bif:0x%05X\t%s\t%s' % (game, name, a, name, b, c, note))
    if check:
        print('check', game, 'OK' if not bad else '%d mismatches' % bad); return 1 if bad else 0
    open(os.path.join(outdir, 'MAP.part.tsv'), 'w').write('\n'.join(maplines) + '\n')
    tot = collections.Counter()
    for l in maplines:
        f = l.split('\t'); tot[f[3]] += int(f[2].split(':')[1], 16) - int(f[1].split(':')[1], 16)
    print(dict(tot))
    return 0

if __name__ == '__main__':
    sys.exit(run(sys.argv[1:]))
