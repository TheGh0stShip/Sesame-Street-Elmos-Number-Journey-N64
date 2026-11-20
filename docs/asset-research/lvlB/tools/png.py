"""Minimal dependency-free PNG writer/reader (8-bit L, LA, RGBA, P with PLTE/tRNS, 1-bit L)."""
import zlib, struct

class Img:
    def __init__(self, mode, size, px=None, palette=None, trns=None):
        self.mode, self.size = mode, size
        self.px = px if px is not None else [0] * (size[0] * size[1])
        self.palette = palette   # list of (r,g,b)
        self.trns = trns         # bytes of alpha per palette entry
    def putdata(self, px): self.px = list(px)
    def getdata(self): return self.px
    def putpixel(self, xy, v): self.px[xy[1] * self.size[0] + xy[0]] = v
    def save(self, path): open(path, 'wb').write(encode(self))

CH = {'L': 1, 'LA': 2, 'RGBA': 4, 'P': 1, '1': 1}
CT = {'L': 0, 'LA': 4, 'RGBA': 6, 'P': 3, '1': 0}

def _chunk(t, d):
    return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)

def encode(im):
    w, h = im.size
    depth = 1 if im.mode == '1' else 8
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        row = im.px[y * w:(y + 1) * w]
        if im.mode == '1':
            b = bytearray((w + 7) // 8)
            for x, v in enumerate(row):
                if v: b[x >> 3] |= 0x80 >> (x & 7)
            raw += b
        elif CH[im.mode] == 1:
            raw += bytes(row)
        else:
            for p in row: raw += bytes(p)
    out = b'\x89PNG\r\n\x1a\n' + _chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, depth, CT[im.mode], 0, 0, 0))
    if im.mode == 'P':
        out += _chunk(b'PLTE', b''.join(bytes(c[:3]) for c in im.palette))
        if im.trns is not None:
            out += _chunk(b'tRNS', bytes(im.trns))
    return out + _chunk(b'IDAT', zlib.compress(bytes(raw), 9)) + _chunk(b'IEND', b'')

def open_png(path):
    d = open(path, 'rb').read()
    assert d[:8] == b'\x89PNG\r\n\x1a\n'
    p = 8; idat = b''; pal = None; trns = None
    while p < len(d):
        n, t = struct.unpack('>I4s', d[p:p + 8]); c = d[p + 8:p + 8 + n]; p += 12 + n
        if t == b'IHDR': w, h, depth, ct, _, _, il = struct.unpack('>IIBBBBB', c)
        elif t == b'PLTE': pal = [tuple(c[i:i + 3]) for i in range(0, n, 3)]
        elif t == b'tRNS': trns = c
        elif t == b'IDAT': idat += c
    assert il == 0
    mode = {0: 'L', 4: 'LA', 6: 'RGBA', 3: 'P', 2: 'RGB'}[ct]
    if depth == 1: mode = '1'
    bpp = {'L': 1, 'LA': 2, 'RGBA': 4, 'P': 1, 'RGB': 3, '1': 1}[mode]
    stride = (w + 7) // 8 if depth == 1 else w * bpp
    raw = zlib.decompress(idat)
    rows = []; prev = bytearray(stride); q = 0
    for y in range(h):
        f = raw[q]; line = bytearray(raw[q + 1:q + 1 + stride]); q += 1 + stride
        for i in range(stride):
            a = line[i - bpp] if i >= bpp else 0; b = prev[i]; cc = prev[i - bpp] if i >= bpp else 0
            if f == 1: line[i] = (line[i] + a) & 255
            elif f == 2: line[i] = (line[i] + b) & 255
            elif f == 3: line[i] = (line[i] + (a + b) // 2) & 255
            elif f == 4:
                pa, pb, pc = abs(b - cc), abs(a - cc), abs(a + b - 2 * cc)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else cc)
                line[i] = (line[i] + pr) & 255
        rows.append(line); prev = line
    px = []
    for line in rows:
        if mode == '1':
            px += [(line[x >> 3] >> (7 - (x & 7))) & 1 for x in range(w)]
        elif bpp == 1:
            px += list(line)
        else:
            px += [tuple(line[i:i + bpp]) for i in range(0, stride, bpp)]
    return Img(mode, (w, h), px, pal, trns)
