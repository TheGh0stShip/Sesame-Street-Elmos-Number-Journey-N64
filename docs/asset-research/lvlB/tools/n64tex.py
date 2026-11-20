"""N64 texel codecs: decode to png.Img images and encode back to the exact texel bytes (lossless)."""
import os as _os, sys as _sys
_sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from png import Img

FMT = {0: 'RGBA', 1: 'YUV', 2: 'CI', 3: 'IA', 4: 'I'}
SIZ = {0: 4, 1: 8, 2: 16, 3: 32}

def nbytes(fmt, bits, w, h):
    return (w * h * bits + 7) // 8

def rgba16(v):
    r = (v >> 11) & 31; g = (v >> 6) & 31; b = (v >> 1) & 31
    return ((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 255 if v & 1 else 0)

def to_rgba16(p):
    r, g, b, a = p
    return ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | (1 if a >= 128 else 0)

def palette_colors(pal_bytes):
    return [rgba16((pal_bytes[i] << 8) | pal_bytes[i + 1]) for i in range(0, len(pal_bytes) - 1, 2)]

def _pix(data, bits, w, h):
    out = []
    if bits == 4:
        for i in range(w * h):
            b = data[i >> 1]
            out.append((b >> 4) if not (i & 1) else (b & 15))
    elif bits == 8:
        out = list(data[:w * h])
    elif bits == 16:
        out = [(data[2 * i] << 8) | data[2 * i + 1] for i in range(w * h)]
    elif bits == 32:
        out = [int.from_bytes(data[4 * i:4 * i + 4], 'big') for i in range(w * h)]
    return out

def _pack(vals, bits):
    if bits == 4:
        b = bytearray((len(vals) + 1) // 2)
        for i, v in enumerate(vals):
            b[i >> 1] |= (v & 15) << (0 if i & 1 else 4)
        return bytes(b)
    if bits == 8:
        return bytes(vals)
    if bits == 16:
        return b''.join(v.to_bytes(2, 'big') for v in vals)
    return b''.join(v.to_bytes(4, 'big') for v in vals)

def decode(fmt, bits, w, h, data, pal=None):
    """fmt: 'RGBA','CI','IA','I'. pal: bytes of RGBA16 TLUT (CI only). Returns png.Img."""
    px = _pix(data, bits, w, h)
    if fmt == 'CI':
        n = 16 if bits == 4 else 256
        cols = palette_colors(pal) if pal else [(i * 255 // (n - 1),) * 3 + (255,) for i in range(n)]
        cols = (cols + [(0, 0, 0, 255)] * n)[:n]
        return Img('P', (w, h), px, [c[:3] for c in cols], bytes(c[3] for c in cols))
    if fmt == 'RGBA' and bits == 16:
        im = Img('RGBA', (w, h)); im.putdata([rgba16(v) for v in px]); return im
    if fmt == 'RGBA' and bits == 32:
        im = Img('RGBA', (w, h)); im.putdata([((v >> 24) & 255, (v >> 16) & 255, (v >> 8) & 255, v & 255) for v in px]); return im
    if fmt == 'YUV' and bits == 16:
        # texel pairs U Y0 V Y1; stored losslessly as an RGBA PNG with R=Y, G=U, B=V, A=255 (not a colour image)
        out = []
        for i in range(0, w * h, 2):
            u, y0, v, y1 = data[2 * i:2 * i + 4]
            out += [(y0, u, v, 255), (y1, u, v, 255)]
        im = Img('RGBA', (w, h)); im.putdata(out[:w * h]); return im
    if fmt == 'IA':
        if bits == 4:
            f = lambda v: (((v >> 1) * 255) // 7, 255 if v & 1 else 0)
        elif bits == 8:
            f = lambda v: ((v >> 4) * 17, (v & 15) * 17)
        else:
            f = lambda v: (v >> 8, v & 255)
        im = Img('LA', (w, h)); im.putdata([f(v) for v in px]); return im
    if fmt == 'I':
        f = (lambda v: v * 17) if bits == 4 else (lambda v: v)
        im = Img('L', (w, h)); im.putdata([f(v) for v in px]); return im
    raise ValueError('unsupported %s%d' % (fmt, bits))

def encode(fmt, bits, im):
    """inverse of decode: returns (texel bytes, tlut bytes or None)."""
    w, h = im.size
    if fmt == 'CI':
        px = list(im.getdata())
        n = 16 if bits == 4 else 256
        pal = im.palette
        tr = im.trns if im.trns is not None else bytes([255] * n)
        tl = b''.join(to_rgba16(tuple(pal[i]) + (tr[i] if i < len(tr) else 255,)).to_bytes(2, 'big') for i in range(n))
        return _pack(px, bits), tl
    px = list(im.getdata())
    if fmt == 'RGBA' and bits == 16:
        return _pack([to_rgba16(p) for p in px], 16), None
    if fmt == 'RGBA' and bits == 32:
        return _pack([(p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3] for p in px], 32), None
    if fmt == 'YUV' and bits == 16:
        b = bytearray()
        for i in range(0, len(px), 2):
            a, c = px[i], px[i + 1]
            b += bytes((a[1], a[0], a[2], c[0]))
        return bytes(b), None
    if fmt == 'IA':
        if bits == 4:
            g = lambda p: (((p[0] * 7 + 127) // 255) << 1) | (1 if p[1] >= 128 else 0)
        elif bits == 8:
            g = lambda p: ((p[0] // 17) << 4) | (p[1] // 17)
        else:
            g = lambda p: (p[0] << 8) | p[1]
        return _pack([g(p) for p in px], bits), None
    if fmt == 'I':
        g = (lambda v: v // 17) if bits == 4 else (lambda v: v)
        return _pack([g(v) for v in px], bits), None
    raise ValueError(fmt)


# every texel format the RDP can sample
FORMATS = [('RGBA', 16), ('RGBA', 32), ('YUV', 16), ('CI', 4), ('CI', 8), ('IA', 4), ('IA', 8), ('IA', 16), ('I', 4), ('I', 8)]


def selftest(seed=1):
    """random texels (and TLUTs) for every format: decode -> PNG bytes -> parse -> encode must give the same bytes"""
    import random, os, tempfile, png
    r = random.Random(seed)
    for fmt, bits in FORMATS:
        for w, h in ((8, 4), (32, 32), (16, 1)):
            data = bytes(r.getrandbits(8) for _ in range(nbytes(fmt, bits, w, h)))
            pal = bytes(r.getrandbits(8) for _ in range(2 * (16 if bits == 4 else 256))) if fmt == 'CI' else None
            if fmt == 'YUV' and w % 2:
                continue
            im = decode(fmt, bits, w, h, data, pal)
            fd, path = tempfile.mkstemp(suffix='.png'); os.close(fd)
            try:
                im.save(path); back = png.open_png(path)
            finally:
                os.remove(path)
            t, tl = encode(fmt, bits, back)
            assert t == data, (fmt, bits, w, h)
            if pal: assert tl == pal, (fmt, bits, 'tlut')
    return True


if __name__ == '__main__':
    selftest(); print('n64tex selftest ok')
