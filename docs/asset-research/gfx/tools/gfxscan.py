"""Find display lists in every RAM image of a game and trace them for textures and vertices."""
import struct
import blobs, f3dex

class Space:
    def __init__(self, imgs, level):
        self.imgs = imgs  # name -> (base, data)
        self.level = level
    def where(self, a, n=1):
        for name in ('main', 'common', self.level):
            if name is None: continue
            b, d = self.imgs[name]
            if b <= a and a + n <= b + len(d):
                return name, a - b
        return None
    def read(self, a, n):
        w = self.where(a, n)
        if not w: return None
        b, d = self.imgs[w[0]]
        return d[w[1]:w[1] + n]

def find_dls(data, base):
    """returns list of (start_off, end_off_exclusive) of command runs that end with G_ENDDL."""
    out = []
    n = len(data) // 8 * 8
    lastend = 0
    for o in range(0, n, 8):
        if data[o:o + 8] != b'\xB8\0\0\0\0\0\0\0':
            continue
        s = o
        while s - 8 >= lastend:
            w0, w1 = struct.unpack_from('>II', data, s - 8)
            if not f3dex.plausible(w0, w1) or (w0 >> 24) == 0xB8:
                break
            s -= 8
        if s < o or True:
            out.append((s, o + 8))
        lastend = o + 8
    return out

def load(rom, game):
    imgs = {}; meta = {}
    for name, base, data, s, e, comp, extra in blobs.images(rom, game):
        imgs[name] = (base, data); meta[name] = (s, e, comp, extra)
    return imgs, meta
