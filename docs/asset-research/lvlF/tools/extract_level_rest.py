#!/usr/bin/env python3
"""extract_level_rest.py ROM GAME OUTDIR [check]

Dumps the level-image ranges listed in ../MAP.tsv (agent lvlF: the bytes of the gzip .bif level images that
neither the graphics (lvlB) nor the object/BSP (lvlC) decoders cover) as PNG / JSON, and with `check`
re-serialises every range from those output files and compares with the inflated bytes.
The ROM is only read; level files are located by ../../files/FILES_<GAME>.tsv (start/end ROM offsets) and
inflated here (gzip member, FNAME flag).  No ROM content is stored in this tool or in MAP.tsv.
"""
import sys, os, json, struct, zlib, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
BASE = 0x80154000


def load_levels(rom, game):
    files = {}
    for l in open(os.path.join(ROOT, '..', 'files', 'FILES_%s.tsv' % game)):
        p = l.rstrip('\n').split('\t')
        if len(p) > 5 and p[4] == 'gzip_level':
            name = p[5].split(':')[0]
            files[name] = (int(p[1], 16), int(p[2], 16))
    out = {}
    for name, (s, e) in files.items():
        g = rom[s:e]
        assert g[:3] == b'\x1f\x8b\x08' and g[3] == 0x08
        i = g.index(b'\0', 10) + 1
        d = zlib.decompressobj(-15)
        out[name] = d.decompress(g[i:])
    return out


def rows(game):
    r = []
    for l in open(os.path.join(ROOT, 'MAP.tsv')):
        p = l.rstrip('\n').split('\t')
        if p[0] != game:
            continue
        f, s = p[1].split(':'); e = p[2].split(':')[1]
        r.append(dict(file=f, s=int(s, 16), e=int(e, 16), cls=p[3], fmt=p[4], desc=p[5], ref=p[6]))
    return r


# ---- palettes / CI textures -------------------------------------------------------------------------------
def pal_to_png(pb):
    n = len(pb) // 2
    rgb, alpha = [], []
    for i in range(n):
        v = struct.unpack('>H', pb[2 * i:2 * i + 2])[0]
        for sh in (11, 6, 1):
            c = (v >> sh) & 31
            rgb.append((c << 3) | (c >> 2))
        alpha.append(255 if v & 1 else 0)
    return rgb, alpha


def png_to_pal(rgb, alpha, n):
    out = b''
    for i in range(n):
        r, g, b = (rgb[3 * i + k] >> 3 for k in range(3))
        out += struct.pack('>H', (r << 11) | (g << 6) | (b << 1) | (1 if alpha[i] else 0))
    return out


def tex_dims(fmt):
    bits = 4 if 'CI4' in fmt else 8
    w, h = (int(x) for x in fmt.split()[1].split('x'))
    return bits, w, h


def _chunk(t, data):
    return struct.pack('>I', len(data)) + t + data + struct.pack('>I', zlib.crc32(t + data) & 0xffffffff)


def write_png(path, pb, tb, fmt):
    """8-bit paletted PNG (colour type 3), palette = RGBA5551 expanded, tRNS = alpha bit."""
    bits, w, h = tex_dims(fmt)
    idx = bytes(x for b in tb for x in (b >> 4, b & 15)) if bits == 4 else bytes(tb)
    assert len(idx) == w * h, (path, len(idx), w, h)
    rgb, alpha = pal_to_png(pb)
    raw = b''.join(b'\0' + idx[y * w:(y + 1) * w] for y in range(h))
    png = (b'\x89PNG\r\n\x1a\n' + _chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 3, 0, 0, 0)) +
           _chunk(b'PLTE', bytes(rgb)) + _chunk(b'tRNS', bytes(alpha)) +
           _chunk(b'IDAT', zlib.compress(raw, 9)) + _chunk(b'IEND', b''))
    open(path, 'wb').write(png)


def read_png(path, npal):
    b = open(path, 'rb').read()
    assert b[:8] == b'\x89PNG\r\n\x1a\n'
    q, ch = 8, collections.defaultdict(bytes)
    while q < len(b):
        n = struct.unpack('>I', b[q:q + 4])[0]; t = b[q + 4:q + 8]
        ch[t] += b[q + 8:q + 8 + n]; q += 12 + n
    w, h, bd, ct = struct.unpack('>IIBB', ch[b'IHDR'][:10])
    assert bd == 8 and ct == 3
    raw = zlib.decompress(ch[b'IDAT'])
    idx = b''
    for y in range(h):
        row = raw[y * (w + 1):(y + 1) * (w + 1)]
        assert row[0] == 0, 'only filter 0 supported'
        idx += row[1:]
    alpha = list(ch[b'tRNS']) + [255] * (npal - len(ch[b'tRNS']))
    return list(ch[b'PLTE']), alpha, idx


