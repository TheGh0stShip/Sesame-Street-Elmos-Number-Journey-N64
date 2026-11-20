"""evidence.py: byte-level evidence used by resolve.py (no ROM bytes are written anywhere).

Spaces: 'ROM' (the 8 MB ROM) and '<name>.bif' (an inflated level image, RAM base 0x80154000).
RAM -> space mapping: level image 0x80154000+; common block (raw ROM file PI-copied to 0x8024B000);
main program image (RAM = ROM + 0x7FFFF400).
"""
import os, sys, struct, glob, collections

LAB = os.path.expanduser('${N64DK_LAB}'); S = LAB + '/sesassets'
sys.path.insert(0, S + '/gfx/tools')
import f3dex  # noqa: E402  (gfx's F3DEX 1.23 decoder: opcode set, disassembler)

LEVEL_BASE = 0x80154000
COMMON = {'letter': (0x10D0F0, 0x1BF6F0), 'number': (0x10D560, 0x177680)}    # files/FILES_<game>.tsv id 1
PROG = {'letter': (0x1000, 0x7C6B0), 'number': (0x1000, 0x7CB20)}            # main segment incl. .rodata (ROM, ASSET_FACTS)
ROMS = {'letter': LAB + "/roms/Sesame Street - Elmo's Letter Adventure (USA).z64",
        'number': LAB + "/roms/Sesame Street - Elmo's Number Journey (USA).z64"}
ENDDL = b'\xB8\x00\x00\x00\x00\x00\x00\x00'


