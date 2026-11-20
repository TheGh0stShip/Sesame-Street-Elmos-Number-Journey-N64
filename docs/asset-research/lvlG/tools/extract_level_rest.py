#!/usr/bin/env python3
"""Extract the level-image bytes that neither lvlB (graphics) nor lvlC (objects/BSP) cover.

usage: extract_level_rest.py ROM GAME OUTDIR [extract|check|map]
  GAME   number (street, dunk, bump, mansion) -- the table below can be extended
  extract  writes OUTDIR/<level>/... (JSON, PNG, TXT) and OUTDIR/MAP.tsv
  check    re-serialises every extracted item from OUTDIR and compares with the inflated bytes
  map      only writes OUTDIR/MAP.tsv

Territory = inflated .bif bytes minus lvlB's non-`level_data` ranges minus lvlC's ranges
(read from ../../lvlB/MAP.tsv and ../../lvlC/MAP.tsv). See ../FORMAT.md for every format.
No ROM content is stored in this file; everything comes from the ROM given on the command line.
"""
import sys, os, struct, zlib, json, hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
SES = os.path.normpath(os.path.join(HERE, '..', '..'))
BASE = 0x80154000
LEVELS = {'number': {'street': (0x177680, 0x1BCE90), 'dunk': (0x1BCE90, 0x1D6CA0),
                     'bump': (0x1D6CA0, 0x206CC0), 'mansion': (0x206CC0, 0x22EC10)}}
GCC = b'\0GCC: (GNU) 2.7.2\0'

def u16(d, o): return struct.unpack_from('>H', d, o)[0]
def u32(d, o): return struct.unpack_from('>I', d, o)[0]

def inflate(rom, a, b):
    return zlib.decompressobj(31).decompress(rom[a:b])

def covered(game, level):
    cov = []
    for src, skip_ld in (('lvlB/MAP.tsv', True), ('lvlC/MAP.tsv', False)):
        for l in open(os.path.join(SES, src)):
            p = l.rstrip('\n').split('\t')
            if p[0] != game or len(p) < 4: continue
            if skip_ld and p[3] == 'level_data': continue
            f, a = p[1].split(':'); b = p[2].split(':')[1]
            if f.replace('.bif', '') == level:
                cov.append((int(a, 16), int(b, 16), src.split('/')[0], p[3]))
    return sorted(cov)

def gaps_of(cov, size):
    out, pos = [], 0
    for a, b, _, _ in cov:
        if a > pos: out.append((pos, a))
        pos = max(pos, b)
    if pos < size: out.append((pos, size))
    return out

def is_ptr(w, size): return BASE <= w < BASE + size

# ---------------------------------------------------------------- anim (vertex morph)
def anim_header(d, o):
    if o + 8 > len(d): return None
    he, do, ti, nk, fl = struct.unpack_from('>HHBBH', d, o)
    if nk < 2 or he != 8 + nk or do not in ((he + 7) & ~7, (he + 8) & ~7) or fl & 0x7FFF: return None
    k = d[o + 8:o + 8 + nk]
    if len(k) < nk or k[0] != 0 or any(k[i] >= k[i + 1] for i in range(nk - 1)) or k[-1] > ti: return None
    if any(d[o + he:o + do]): return None
    return dict(hdr_end=he, data_off=do, ticks=ti, nkey=nk, flags=fl, keys=list(k))

def sx(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v

def unpack_vtx(w):
    return [sx(w & 0x7FF, 11), sx((w >> 11) & 0x7FF, 11), sx(w >> 22, 10)]

def pack_vtx(x, y, z):
    return ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF)

def anim_decode(d, o, end):
    h = anim_header(d, o)
    nk, do = h['nkey'], h['data_off']
    nv = (end - o - do) // (4 * nk)
    words = struct.unpack_from('>%dI' % (nv * nk), d, o + do)
    h['nvert'] = nv
    h['keyframes'] = [[unpack_vtx(words[i * nk + k]) for i in range(nv)] for k in range(nk)]
    tail_at = o + do + nv * nk * 4
    h['tail_hex'] = d[tail_at:end].hex()
    return h

