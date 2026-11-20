#!/usr/bin/env python3
"""data_tex.py: the program image's textures and RSP microcode blobs (data/extract.tsv), no ROM content inside.

  data_tex.py extract ROM SHA1FILE TABLE OUTDIR   check the ROM's SHA-1 against SHA1FILE, then write each table
                                                  row: textures as PNG, microcode as .bin (raw RSP words)
  data_tex.py inc PNG FMT WIDTH HEIGHT OUT        write the C initialiser elements of a texture (for #include)

Table columns: name kind rom_offset size fmt width height. Formats (texel order row-major, big-endian):
  rgba16  16-bit RGBA5551 -> PNG RGBA (5-bit channels expanded v<<3|v>>2, alpha 0/255); u16 elements
  i4      4-bit intensity, high nibble first -> PNG grey (v*17); u8 elements
  i8      8-bit intensity -> PNG grey; u8 elements
  i1      1 bit per texel, MSB = leftmost -> PNG grey (0/255); u8 elements
The PNG reader accepts 8-bit grey, grey+alpha, RGB and RGBA, any filter, so edited images can be re-saved."""
import sys, zlib, struct, hashlib, os

def png_write(path, w, h, rows, color):            # color 0 = grey, 6 = RGBA; rows: list of bytes
    raw = b''.join(b'\0' + r for r in rows)
    def chunk(t, d): return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d) & 0xFFFFFFFF)
    open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, color, 0, 0, 0))
                           + chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))

def png_read(path):                                  # -> w, h, list of RGBA tuples per row
    b = open(path, 'rb').read(); assert b[:8] == b'\x89PNG\r\n\x1a\n', path
    p = 8; idat = b''
    while p < len(b):
        n, = struct.unpack('>I', b[p:p + 4]); t = b[p + 4:p + 8]; d = b[p + 8:p + 8 + n]; p += 12 + n
        if t == b'IHDR': w, h, depth, color, _, _, inter = struct.unpack('>IIBBBBB', d)
        elif t == b'IDAT': idat += d
    assert depth == 8 and inter == 0 and color in (0, 2, 4, 6), 'only 8-bit non-interlaced grey/RGB(A) PNG'
    bpp = {0: 1, 2: 3, 4: 2, 6: 4}[color]; raw = zlib.decompress(idat); stride = w * bpp
    rows = []; prev = bytearray(stride); q = 0
    for y in range(h):
        f = raw[q]; cur = bytearray(raw[q + 1:q + 1 + stride]); q += 1 + stride
        for i in range(stride):
            a = cur[i - bpp] if i >= bpp else 0; up = prev[i]; c = prev[i - bpp] if i >= bpp else 0
            if f == 1: cur[i] = (cur[i] + a) & 255
            elif f == 2: cur[i] = (cur[i] + up) & 255
            elif f == 3: cur[i] = (cur[i] + (a + up) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(up - c), abs(a - c), abs(a + up - 2 * c)
                cur[i] = (cur[i] + (a if pa <= pb and pa <= pc else up if pb <= pc else c)) & 255
        prev = cur
        px = []
        for x in range(w):
            v = cur[x * bpp:(x + 1) * bpp]
            px.append((v[0], v[0], v[0], 255) if color == 0 else (v[0], v[0], v[0], v[1]) if color == 4 else (v[0], v[1], v[2], 255) if color == 2 else tuple(v))
        rows.append(px)
    return w, h, rows

def decode(fmt, data, w, h):                         # texels -> (color, rows)
    if fmt == 'rgba16':
        rows = []
        for y in range(h):
            r = bytearray()
            for x in range(w):
                v, = struct.unpack_from('>H', data, (y * w + x) * 2)
                for c in (v >> 11, (v >> 6) & 31, (v >> 1) & 31): r.append((c << 3) | (c >> 2))
                r.append(255 if v & 1 else 0)
            rows.append(bytes(r))
        return 6, rows
    if fmt == 'i8': return 0, [bytes(data[y * w:(y + 1) * w]) for y in range(h)]
    if fmt == 'i4': return 0, [bytes((data[(y * w + x) // 2] >> (4 if x % 2 == 0 else 0) & 15) * 17 for x in range(w)) for y in range(h)]
    if fmt == 'i1': return 0, [bytes(255 if data[(y * w + x) // 8] >> (7 - x % 8) & 1 else 0 for x in range(w)) for y in range(h)]
    raise SystemExit('unknown format ' + fmt)

def encode(fmt, w, h, rows):                         # PNG pixels -> list of element values
    px = [p for r in rows for p in r]; assert len(px) == w * h
    if fmt == 'rgba16': return [(r >> 3) << 11 | (g >> 3) << 6 | (b >> 3) << 1 | (1 if a >= 128 else 0) for r, g, b, a in px]
    if fmt == 'i8': return [p[0] for p in px]
    if fmt == 'i4': return [((px[i][0] + 8) // 17) << 4 | (px[i + 1][0] + 8) // 17 for i in range(0, len(px), 2)]
    if fmt == 'i1': return [sum((1 << (7 - k)) for k in range(8) if px[i + k][0] >= 128) for i in range(0, len(px), 8)]
    raise SystemExit('unknown format ' + fmt)

def main(a):
    if a[0] == 'extract':
        rom = open(a[1], 'rb').read(); want = open(a[2]).read().split()[0]
        if hashlib.sha1(rom).hexdigest() != want: raise SystemExit('%s: SHA-1 is not %s (wrong ROM?)' % (a[1], want))
        os.makedirs(a[4], exist_ok=True); n = 0
        for l in open(a[3]):
            if l.startswith('#') or not l.strip(): continue
            name, kind, off, size, fmt, w, h = l.split()
            d = rom[int(off, 16):int(off, 16) + int(size, 16)]
            if kind == 'bin': open(os.path.join(a[4], name + '.bin'), 'wb').write(d)
            else:
                color, rows = decode(fmt, d, int(w), int(h)); png_write(os.path.join(a[4], name + '.png'), int(w), int(h), rows, color)
            n += 1
        print('extracted %d files to %s' % (n, a[4]))
    elif a[0] == 'inc':
        w, h, rows = png_read(a[1]); assert (w, h) == (int(a[3]), int(a[4])), 'size %dx%d expected' % (int(a[3]), int(a[4]))
        v = encode(a[2], w, h, rows); f = '0x%04X' if a[2] == 'rgba16' else '0x%02X'
        open(a[5], 'w').write(''.join(', '.join(f % x for x in v[i:i + 16]) + ',\n' for i in range(0, len(v), 16)))
    else: raise SystemExit(__doc__)

if __name__ == '__main__': main(sys.argv[1:])