class Game:
    def __init__(self, game):
        self.game = game
        self.rom = open(ROMS[game], 'rb').read()
        self.img = {}
        for p in sorted(glob.glob(S + '/lvlA/out/%s/*.bif.bin' % game)):
            self.img[os.path.basename(p)[:-4].lower()] = open(p, 'rb').read()
        self.dl_cache = {}

    def data(self, space):
        return self.rom if space == 'ROM' else self.img[space]

    def ram_base(self, space, off):
        """RAM address of a space offset (None if the bytes are not in a RAM image)."""
        if space != 'ROM': return LEVEL_BASE + off
        c0, c1 = COMMON[self.game]
        if c0 <= off < c1: return 0x8024B000 + off - c0
        p0, p1 = PROG[self.game]
        if p0 <= off < p1: return off + 0x7FFFF400
        return None

    def loc(self, ram, space):
        """RAM address -> (space, offset) as seen from code running with level image `space` loaded."""
        if space != 'ROM' and LEVEL_BASE <= ram < LEVEL_BASE + len(self.img[space]):
            return space, ram - LEVEL_BASE
        if 0x8024B000 <= ram < 0x8024B000 + COMMON[self.game][1] - COMMON[self.game][0]:
            return 'ROM', ram - 0x8024B000 + COMMON[self.game][0]
        r = ram - 0x7FFFF400
        if PROG[self.game][0] <= r < PROG[self.game][1]:
            return 'ROM', r
        return None

    # ---------------- display lists
    def dl(self, space, off, limit=6000):
        """Linear scan of one display list from `off` until its G_ENDDL (or a branch). Returns dict(start, end, ok,
        vtx=[(raw_addr, len)], calls=[raw_addr], timg=[raw_addr], segs=[(seg, addr)]). ok=False if a word pair that
        is not an F3DEX 1.23 command comes before the G_ENDDL. Addresses are raw (segmented or RAM)."""
        k = (space, off)
        if k in self.dl_cache: return self.dl_cache[k]
        d = self.data(space); pc = off
        res = dict(start=off, end=None, ok=False, vtx=[], calls=[], timg=[], segs=[], ncmd=0, tload=[])
        cur = None   # last G_SETTIMG: (raw addr, siz, width)
        if off % 8:
            self.dl_cache[k] = res; return res
        for _ in range(limit):
            if pc + 8 > len(d): break
            w0, w1 = struct.unpack_from('>II', d, pc)
            op = w0 >> 24
            if op not in f3dex.VALID: break
            res['ncmd'] += 1
            if op == 0x04: res['vtx'].append((w1, ((w0 >> 10) & 0x3F) * 16))
            elif op == 0xFD:
                res['timg'].append(w1); cur = (w1, (w0 >> 19) & 3, (w0 & 0xFFF) + 1)
            elif op == 0xF3 and cur:      # LoadBlock: lrs+1 texels of the G_SETTIMG size
                n = ((w1 >> 12) & 0xFFF) + 1
                res['tload'].append((cur[0], (n * (4, 8, 16, 32)[cur[1]] + 7) // 8))
            elif op == 0xF0 and cur:      # LoadTLUT: count+1 16-bit colours
                res['tload'].append((cur[0], (((w1 >> 14) & 0x3FF) + 1) * 2))
            elif op == 0xF4 and cur:      # LoadTile: rectangle of a cur[2]-texel-wide image
                uls, ult, lrs, lrt = (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 12) & 0xFFF, w1 & 0xFFF
                bpp = (4, 8, 16, 32)[cur[1]]
                ext = ((lrt >> 2) * cur[2] + (lrs >> 2) + 1) * bpp // 8
                res['tload'].append((cur[0], ext))
            elif op == 0xBC and (w0 & 0xFF) == 6: res['segs'].append((((w0 >> 8) & 0xFFFF) // 4, w1))
            elif op == 0x06:
                res['calls'].append(w1)
                if (w0 >> 16) & 0xFF:      # branch: the list ends here
                    res['end'] = pc + 8; res['ok'] = True; break
            if op == 0xB8:
                res['end'] = pc + 8; res['ok'] = (w0, w1) == (0xB8000000, 0); break
            pc += 8
        self.dl_cache[k] = res
        return res

    def resolve(self, addr, space, segs):
        s = addr >> 24
        if 1 <= s <= 15:
            if s not in segs: return None
            addr = (segs[s] & 0x00FFFFFF) + (addr & 0x00FFFFFF) | 0x80000000
        return self.loc(addr, space)

    def dl_closure(self, roots, space):
        """roots: [(loc, segs)] with loc=(space, off) of a display list and segs={seg: base} active when it is
        called. Returns dict loc -> dict(dl=scan, vtx=[(space, off, len)] resolved with the active segments)."""
        seen = {}; todo = [(l, dict(sg)) for l, sg in roots]
        while todo:
            loc, segs = todo.pop()
            key = (loc, tuple(sorted(segs.items())))
            if key in seen: continue
            r = self.dl(*loc)
            seen[key] = r
            if not r['ok']: continue
            segs = dict(segs)
            for sg, base in r['segs']: segs[sg] = base
            r_vtx = []
            for raw, ln in r['vtx']:
                L = self.resolve(raw, space, segs)
                if L: r_vtx.append((L[0], L[1], ln))
            r.setdefault('rvtx', set()).update(r_vtx)
            for raw in r['timg']:
                L = self.resolve(raw, space, segs)
                if L: r.setdefault('rtimg', set()).add(L)
            for raw, n in r['tload']:
                L = self.resolve(raw, space, segs)
                if L: r.setdefault('rtload', set()).add((L[0], L[1], n))
            for c in r['calls']:
                L = self.resolve(c, space, segs)
                if L: todo.append((L, segs))
        out = {}
        for (loc, _), r in seen.items(): out[loc] = r
        return out

    def model_roots(self, space, header_offsets):
        """model records (lvlB FORMAT: +0x08 display list, +0x0C segment-3 base, +0x1C segment-4 base): roots for
        dl_closure. Records whose +0x08 does not resolve are ignored."""
        d = self.data(space); roots = []
        for o in header_offsets:
            if o + 0x20 > len(d): continue
            dl, s3, s4 = struct.unpack_from('>I', d, o + 8)[0], struct.unpack_from('>I', d, o + 12)[0], struct.unpack_from('>I', d, o + 0x1C)[0]
            L = self.loc(dl, space)
            if L: roots.append((L, {3: s3, 4: s4}))
        return roots


def merge_intervals(iv, touch=True):
    iv = sorted(iv); out = []
    for a, b in iv:
        if out and (a < out[-1][1] or (touch and a == out[-1][1])):
            out[-1][1] = max(out[-1][1], b)
        else:
            out.append([a, b])
    return [tuple(x) for x in out]


def vertex_arrays(dls, space):
    """vertex arrays in `space` as given by the G_VTX commands: loads issued by one display list that touch or
    overlap form one array; arrays of different lists are joined only when they overlap (shared vertices)."""
    per = []
    for (s, o), r in dls.items():
        if not r['ok']: continue
        iv = [(vo, vo + ln) for vs, vo, ln in r.get('rvtx', ()) if vs == space]
        per += merge_intervals(iv, touch=True)
    return merge_intervals(per, touch=False)


def pointer_words(data, a, b, lo=0x80000000, hi=0x80400000):
    """(number of aligned words in [a,b) that are RAM pointers, number of words)."""
    a4 = (a + 3) & ~3; n = 0; p = 0
    for o in range(a4, b - 3, 4):
        w = struct.unpack_from('>I', data, o)[0]; n += 1
        if lo <= w < hi: p += 1
    return p, n