def anim_encode(h):
    nk = h['nkey']
    b = bytearray(struct.pack('>HHBBH', h['hdr_end'], h['data_off'], h['ticks'], nk, h['flags']))
    b += bytes(h['keys']); b += bytes(h['data_off'] - len(b))
    kf = h['keyframes']
    for i in range(h['nvert']):
        for k in range(nk):
            b += struct.pack('>I', pack_vtx(*kf[k][i]))
    return bytes(b) + bytes.fromhex(h['tail_hex'])

# ---------------------------------------------------------------- vertex run list (mesh+0x14)
def runs_decode(d, o):
    p, runs = o, []
    while True:
        n = u16(d, p); p += 2
        if n == 0: runs.append({'n': 0}); break
        m = u16(d, p); p += 2
        idx = list(struct.unpack_from('>%dH' % m, d, p)); p += 2 * m
        runs.append({'n': n, 'dup': idx})
        if m == 0: break
    return runs, p

def runs_encode(runs):
    b = bytearray()
    for r in runs:
        b += struct.pack('>H', r['n'])
        if r['n'] == 0: break
        b += struct.pack('>H', len(r['dup'])) + struct.pack('>%dH' % len(r['dup']), *r['dup'])
    return bytes(b)

# ---------------------------------------------------------------- collision quads (type 3)
QUAD_FMT = '>3I5f12f'   # type, owner, flags, 0x0C u32 + 4 f32 (plane), 4 corners
def quad_decode(d, o):
    v = struct.unpack_from('>4I4f12f', d, o)
    return {'type': v[0], 'owner': v[1], 'flags': v[2], 'w0C': v[3], 'plane': list(v[4:8]),
            'a': list(v[8:11]), 'b': list(v[11:14]), 'c': list(v[14:17]), 'd': list(v[17:20])}
def quad_encode(q):
    return struct.pack('>4I4f12f', q['type'], q['owner'], q['flags'], q['w0C'], *q['plane'], *q['a'], *q['b'], *q['c'], *q['d'])

# ---------------------------------------------------------------- texture-load display lists
def parse_texdl(d, o, end):
    """F3DEX texture-load DL (palette + CI image). returns (end_offset, cmds, tex) or None."""
    if o + 16 > end or d[o:o + 8] != bytes.fromhex('e700000000000000') or d[o + 8:o + 12] != bytes.fromhex('ba000e02'):
        return None
    cmds, p, timg, pal, img, fmt = [], o, None, None, None, None
    while p + 8 <= end:
        w0, w1 = u32(d, p), u32(d, p + 4); cmds.append('%08X %08X' % (w0, w1)); op = w0 >> 24; p += 8
        if op == 0xFD: timg = w1
        elif op == 0xF0: pal = (timg, ((w1 >> 14) & 0x3FF) + 1)
        elif op == 0xF5 and (w1 >> 24) == 0 and (w0 >> 21) & 7 == 2 and timg is not None and pal and img is None:
            img = timg; fmt = (w0 >> 19) & 3
        elif op == 0xF2: sz = (((w1 >> 12) & 0xFFF) >> 2) + 1, ((w1 & 0xFFF) >> 2) + 1
        elif op == 0xB8: break
    else:
        return None
    if pal is None or img is None: return None
    return p, cmds, {'palette': pal[0], 'ncolors': pal[1], 'image': img, 'bits': 4 if fmt == 0 else 8,
                     'width': sz[0], 'height': sz[1]}

def rgba5551(c):
    return ((c >> 11) & 31) * 255 // 31, ((c >> 6) & 31) * 255 // 31, ((c >> 1) & 31) * 255 // 31, 255 if c & 1 else 0


