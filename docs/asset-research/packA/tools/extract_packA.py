#!/usr/bin/env python3
"""Extract / verify the mode-22 menu sprite pack (packA).

Usage:
  extract_packA.py ROM GAME OUTDIR          extract to OUTDIR (PNG + palette JSON + raw .bin for unknowns)
  extract_packA.py check ROM GAME OUTDIR    rebuild the file bytes from OUTDIR and compare with the ROM
GAME is 'letter' or 'number'.

The file is a raw RAM image copied verbatim to 0x80154000 by the level loader
(Letter func_8001D9D0 case 22 / Number func_8001E2CC case 22, via func_80017318 / func_80017788).
Every object is: TLUT (RGBA5551, big-endian u16) immediately followed by colour-indexed pixels
(row-major, CI4 = two pixels per byte, high nibble first). No header, no compression.
"""
import sys, os, json, struct
import zlib

# Minimal indexed-PNG writer/reader (no third-party modules; the lab venv has no PIL).
def png_write(path, w, h, idx, rgba):
    def chunk(t, b):
        return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xffffffff)
    raw = b''.join(b'\0' + idx[y * w:(y + 1) * w] for y in range(h))
    data = (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 3, 0, 0, 0)) +
            chunk(b'PLTE', bytes(c for p in rgba for c in p[:3])) +
            chunk(b'tRNS', bytes(p[3] for p in rgba)) + chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))
    open(path, 'wb').write(data)


def png_read_indices(path):
    d = open(path, 'rb').read()
    assert d[:8] == b'\x89PNG\r\n\x1a\n'
    p, idat = 8, b''
    while p < len(d):
        n, t = struct.unpack('>I4s', d[p:p + 8])
        b = d[p + 8:p + 8 + n]
        if t == b'IHDR':
            w, h, bd, ct = struct.unpack('>IIBB', b[:10])
            assert bd == 8 and ct == 3, 'expects 8-bit indexed PNG'
        elif t == b'IDAT':
            idat += b
        p += 12 + n
    raw, out, prev = zlib.decompress(idat), bytearray(), bytearray(w)
    for y in range(h):
        f, line = raw[y * (w + 1)], bytearray(raw[y * (w + 1) + 1:(y + 1) * (w + 1)])
        for x in range(w):
            a = line[x - 1] if x else 0; b = prev[x]; c = prev[x - 1] if x else 0
            if f == 1: line[x] = (line[x] + a) & 255
            elif f == 2: line[x] = (line[x] + b) & 255
            elif f == 3: line[x] = (line[x] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        out += line; prev = line
    return bytes(out)

FILES = {'letter': (0x7C6B0, 0xA54D0), 'number': (0x7CB20, 0xA5940)}
RAM = 0x80154000


def layout():
    """(offset, name, kind, pal_entries, w, h, ref) ; kind 'ci4'/'ci8'/'raw'."""
    L = [(0x0, 'sprite_unref_00', 'ci4', 16, 64, 64,
          'not referenced: sits just before the anim tables (table index 0 starts at +0x820)')]
    groups = [('animA', 22, 'D_80074664[i] (Letter) / twin table (Number), drawn when D_800745E0==0'),
              ('animB', 28, 'D_80074714[i], drawn when D_800745E0==1 and default'),
              ('animC', 26, 'D_800747F4[i], drawn when D_800745E0==2')]
    off = 0x820
    for g, n, ref in groups:
        for i in range(n):
            L.append((off, '%s_%02d' % (g, i), 'ci4', 16, 64, 64, ref + '; func_8001A318'))
            off += 0x820
    assert off == 0x271A0
    L.append((0x271A0, 'icon_ci8_unref', 'ci8', 256, 32, 32, 'not referenced by any code or table found (twin of icon_ci8)'))
    L.append((0x277A0, 'icon_ci8', 'ci8', 256, 32, 32, 'D_8017B7A0 (TLUT) / D_8017B9A0 (image); func_8001A318, drawn at 3 positions'))
    off = 0x27DA0
    for i in range(3):
        L.append((off, 'button_%d' % i, 'ci4', 16, 64, 32, 'D_800748C4[%d]; func_8001A318' % i))
        off += 0x420
    L.append((0x28A00, 'banner', 'ci4', 16, 64, 32, 'D_8017CA00 (TLUT) / D_8017CA20 (image); func_8001A318'))
    return L


def size_of(k, pe, w, h):
    if k == 'raw':
        return w
    return pe * 2 + (w * h // 2 if k == 'ci4' else w * h)


def c5551(v):
    r, g, b, a = (v >> 11) & 31, (v >> 6) & 31, (v >> 1) & 31, v & 1
    return (r * 255 // 31, g * 255 // 31, b * 255 // 31, 255 * a)


def read_file(rom, game):
    s, e = FILES[game]
    with open(rom, 'rb') as f:
        f.seek(s)
        return f.read(e - s)


def extract(rom, game, out):
    d = read_file(rom, game)
    os.makedirs(out, exist_ok=True)
    meta = []
    for off, name, k, pe, w, h, ref in layout():
        n = size_of(k, pe, w, h)
        blob = d[off:off + n]
        ent = dict(name=name, offset=off, size=n, kind=k, ram=hex(RAM + off), ref=ref)
        if k == 'raw':
            open(os.path.join(out, name + '.bin'), 'wb').write(blob)
        else:
            pal = list(struct.unpack('>%dH' % pe, blob[:pe * 2]))
            px = blob[pe * 2:]
            if k == 'ci4':
                idx = bytes(v for b in px for v in (b >> 4, b & 15))
            else:
                idx = px
            png_write(os.path.join(out, name + '.png'), w, h, idx, [c5551(v) for v in pal])
            ent.update(width=w, height=h, tlut_rgba5551=['%04X' % v for v in pal])
        meta.append(ent)
    json.dump(dict(game=game, rom_range=[hex(x) for x in FILES[game]], objects=meta),
              open(os.path.join(out, 'packA.json'), 'w'), indent=1)
    print('extracted %d objects to %s' % (len(meta), out))


def rebuild(out):
    m = json.load(open(os.path.join(out, 'packA.json')))
    buf = bytearray()
    for o in m['objects']:
        assert len(buf) == o['offset'], o['name']
        if o['kind'] == 'raw':
            buf += open(os.path.join(out, o['name'] + '.bin'), 'rb').read()
            continue
        buf += b''.join(struct.pack('>H', int(v, 16)) for v in o['tlut_rgba5551'])
        idx = png_read_indices(os.path.join(out, o['name'] + '.png'))
        if o['kind'] == 'ci4':
            buf += bytes((idx[i] << 4) | idx[i + 1] for i in range(0, len(idx), 2))
        else:
            buf += idx
    return bytes(buf)


def main(a):
    if a and a[0] == 'check':
        rom, game, out = a[1:4]
        orig, new = read_file(rom, game), rebuild(out)
        ok = orig == new
        print('%s: %d bytes, rebuilt %d bytes: %s' % (game, len(orig), len(new), 'MATCH' if ok else 'MISMATCH'))
        sys.exit(0 if ok else 1)
    rom, game, out = a[:3]
    extract(rom, game, out)


if __name__ == '__main__':
    main(sys.argv[1:])
