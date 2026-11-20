"""F3DEX 1.23 (GBI 1.x, F3D opcode numbering) display-list disassembler and texture/vertex tracer."""
import struct

FMTN = {0: 'G_IM_FMT_RGBA', 1: 'G_IM_FMT_YUV', 2: 'G_IM_FMT_CI', 3: 'G_IM_FMT_IA', 4: 'G_IM_FMT_I'}
SIZN = {0: 'G_IM_SIZ_4b', 1: 'G_IM_SIZ_8b', 2: 'G_IM_SIZ_16b', 3: 'G_IM_SIZ_32b'}
FMTS = {0: 'RGBA', 1: 'YUV', 2: 'CI', 3: 'IA', 4: 'I'}
BITS = {0: 4, 1: 8, 2: 16, 3: 32}

VALID = {0x01, 0x03, 0x04, 0x06, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,
         0xBE, 0xBF, 0xC0, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1,
         0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF}
ADDR_OPS = {0x01, 0x03, 0x04, 0x06, 0xFD}

def seg(a):
    return '0x%08X' % a

def _disasm(w0, w1):
    op = w0 >> 24
    if op == 0x04:
        n = (w0 >> 10) & 0x3F; v0 = ((w0 >> 16) & 0xFF) // 2
        return 'gsSPVertex(%s, %d, %d)' % (seg(w1), n, v0)
    if op == 0x06:
        return ('gsSPBranchList(%s)' if (w0 >> 16) & 0xFF else 'gsSPDisplayList(%s)') % seg(w1)
    if op == 0x01:
        p = (w0 >> 16) & 0xFF
        fl = []
        fl.append('G_MTX_PROJECTION' if p & 1 else 'G_MTX_MODELVIEW')
        fl.append('G_MTX_LOAD' if p & 2 else 'G_MTX_MUL')
        fl.append('G_MTX_PUSH' if p & 4 else 'G_MTX_NOPUSH')
        return 'gsSPMatrix(%s, %s)' % (seg(w1), '|'.join(fl))
    if op == 0x03:
        return 'gsDma1p(G_MOVEMEM, %s, %d, 0x%02X)' % (seg(w1), w0 & 0xFFFF, (w0 >> 16) & 0xFF)
    if op == 0xBF:
        return 'gsSP1Triangle(%d, %d, %d, %d)' % (((w1 >> 16) & 0xFF) // 2, ((w1 >> 8) & 0xFF) // 2, (w1 & 0xFF) // 2, w1 >> 24)
    if op == 0xB1:
        return 'gsSP2Triangles(%d, %d, %d, 0, %d, %d, %d, 0)' % (((w0 >> 16) & 0xFF) // 2, ((w0 >> 8) & 0xFF) // 2, (w0 & 0xFF) // 2,
                                                                 ((w1 >> 16) & 0xFF) // 2, ((w1 >> 8) & 0xFF) // 2, (w1 & 0xFF) // 2)
    if op == 0xB8: return 'gsSPEndDisplayList()'
    if op == 0xB6: return 'gsSPClearGeometryMode(0x%08X)' % w1
    if op == 0xB7: return 'gsSPSetGeometryMode(0x%08X)' % w1
    if op == 0xB9: return 'gsSPSetOtherMode(G_SETOTHERMODE_L, %d, %d, 0x%08X)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, w1)
    if op == 0xBA: return 'gsSPSetOtherMode(G_SETOTHERMODE_H, %d, %d, 0x%08X)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, w1)
    if op == 0xBB: return 'gsSPTexture(0x%04X, 0x%04X, %d, %d, %s)' % (w1 >> 16, w1 & 0xFFFF, (w0 >> 11) & 7, (w0 >> 8) & 7, 'G_ON' if w0 & 0xFF else 'G_OFF')
    if op == 0xBC: return 'gsMoveWd(%d, 0x%04X, 0x%08X)' % (w0 & 0xFF, (w0 >> 8) & 0xFFFF, w1)
    if op == 0xBD: return 'gsSPPopMatrix(%d)' % (w1 & 1)
    if op == 0xBE: return 'gsSPCullDisplayList(%d, %d)' % ((w0 & 0xFFFF) // 40, (w1 & 0xFFFF) // 40)
    if op == 0xB2: return 'gsSPModifyVertex(0x%08X, 0x%08X)' % (w0 & 0xFFFFFF, w1)
    if op == 0xB3: return 'gsImmp1(G_RDPHALF_2, 0x%08X)' % w1
    if op == 0xB4: return 'gsImmp1(G_RDPHALF_1, 0x%08X)' % w1
    if op == 0xB5: return 'gsSPLine3D(0x%08X, 0x%08X)' % (w0 & 0xFFFFFF, w1)
    if op == 0xC0: return 'gsSPNoOp()'
    if op == 0xE6: return 'gsDPLoadSync()'
    if op == 0xE7: return 'gsDPPipeSync()'
    if op == 0xE8: return 'gsDPTileSync()'
    if op == 0xE9: return 'gsDPFullSync()'
    if op in (0xE4, 0xE5):
        return 'gsSPTextureRectangle%s(%d, %d, %d, %d, %d)' % ('Flip' if op == 0xE5 else '', ((w1 >> 12) & 0xFFF), w1 & 0xFFF, (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 24) & 7)
    if op == 0xFD:
        return 'gsDPSetTextureImage(%s, %s, %d, %s)' % (FMTN.get((w0 >> 21) & 7, (w0 >> 21) & 7), SIZN[(w0 >> 19) & 3], (w0 & 0xFFF) + 1, seg(w1))
    if op == 0xF5:
        return 'gsDPSetTile(%s, %s, %d, 0x%03X, %d, %d, %d, %d, %d, %d, %d, %d)' % (
            FMTN.get((w0 >> 21) & 7, (w0 >> 21) & 7), SIZN[(w0 >> 19) & 3], (w0 >> 9) & 0x1FF, w0 & 0x1FF, (w1 >> 24) & 7, (w1 >> 20) & 15,
            (w1 >> 18) & 3, (w1 >> 14) & 15, (w1 >> 10) & 15, (w1 >> 8) & 3, (w1 >> 4) & 15, w1 & 15)
    if op == 0xF0: return 'gsDPLoadTLUTCmd(%d, %d)' % ((w1 >> 24) & 7, ((w1 >> 14) & 0x3FF))
    if op == 0xF3: return 'gsDPLoadBlock(%d, %d, %d, %d, %d)' % ((w1 >> 24) & 7, (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 12) & 0xFFF, w1 & 0xFFF)
    if op in (0xF4, 0xF2):
        return 'gsDP%s(%d, %d, %d, %d, %d)' % ('LoadTile' if op == 0xF4 else 'SetTileSize', (w1 >> 24) & 7, (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 12) & 0xFFF, w1 & 0xFFF)
    if op == 0xFC: return 'gsDPSetCombine(0x%06X, 0x%08X)' % (w0 & 0xFFFFFF, w1)
    if op == 0xFA: return 'gsDPSetPrimColor(%d, %d, %d, %d, %d, %d)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    if op == 0xFB: return 'gsDPSetEnvColor(%d, %d, %d, %d)' % (w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    if op == 0xF9: return 'gsDPSetBlendColor(%d, %d, %d, %d)' % (w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    if op == 0xF8: return 'gsDPSetFogColor(%d, %d, %d, %d)' % (w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    if op == 0xF7: return 'gsDPSetFillColor(0x%08X)' % w1
    if op == 0xF6: return 'gsDPFillRectangle(%d, %d, %d, %d)' % ((w1 >> 14) & 0x3FF, (w1 >> 2) & 0x3FF, (w0 >> 14) & 0x3FF, (w0 >> 2) & 0x3FF)
    if op == 0xEF: return 'gsDPSetOtherMode(0x%06X, 0x%08X)' % (w0 & 0xFFFFFF, w1)
    if op == 0xED: return 'gsDPSetScissor(%d, %d, %d, %d, %d)' % ((w1 >> 24) & 3, (w0 >> 14) & 0x3FF, (w0 >> 2) & 0x3FF, (w1 >> 14) & 0x3FF, (w1 >> 2) & 0x3FF)
    if op == 0xFF: return 'gsDPSetColorImage(%d, %d, %d, %s)' % ((w0 >> 21) & 7, (w0 >> 19) & 3, (w0 & 0xFFF) + 1, seg(w1))
    if op == 0xFE: return 'gsDPSetDepthImage(%s)' % seg(w1)
    return 'gsDPWord(0x%08X, 0x%08X) /* op 0x%02X */' % (w0, w1, op)


FMTV = {v: k for k, v in FMTN.items()}
SIZV = {v: k for k, v in SIZN.items()}
_MTX = {'G_MTX_PROJECTION': 1, 'G_MTX_MODELVIEW': 0, 'G_MTX_LOAD': 2, 'G_MTX_MUL': 0, 'G_MTX_PUSH': 4, 'G_MTX_NOPUSH': 0}


def _args(text):
    name, rest = text.split('(', 1)
    rest = rest.rsplit(')', 1)[0]
    a = [x.strip() for x in rest.split(',')] if rest.strip() else []
    return name.strip(), a


def _n(x):
    if x in FMTV: return FMTV[x]
    if x in SIZV: return SIZV[x]
    if x == 'G_ON': return 1
    if x == 'G_OFF': return 0
    return int(x, 0)


def assemble(text):
    """inverse of disasm(): one macro (as disasm prints it, trailing comments/commas allowed) -> (w0, w1)"""
    text = text.split('/*')[0].strip().rstrip(',').strip()
    name, a = _args(text)
    n = [_n(x) if not x.startswith('G_MTX') else 0 for x in a]
    if name == 'gsSPVertex':
        cnt, v0 = n[1], n[2]
        return (0x04 << 24) | ((v0 * 2) << 16) | (cnt << 10) | (16 * cnt - 1), n[0]
    if name == 'gsSPDisplayList': return 0x06000000, n[0]
    if name == 'gsSPBranchList': return 0x06010000, n[0]
    if name == 'gsSPMatrix':
        p = 0
        for f in a[1].split('|'): p |= _MTX[f]
        return 0x01000040 | (p << 16), n[0]
    if name == 'gsDma1p': return (0x03 << 24) | (n[3] << 16) | n[2], n[1]
    if name == 'gsSP1Triangle': return 0xBF000000, (n[3] << 24) | (n[0] * 2 << 16) | (n[1] * 2 << 8) | n[2] * 2
    if name == 'gsSP2Triangles':
        return 0xB1000000 | (n[0] * 2 << 16) | (n[1] * 2 << 8) | n[2] * 2, (n[4] * 2 << 16) | (n[5] * 2 << 8) | n[6] * 2
    if name == 'gsSPEndDisplayList': return 0xB8000000, 0
    if name == 'gsSPClearGeometryMode': return 0xB6000000, n[0]
    if name == 'gsSPSetGeometryMode': return 0xB7000000, n[0]
    if name == 'gsSPSetOtherMode':
        return (0xB9 if a[0] == 'G_SETOTHERMODE_L' else 0xBA) << 24 | (n[1] << 8) | n[2], n[3]
    if name == 'gsSPTexture': return 0xBB000000 | (n[2] << 11) | (n[3] << 8) | n[4], (n[0] << 16) | n[1]
    if name == 'gsMoveWd': return 0xBC000000 | (n[1] << 8) | n[0], n[2]
    if name == 'gsSPPopMatrix': return 0xBD000000, n[0]
    if name == 'gsSPCullDisplayList': return 0xBE000000 | (n[0] * 40), n[1] * 40
    if name == 'gsSPModifyVertex': return 0xB2000000 | n[0], n[1]
    if name == 'gsImmp1': return (0xB3 if a[0] == 'G_RDPHALF_2' else 0xB4) << 24, n[1]
    if name == 'gsSPLine3D': return 0xB5000000 | n[0], n[1]
    if name == 'gsSPNoOp': return 0xC0000000, 0
    simple = {'gsDPLoadSync': 0xE6, 'gsDPPipeSync': 0xE7, 'gsDPTileSync': 0xE8, 'gsDPFullSync': 0xE9}
    if name in simple: return simple[name] << 24, 0
    if name.startswith('gsSPTextureRectangle'):
        op = 0xE5 if name.endswith('Flip') else 0xE4
        return (op << 24) | (n[2] << 12) | n[3], (n[4] << 24) | (n[0] << 12) | n[1]
    if name == 'gsDPSetTextureImage': return 0xFD000000 | (n[0] << 21) | (n[1] << 19) | (n[2] - 1), n[3]
    if name == 'gsDPSetTile':
        return (0xF5000000 | (n[0] << 21) | (n[1] << 19) | (n[2] << 9) | n[3],
                (n[4] << 24) | (n[5] << 20) | (n[6] << 18) | (n[7] << 14) | (n[8] << 10) | (n[9] << 8) | (n[10] << 4) | n[11])
    if name == 'gsDPLoadTLUTCmd': return 0xF0000000, (n[0] << 24) | (n[1] << 14)
    if name in ('gsDPLoadBlock', 'gsDPLoadTile', 'gsDPSetTileSize'):
        op = {'gsDPLoadBlock': 0xF3, 'gsDPLoadTile': 0xF4, 'gsDPSetTileSize': 0xF2}[name]
        return (op << 24) | (n[1] << 12) | n[2], (n[0] << 24) | (n[3] << 12) | n[4]
    if name == 'gsDPSetCombine': return 0xFC000000 | n[0], n[1]
    if name == 'gsDPSetPrimColor': return 0xFA000000 | (n[0] << 8) | n[1], (n[2] << 24) | (n[3] << 16) | (n[4] << 8) | n[5]
    colors = {'gsDPSetEnvColor': 0xFB, 'gsDPSetBlendColor': 0xF9, 'gsDPSetFogColor': 0xF8}
    if name in colors: return colors[name] << 24, (n[0] << 24) | (n[1] << 16) | (n[2] << 8) | n[3]
    if name == 'gsDPSetFillColor': return 0xF7000000, n[0]
    if name == 'gsDPFillRectangle': return 0xF6000000 | (n[2] << 14) | (n[3] << 2), (n[0] << 14) | (n[1] << 2)
    if name == 'gsDPSetOtherMode': return 0xEF000000 | n[0], n[1]
    if name == 'gsDPSetScissor': return 0xED000000 | (n[1] << 14) | (n[2] << 2), (n[0] << 24) | (n[3] << 14) | (n[4] << 2)
    if name == 'gsDPSetColorImage': return 0xFF000000 | (n[0] << 21) | (n[1] << 19) | (n[2] - 1), n[3]
    if name == 'gsDPSetDepthImage': return 0xFE000000, n[0]
    if name == 'gsDPWord': return n[0], n[1]
    raise ValueError('unknown macro ' + name)


def disasm(w0, w1):
    """F3DEX 1.23 macro text for one command; falls back to gsDPWord when the macro form would lose bits,
    so assemble(disasm(w0, w1)) == (w0, w1) always."""
    t = _disasm(w0, w1)
    try:
        if assemble(t) == (w0, w1):
            return t
    except Exception:
        pass
    return 'gsDPWord(0x%08X, 0x%08X) /* op 0x%02X: %s */' % (w0, w1, w0 >> 24, t)


def selftest(n=200000, seed=1):
    import random
    r = random.Random(seed)
    for _ in range(n):
        w0 = (r.choice(sorted(VALID | {0, 5, 0x7F})) << 24) | r.getrandbits(24)
        w1 = r.getrandbits(32)
        assert assemble(disasm(w0, w1)) == (w0, w1), hex(w0)
    return True


def plausible(w0, w1):
    op = w0 >> 24
    if op not in VALID:
        return False
    if op == 0x04:
        n = (w0 >> 10) & 0x3F
        return n >= 1 and (w0 & 0x3FF) + 1 == n * 16 and ((w0 >> 16) & 0xFF) // 2 + n <= 32 and ((w1 >> 24) in (0x80, 0x03, 0x04, 0x05, 0x06, 0x07, 0x01, 0x02))
    if op in (0x06,):
        return (w0 & 0x00FFFFFF) in (0, 0x010000) and (w1 >> 24) in (0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07) and w1 & 7 == 0
    if op == 0xB8:
        return w0 == 0xB8000000 and w1 == 0
    if op in (0xE6, 0xE7, 0xE8, 0xE9):
        return (w0 & 0xFFFFFF) == 0 and w1 == 0
    if op == 0xBF:
        return (w0 & 0xFFFFFF) == 0 and all(((w1 >> s) & 0xFF) % 2 == 0 and ((w1 >> s) & 0xFF) < 64 for s in (0, 8, 16))
    if op == 0xB1:
        return all(((x >> s) & 0xFF) % 2 == 0 and ((x >> s) & 0xFF) < 64 for x in (w0, w1) for s in (0, 8, 16)) and (w1 >> 24) == 0
    if op == 0xFD:
        return (w0 >> 21) & 7 <= 4 and (w0 & 0x0007F000) == 0 and (w1 >> 24) in (0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07)
    if op == 0xF5:
        return (w0 >> 21) & 7 <= 4 and (w1 >> 27) == 0
    if op in (0xC0, 0xB5, 0xB2, 0xFF, 0xFE, 0xE9, 0xBD, 0xBE):
        return False
    if op in (0x01, 0x03):
        return (w1 >> 24) in (0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07) and (w0 & 0xFFFF) in (0x40, 0x10, 0x20, 0x28, 0x08, 0x18)
    return True


class Tracer:
    """Symbolic run of a display list. resolve(addr) -> (key, bytes_at) or None.
    Records textures: dict(addr, fmt, bits, w, h, tlut_addr, tlut_n, pal)."""
    def __init__(self, resolve, segments=None):
        self.resolve = resolve
        self.seg = dict(segments or {})
        self.timg = None
        self.tiles = {i: {} for i in range(8)}
        self.tmem = {}   # tmem word addr -> (img addr, fmt, bits, width) loaded by LoadBlock/LoadTile
        self.tlut = {}   # tmem word -> (addr, count)
        self.textures = []
        self.vtx = []
        self.dls = []
        self.lines = []
        self.vbuf = [None] * 32
        self.tris = []
        self.cur = None

    def addr(self, a):
        s = a >> 24
        if s in self.seg:
            return self.seg[s] + (a & 0xFFFFFF)
        return a

    def run(self, start, depth=0, limit=20000):
        pc = start
        for _ in range(limit):
            r = self.resolve(pc, 8)
            if r is None:
                return False
            w0, w1 = struct.unpack('>II', r)
            self.step(w0, w1, pc, depth)
            op = w0 >> 24
            if op == 0xB8:
                return True
            if op == 0x06:
                tgt = self.addr(w1)
                if depth < 8 and self.resolve(tgt, 8) is not None:
                    self.dls.append(tgt)
                    self.run(tgt, depth + 1)
                if (w0 >> 16) & 0xFF:
                    return True
            pc += 8
        return False

    def step(self, w0, w1, pc, depth):
        op = w0 >> 24
        if op == 0xBC and (w0 & 0xFF) == 6:
            self.seg[((w0 >> 8) & 0xFFFF) // 4] = w1 & 0xFFFFFF | 0x80000000 if w1 >= 0x80000000 else w1
        elif op == 0x04:
            n = (w0 >> 10) & 0x3F; v0 = ((w0 >> 16) & 0xFF) // 2
            a = self.addr(w1)
            self.vtx.append((a, n))
            for i in range(n):
                if v0 + i < 32:
                    self.vbuf[v0 + i] = (a, i)
        elif op == 0xBF:
            self.tris.append((tuple(self.vbuf[((w1 >> s) & 0xFF) // 2] for s in (16, 8, 0)), self.cur))
        elif op == 0xB1:
            self.tris.append((tuple(self.vbuf[((w0 >> s) & 0xFF) // 2] for s in (16, 8, 0)), self.cur))
            self.tris.append((tuple(self.vbuf[((w1 >> s) & 0xFF) // 2] for s in (16, 8, 0)), self.cur))
        elif op == 0xFD:
            self.timg = (self.addr(w1), (w0 >> 21) & 7, BITS[(w0 >> 19) & 3], (w0 & 0xFFF) + 1)
        elif op == 0xF5:
            t = (w1 >> 24) & 7
            self.tiles[t] = dict(fmt=(w0 >> 21) & 7, bits=BITS[(w0 >> 19) & 3], line=(w0 >> 9) & 0x1FF, tmem=w0 & 0x1FF, pal=(w1 >> 20) & 15)
        elif op == 0xF0:
            t = (w1 >> 24) & 7; n = ((w1 >> 14) & 0x3FF) + 1
            if self.timg:
                self.tlut[self.tiles[t].get('tmem', 0x100)] = (self.timg[0], n)
        elif op == 0xF3:
            t = (w1 >> 24) & 7
            if self.timg:
                texels = ((w1 >> 12) & 0xFFF) + 1
                self.tmem[self.tiles[t].get('tmem', 0)] = dict(addr=self.timg[0], lbits=self.timg[2], texels=texels, width=None)
        elif op == 0xF4:
            t = (w1 >> 24) & 7
            if self.timg:
                uls = (w0 >> 12) & 0xFFF; ult = w0 & 0xFFF; lrs = (w1 >> 12) & 0xFFF; lrt = w1 & 0xFFF
                self.tmem[self.tiles[t].get('tmem', 0)] = dict(addr=self.timg[0], lbits=self.timg[2], width=self.timg[3],
                                                               uls=uls >> 2, ult=ult >> 2, w=(lrs - uls) // 4 + 1, h=(lrt - ult) // 4 + 1)
        elif op == 0xF2:
            t = (w1 >> 24) & 7
            tile = self.tiles.get(t)
            if t == 7 or not tile or 'tmem' not in tile:
                return
            ld = self.tmem.get(tile['tmem'])
            if not ld:
                return
            uls = (w0 >> 12) & 0xFFF; ult = w0 & 0xFFF; lrs = (w1 >> 12) & 0xFFF; lrt = w1 & 0xFFF
            w = (lrs - uls) // 4 + 1; h = (lrt - ult) // 4 + 1
            fmt = FMTS.get(tile['fmt']); bits = tile['bits']
            if ld.get('width') is None:
                # LoadBlock: whole texture contiguous; width from tile line (in 64-bit words)
                line = tile['line']
                if line:
                    w = line * 64 // bits if bits else w
                nbytes = ld['texels'] * ld['lbits'] // 8
                h2 = (nbytes * 8 // bits) // w if w else h
                h = max(1, h2)
                tex = dict(addr=ld['addr'], fmt=fmt, bits=bits, w=w, h=h, row=None)
            else:
                # LoadTile: sub-rectangle of a wider image
                tex = dict(addr=ld['addr'], fmt=fmt, bits=bits, w=ld['w'] if ld['lbits'] == bits else w, h=ld['h'],
                           row=ld['width'], x=ld['uls'], y=ld['ult'])
            if fmt == 'CI':
                key = 0x100 + tile['pal'] * 16 if bits == 4 else 0x100
                tl = self.tlut.get(key) or self.tlut.get(0x100)
                if tl and bits == 4 and key not in self.tlut:
                    tex['tlut'] = (tl[0] + tile['pal'] * 32, 16)
                elif tl:
                    tex['tlut'] = (tl[0], 16 if bits == 4 else 256)
            self.textures.append(tex)
            self.cur = len(self.textures) - 1


if __name__ == '__main__':
    selftest(); print('f3dex selftest ok')