def png_write(path, w, h, rows, ctype):
    """minimal PNG: ctype 6 = RGBA8 (rows of bytes), 0 = 8-bit grey (index image); filter 0."""
    def chunk(t, b): return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xffffffff)
    raw = b''.join(b'\0' + r for r in rows)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, ctype, 0, 0, 0)) +
                           chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))

def png_read_grey(path):
    b = open(path, 'rb').read(); p = 8; idat = b''; w = h = 0
    while p < len(b):
        n = struct.unpack_from('>I', b, p)[0]; t = b[p + 4:p + 8]; c = b[p + 8:p + 8 + n]; p += 12 + n
        if t == b'IHDR': w, h = struct.unpack_from('>II', c)
        elif t == b'IDAT': idat += c
    raw = zlib.decompress(idat)
    return [x for r in range(h) for x in raw[r * (w + 1) + 1:(r + 1) * (w + 1)]]

# ---------------------------------------------------------------- classification
def classify(game, level, d):
    size = len(d)
    cov = covered(game, level)
    gaps = gaps_of(cov, size)
    items = {}          # start -> (end, cls, fmt, desc, ref, payload)
    def add(a, b, cls, fmt, desc, ref, payload=None):
        items[a] = (b, cls, fmt, desc, ref, payload)
    ingap = lambda a, b: any(ga <= a and b <= gb for ga, gb in gaps)
    # 1. mesh headers (lvlB model_header covers +0..+0x24): +0x24 bbox ptr, +0x14 run list, bbox
    for a, b, src, cls in cov:
        if cls != 'model_header': continue
        if ingap(b, b + 4):
            bp = u32(d, b)
            add(b, b + 4, 'mesh_header_tail', 'u32 ptr', 'mesh header +0x24: pointer to bbox %08X' % bp,
                'part of the mesh header at %05X (func_8000092C reads hdr+0x24)' % a)
            if is_ptr(bp, size) and ingap(bp - BASE, bp - BASE + 24):
                o = bp - BASE
                add(o, o + 24, 'mesh_bbox', 'f32[6] min xyz, max xyz', 'bounding box of mesh %05X' % a,
                    'mesh hdr+0x24; func_8000092C centres the model on (min+max)/2',
                    list(struct.unpack_from('>6f', d, o)))
        rp = u32(d, a + 0x14)
        if is_ptr(rp, size) and ingap(rp - BASE, rp - BASE + 4):
            o = rp - BASE
            runs, e = runs_decode(d, o)
            add(o, e, 'vertex_runs', 'u16 run list', 'vertex order of animated mesh %05X: %d unique vertices' %
                (a, sum(r['n'] for r in runs)), 'mesh hdr+0x14; walked by func_80000450', runs)
    # 2. collision quad sets {u32 count, ptr -> u32 ptr[count] -> 0x50 type-3 quads}
    for ga, gb in gaps:
        for o in range((ga + 3) & ~3, gb - 7, 4):
            n, p = u32(d, o), u32(d, o + 4)
            if not (1 <= n <= 64 and is_ptr(p, size)): continue
            ao = p - BASE
            if ao + 4 * n > size or not ingap(ao, ao + 4 * n): continue
            qs = [u32(d, ao + 4 * i) for i in range(n)]
            if not all(is_ptr(q, size) and u32(d, q - BASE) == 3 and ingap(q - BASE, q - BASE + 0x50) for q in qs): continue
            add(o, o + 8, 'coll_quad_set', 'u32 count, u32 ptr', '%d quads, array %08X' % (n, p),
                'func_80023A38 `model` (muppet actor ids 0x7004/0x7005/0x7006/0x7016 ...); found by pointer from code')
            add(ao, ao + 4 * n, 'coll_quad_ptrs', 'u32 ptr[count]', 'quad pointers of set %05X' % o, 'coll_quad_set +4', qs)
            for q in qs:
                add(q - BASE, q - BASE + 0x50, 'coll_quad', '0x50 type-3 shape', 'wall quad of set %05X' % o,
                    'coll_quad_ptrs; corners a..d copied/transformed by func_80023A38, test func_80040C00',
                    quad_decode(d, q - BASE))
    # 3. texture-load DLs and their palette/image
    for ga, gb in gaps:
        o = ga
        while o < gb:
            r = parse_texdl(d, o, gb)
            if not r: o += 8; continue
            e, cmds, t = r
            add(o, e, 'texload_dl', 'F3DEX 1.23 GBI', 'loads %d-colour TLUT %08X + CI%d %dx%d %08X' %
                (t['ncolors'], t['palette'], t['bits'], t['width'], t['height'], t['image']),
                'no root found (no code/data/level pointer); not reached by lvlB', {'cmds': cmds, 'tex': t})
            po, io = t['palette'] - BASE, t['image'] - BASE
            if ingap(po, po + 2 * t['ncolors']):
                add(po, po + 2 * t['ncolors'], 'palette', 'TLUT RGBA5551', '%d colours' % t['ncolors'], 'texload_dl %05X' % o)
            isz = t['width'] * t['height'] * t['bits'] // 8
            if ingap(io, io + isz):
                add(io, io + isz, 'texture', 'CI%d %dx%d' % (t['bits'], t['width'], t['height']),
                    'palette %08X' % t['palette'], 'texload_dl %05X' % o, t)
            o = e
    # 4. combiner prologue DLs at offset 0 (lvlB maps the same pair in other levels)
    if ingap(0, 0x30) and d[0:8] == bytes.fromhex('e700000000000000') and d[0x10] == 0xB8 and d[0x28] == 0xB8:
        add(0, 0x18, 'display_list', 'F3DEX 1.23 GBI', 'pipesync; setcombine; enddl (80154000)', 'level-start prologue, as lvlB entry 80154000 in other levels', None)
        add(0x18, 0x30, 'display_list', 'F3DEX 1.23 GBI', 'pipesync; setcombine; enddl (80154018)', 'as lvlB entry 80154018 in other levels', None)
    # 5. vertex morph animations: headers in gaps; length = up to next item / end of non-zero data
    for ga, gb in gaps:
        cands = [o for o in range((ga + 3) & ~3, gb, 4) if anim_header(d, o)
                 and not any(a <= o < v[0] for a, v in items.items())]
        for o in cands:
            nxt = min([a for a in items if a > o and a < gb] + [c for c in cands if c > o] + [gb])
            e = nxt
            while e > o and d[e - 1] == 0: e -= 1
            if d[o:gb].find(GCC) >= 0: e = min(e, o + d[o:gb].find(GCC))
            while e > o and d[e - 1] == 0: e -= 1
            h = anim_header(d, o)
            row = 4 * h['nkey']
            # ceil: the last vertex may end in zero bytes; never past the next item
            nv = min(-(-(e - o - h['data_off']) // row), (nxt - o - h['data_off']) // row)
            end = o + h['data_off'] + nv * 4 * h['nkey']
            if e - end and any(d[end:e]): end = e     # stale tail words belong to the anim
            add(o, end, 'morph_anim', 'vertex morph animation (sprA format)', '%d keys, %d ticks, %d vertices%s' %
                (h['nkey'], h['ticks'], nv, ', looping' if h['flags'] & 0x8000 else ''),
                'func_8000092C/func_80000450 via actor anim table D_800768A0 or mesh hdr+0x28 (func_8003D3D0 obj+0x1BC)', None)
    # 6. GCC .comment leftovers at file end
    for ga, gb in gaps:
        p = d.find(GCC[1:], ga)
        if p > 0 and p < gb:
            s = p - 1
            add(s, gb, 'build_comment', 'ASCII, NUL separated', '%d x "GCC: (GNU) 2.7.2" (ELF .comment of the linked level objects)' %
                d[s:gb].count(b'GCC'), 'none (link leftover, never read)', d[s:gb].decode('latin1'))
    # 7. fill the rest: zero -> pad, else unknown
    out = []
    for ga, gb in gaps:
        pos = ga
        for a in sorted(x for x in items if ga <= x < gb):
            b = items[a][0]
            if a > pos: out += filler(d, pos, a)
            out.append((a, b) + items[a][1:])
            pos = max(pos, b)
        if pos < gb: out += filler(d, pos, gb)
    return out, gaps

def level_refs(game):
    r = {}
    p = os.path.join(SES, 'LEVEL_REFS_%s.tsv' % game)
    for l in open(p).read().splitlines()[1:]:
        f = l.split('\t')
        if f[1].startswith('level_image'): r.setdefault(int(f[0], 16), []).append(f[3])
    return r

def filler(d, a, b):
    if not any(d[a:b]):
        if b - a >= 0x200:
            return [(a, b, 'unknown', 'all zero', '0x%X zero bytes; nothing in the image, program or code points here (same size as a CI8 palette+32x32 CI8 texture slot)' % (b - a), 'none found', None)]
        return [(a, b, 'pad', 'zero', 'alignment / unused', '-', None)]
    note = 'unidentified'
    if b - a == 0x220:
        note = ('unidentified; layout reads as a 16-colour RGBA5551 TLUT (0x20) + 0x200 bytes of 4-bit pixels (CI4 32x32), '
                'but no display list, image pointer or code address reaches it')
    return [(a, b, 'unknown', 'raw', note, 'none found', None)]

# ---------------------------------------------------------------- output
def write_item(od, lvl, d, it):
    a, b, cls, fmt, desc, ref, pl = it
    name = '%s_%05X' % (cls, a)
    path = os.path.join(od, lvl)
    if cls == 'morph_anim':
        h = anim_decode(d, a, b)
        json.dump(h, open(os.path.join(path, name + '.json'), 'w'))
        for k, kf in enumerate(h['keyframes']):
            with open(os.path.join(path, '%s_k%02d_t%03d.obj' % (name, k, h['keys'][k])), 'w') as f:
                f.writelines('v %d %d %d\n' % tuple(v) for v in kf)
    elif cls == 'texture':
        t = pl; po = t['palette'] - 0x80154000
        pal = [u16(d, po + 2 * i) for i in range(t['ncolors'])]
        raw = d[a:b]; W = t['width']
        idx = [x for byte in raw for x in ((byte >> 4, byte & 15) if t['bits'] == 4 else (byte,))]
        png_write(os.path.join(path, name + '.png'), W, t['height'],
                  [bytes(c for i in idx[r * W:(r + 1) * W] for c in rgba5551(pal[i])) for r in range(t['height'])], 6)
        png_write(os.path.join(path, name + '_index.png'), W, t['height'], [bytes(idx[r * W:(r + 1) * W]) for r in range(t['height'])], 0)
        json.dump({'width': t['width'], 'height': t['height'], 'bits': t['bits'], 'palette_offset': po}, open(os.path.join(path, name + '.json'), 'w'))
    elif cls == 'palette':
        json.dump([u16(d, a + 2 * i) for i in range((b - a) // 2)], open(os.path.join(path, name + '.json'), 'w'))
    elif cls in ('texload_dl',):
        open(os.path.join(path, name + '.txt'), 'w').write('\n'.join(pl['cmds']) + '\n')
    elif cls == 'display_list':
        open(os.path.join(path, name + '.txt'), 'w').write('\n'.join('%08X %08X' % (u32(d, o), u32(d, o + 4)) for o in range(a, b, 8)) + '\n')
    elif cls in ('vertex_runs', 'mesh_bbox', 'coll_quad', 'coll_quad_ptrs', 'build_comment'):
        json.dump(pl, open(os.path.join(path, name + '.json'), 'w'))
    elif cls == 'coll_quad_set':
        json.dump(list(struct.unpack_from('>2I', d, a)), open(os.path.join(path, name + '.json'), 'w'))
    elif cls == 'mesh_header_tail':
        json.dump(u32(d, a), open(os.path.join(path, name + '.json'), 'w'))
    elif cls == 'unknown':
        open(os.path.join(path, name + '.bin'), 'wb').write(d[a:b]) if any(d[a:b]) else json.dump({'zero': b - a}, open(os.path.join(path, name + '.json'), 'w'))
    # pad: nothing written (zero bytes, length in MAP)

def read_item(od, lvl, it):
    a, b, cls = it[0], it[1], it[2]
    name = os.path.join(od, lvl, '%s_%05X' % (cls, a))
    J = lambda: json.load(open(name + '.json'))
    if cls == 'pad': return bytes(b - a)
    if cls == 'morph_anim': return anim_encode(J())
    if cls == 'texture':
        j = J(); idx = png_read_grey(name + '_index.png')
        if j['bits'] == 8: return bytes(idx)
        return bytes((idx[i] << 4) | idx[i + 1] for i in range(0, len(idx), 2))
    if cls == 'palette': return struct.pack('>%dH' % (b - a >> 1), *J())
    if cls in ('texload_dl', 'display_list'):
        return b''.join(struct.pack('>2I', *(int(x, 16) for x in l.split())) for l in open(name + '.txt').read().split('\n') if l)
    if cls == 'vertex_runs': return runs_encode(J())
    if cls == 'mesh_bbox': return struct.pack('>6f', *J())
    if cls == 'coll_quad': return quad_encode(J())
    if cls == 'coll_quad_ptrs': j = J(); return struct.pack('>%dI' % len(j), *j)
    if cls == 'coll_quad_set': return struct.pack('>2I', *J())
    if cls == 'mesh_header_tail': return struct.pack('>I', J())
    if cls == 'build_comment': return J().encode('latin1')
    if cls == 'unknown':
        return open(name + '.bin', 'rb').read() if os.path.exists(name + '.bin') else bytes(J()['zero'])
    raise ValueError(cls)

def main():
    rom_path, game, od = sys.argv[1:4]
    mode = sys.argv[4] if len(sys.argv) > 4 else 'extract'
    rom = open(rom_path, 'rb').read()
    rows, bad, tot = [], 0, {}
    for lvl, (ra, rb) in LEVELS[game].items():
        d = inflate(rom, ra, rb)
        items, gaps = classify(game, lvl, d)
        # tiling self-test
        assert sum(i[1] - i[0] for i in items) == sum(b - a for a, b in gaps), lvl
        os.makedirs(os.path.join(od, lvl), exist_ok=True)
        LR = level_refs(game)
        for it in items:
            a, b, cls, fmt, desc, ref, pl = it
            if cls == 'morph_anim':
                ref = '; '.join(LR.get(BASE + a, [])) or 'no reference found in code, program data or the image (unused?)'
                ref += ' | played by func_8000092C/func_80000450'
            tot[cls] = tot.get(cls, 0) + b - a
            ext = '-' if cls == 'pad' else 'tools/extract_level_rest.py -> out/%s/%s_%05X.*' % (lvl, cls, a)
            rows.append('\t'.join([game, '%s.bif:0x%05X' % (lvl, a), '%s.bif:0x%05X' % (lvl, b), cls, fmt, desc, ref, ext]))
            if mode == 'extract': write_item(od, lvl, d, it)
            elif mode == 'check':
                if read_item(od, lvl, it) != d[a:b]:
                    bad += 1; print('MISMATCH', lvl, '%05X' % a, cls)
    if mode in ('extract', 'map'):
        with open(os.path.join(od, 'MAP.tsv'), 'w') as f:
            f.write('game\tstart\tend\tclass\tformat\tdescription\treferenced_by\textractor\n' + '\n'.join(rows) + '\n')
        for k in sorted(tot): print('%-18s %8d' % (k, tot[k]))
    if mode == 'check':
        print('check: %d items, %d mismatches' % (len(rows), bad)); sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