# ---- morph animation (same format as sprA) ----------------------------------------------------------------
def s(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def anim_dec(b):
    he, do, ticks, nk, fl = struct.unpack('>HHBBH', b[:8])
    keys = list(b[8:8 + nk])
    nv = (len(b) - do) // (4 * nk)
    verts = []
    for i in range(nv):
        row = []
        for k in range(nk):
            w = struct.unpack('>I', b[do + 4 * (i * nk + k):do + 4 * (i * nk + k) + 4])[0]
            row.append([s(w & 0x7FF, 11), s((w >> 11) & 0x7FF, 11), s(w >> 22, 10)])
        verts.append(row)
    return dict(hdr_end=he, data_off=do, ticks=ticks, flags=fl, keys=keys,
                hdr_pad=b[8 + nk:do].hex(), nvert=nv, verts_vertex_major=verts,
                tail=b[do + nv * nk * 4:].hex())


def anim_enc(j):
    nk = len(j['keys'])
    b = struct.pack('>HHBBH', j['hdr_end'], j['data_off'], j['ticks'], nk, j['flags']) + bytes(j['keys'])
    b += bytes.fromhex(j['hdr_pad'])
    for row in j['verts_vertex_major']:
        for x, y, z in row:
            b += struct.pack('>I', ((z & 0x3FF) << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF))
    return b + bytes.fromhex(j['tail'])


def vorder_dec(b):
    g, q = [], 0
    while True:
        n = struct.unpack('>H', b[q:q + 2])[0]; q += 2
        if n == 0:
            g.append({'n': 0}); break
        m = struct.unpack('>H', b[q:q + 2])[0]; q += 2
        if m == 0:
            g.append({'n': n, 'm': 0}); break
        g.append({'n': n, 'dup': list(struct.unpack('>%dH' % m, b[q:q + 2 * m]))}); q += 2 * m
    assert q == len(b)
    return {'groups': g}


def vorder_enc(j):
    b = b''
    for x in j['groups']:
        b += struct.pack('>H', x['n'])
        if x['n'] == 0:
            break
        if 'm' in x:
            b += struct.pack('>H', 0); break
        b += struct.pack('>H', len(x['dup'])) + struct.pack('>%dH' % len(x['dup']), *x['dup'])
    return b


def f32s(b):
    return [struct.unpack('>f', b[i:i + 4])[0] for i in range(0, len(b), 4)]


def pf32(v):
    return b''.join(struct.pack('>f', x) for x in v)


def quad_dec(b):
    out = []
    for q in range(0, len(b), 0x50):
        r = b[q:q + 0x50]
        t = struct.unpack('>4I', r[:16])
        fl = f32s(r[16:])
        out.append(dict(type=t[0], w4=t[1], mask=t[2], wC=t[3], normal=fl[0:3], dist=fl[3],
                        corners=[fl[4 + 3 * i:7 + 3 * i] for i in range(4)]))
    return out


def quad_enc(j):
    b = b''
    for q in j:
        b += struct.pack('>4I', q['type'], q['w4'], q['mask'], q['wC']) + pf32(q['normal'] + [q['dist']] + sum(q['corners'], []))
    return b


def words(b):
    return ['0x%08X' % w for w in struct.unpack('>%dI' % (len(b) // 4), b)]


def unwords(j):
    return b''.join(struct.pack('>I', int(w, 16)) for w in j)


def main():
    rom = open(sys.argv[1], 'rb').read()
    game, outdir = sys.argv[2], sys.argv[3]
    check = len(sys.argv) > 4 and sys.argv[4] == 'check'
    lv = load_levels(rom, game)
    od = os.path.join(outdir, game)
    os.makedirs(od, exist_ok=True)
    R = rows(game)
    bad = 0
    stats = collections.Counter()
    lastpal = {}
    for r in R:
        f = r['file']; d = lv[f]; b = d[r['s']:r['e']]
        cls = r['cls']; stem = os.path.join(od, '%s_%05X_%s' % (f.replace('.bif', ''), r['s'], cls))
        if cls in ('hud_palette', 'texframe_palette', 'unused_palette'):
            lastpal[f] = b
            rb = None  # serialised with its texture (PNG palette); checked there
            if not check:
                pass
            stats[cls] += len(b)
            pend = (r, b)
            continue
        if cls in ('hud_texture', 'texframe_texture', 'unused_texture'):
            bits, w, h = tex_dims(r['fmt'])
            pb = lastpal[f]
            own = pend[0]['e'] == r['s']
            n = r['e'] - r['s']
            if n != w * h * bits // 8:
                # identical labels merged by MAP (consecutive textures of the same class): split by size
                pass
            per = w * h * bits // 8
            for k in range(0, n, per):
                tb = b[k:k + per]
                path = '%s_%02d.png' % (stem, k // per) if n > per else stem + '.png'
                if not check:
                    write_png(path, pb, tb, r['fmt'])
                rgb, alpha, idx = read_png(path, len(pb) // 2)
                tb2 = bytes((idx[2 * i] << 4) | idx[2 * i + 1] for i in range(len(idx) // 2)) if bits == 4 else idx
                pb2 = png_to_pal(rgb, alpha, len(pb) // 2)
                if tb2 != tb or (own and k == 0 and pb2 != pb):
                    print('MISMATCH', path); bad += 1
            stats[cls] += n
            continue
        if cls == 'pad':
            if any(b):
                print('MISMATCH pad', f, hex(r['s'])); bad += 1
            stats[cls] += len(b); continue
        if cls == 'morph_anim':
            enc, dec = anim_enc, anim_dec
        elif cls == 'morph_vertex_order':
            enc, dec = vorder_enc, vorder_dec
        elif cls == 'model_bounds':
            dec = lambda b: [f32s(b[i:i + 24]) for i in range(0, len(b), 24)]
            enc = lambda j: b''.join(pf32(x) for x in j)
        elif cls == 'coll_quad':
            enc, dec = quad_enc, quad_dec
        elif cls in ('model_header_ext', 'coll_quad_ptrs', 'coll_quad_set', 'display_list'):
            enc, dec = unwords, words
        else:
            print('no decoder for', cls); bad += 1; continue
        path = stem + '.json'
        if not check:
            json.dump(dict(file=f, start=r['s'], end=r['e'], cls=cls, ref=r['ref'], data=dec(b)), open(path, 'w'))
        j = json.load(open(path))
        if enc(j['data']) != b:
            print('MISMATCH', path); bad += 1
        stats[cls] += len(b)
    for k, v in sorted(stats.items()):
        print('%-20s %8d' % (k, v))
    print('total %d bytes, %s' % (sum(stats.values()), 'check OK' if bad == 0 else '%d MISMATCHES' % bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
