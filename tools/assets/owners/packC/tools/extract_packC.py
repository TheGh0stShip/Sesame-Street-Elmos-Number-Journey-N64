#!/usr/bin/env python3
"""Extractor for the `common` graphics pack (Letter ROM 0x10D0F0-0x1BF6F0, Number ROM 0x10D560-0x177680).

The file is a pre-linked RAM image that the level loader (Letter func_8001D9D0, Number func_8001E2CC) PI-copies
raw to RAM 0x8024B000 with func_80017318 / func_80017788. Every pointer inside it is an absolute KSEG0 address.

usage:
  extract_packC.py ROM GAME OUTDIR          extract (DL text, vertex JSON, tables, textures PNG+bin, OBJ, MAP rows)
  extract_packC.py check ROM GAME OUTDIR    re-assemble every text/JSON form in OUTDIR back to bytes and compare
  extract_packC.py map ROM GAME             print MAP.tsv rows only
GAME is letter or number. No ROM content is stored in this script.
"""
import sys, os, re, struct, json, collections

FILES = {'letter': (0x10D0F0, 0x1BF6F0), 'number': (0x10D560, 0x177680)}
LOADER = {'letter': 'func_8001D9D0 -> func_80017318 (raw PI copy)', 'number': 'func_8001E2CC -> func_80017788 (raw PI copy)'}
BASE = 0x8024B000
# gzip level members (RAM images for 0x80154000) whose display lists point into this pack
LEVELS = {'letter': [(0x1BF6F0, 0x204D00), (0x204D00, 0x227BA0), (0x227BA0, 0x243E20), (0x243E20, 0x2604B0),
                     (0x2604B0, 0x28FDB0), (0x28FDB0, 0x2ACFA0), (0x2ACFA0, 0x2D1FB0), (0x2D1FB0, 0x2F9B60)],
          'number': [(0x177680, 0x1BCE90), (0x1BCE90, 0x1D6CA0), (0x1D6CA0, 0x206CC0), (0x206CC0, 0x22EC10),
                     (0x22EC10, 0x25AF80), (0x25AF80, 0x27ADC0), (0x27ADC0, 0x2B8FD0), (0x2B8FD0, 0x2E6FC0)]}
REFS = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'LEVEL_REFS_%s.tsv')


def load_refs(game):
    """Code / .data roots into this pack (LEVEL_REFS_<game>.tsv, computed from the ROM by the coordinator)."""
    r = {}
    try:
        for l in open(REFS % game):
            f = l.rstrip('\n').split('\t')
            if len(f) >= 4 and f[1].startswith('common'):
                r[int(f[0], 16) - BASE] = f[3]
    except OSError:
        pass
    return r


def level_images(rom, game):
    import zlib
    out = []
    for a, b in LEVELS[game]:
        try:
            out.append(((a, b), zlib.decompressobj(31).decompress(rom[a:b])))
        except zlib.error:
            pass
    return out

FMTN = {0: 'RGBA', 1: 'YUV', 2: 'CI', 3: 'IA', 4: 'I'}
BITS = {0: 4, 1: 8, 2: 16, 3: 32}

# ---------------------------------------------------------------- F3DEX 1.x disassembler / assembler (lossless)
FMTS = ['G_IM_FMT_RGBA', 'G_IM_FMT_YUV', 'G_IM_FMT_CI', 'G_IM_FMT_IA', 'G_IM_FMT_I']
SIZS = ['G_IM_SIZ_4b', 'G_IM_SIZ_8b', 'G_IM_SIZ_16b', 'G_IM_SIZ_32b']


def _h(v):
    return '0x%08X' % v


def dis(w0, w1):
    op = w0 >> 24
    t = None
    if op == 0x04:
        n = (w0 >> 10) & 0x3F
        t = 'gsSPVertex(%s, %d, %d)' % (_h(w1), n, ((w0 >> 16) & 0xFF) // 2)
    elif op == 0x06:
        t = ('gsSPBranchList(%s)' if (w0 >> 16) & 0xFF else 'gsSPDisplayList(%s)') % _h(w1)
    elif op == 0x01:
        t = 'gsSPMatrix(%s, 0x%02X)' % (_h(w1), (w0 >> 16) & 0xFF)
    elif op == 0xBF:
        t = 'gsSP1Triangle(%d, %d, %d, %d)' % (((w1 >> 16) & 0xFF) // 2, ((w1 >> 8) & 0xFF) // 2, (w1 & 0xFF) // 2, w1 >> 24)
    elif op == 0xB1:
        t = 'gsSP2Triangles(%d, %d, %d, 0, %d, %d, %d, 0)' % (((w0 >> 16) & 0xFF) // 2, ((w0 >> 8) & 0xFF) // 2, (w0 & 0xFF) // 2,
                                                             ((w1 >> 16) & 0xFF) // 2, ((w1 >> 8) & 0xFF) // 2, (w1 & 0xFF) // 2)
    elif op == 0xB8: t = 'gsSPEndDisplayList()'
    elif op == 0xB6: t = 'gsSPClearGeometryMode(%s)' % _h(w1)
    elif op == 0xB7: t = 'gsSPSetGeometryMode(%s)' % _h(w1)
    elif op == 0xB9: t = 'gsSPSetOtherMode(G_SETOTHERMODE_L, %d, %d, %s)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, _h(w1))
    elif op == 0xBA: t = 'gsSPSetOtherMode(G_SETOTHERMODE_H, %d, %d, %s)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, _h(w1))
    elif op == 0xBB: t = 'gsSPTexture(0x%04X, 0x%04X, %d, %d, %d)' % (w1 >> 16, w1 & 0xFFFF, (w0 >> 11) & 7, (w0 >> 8) & 7, w0 & 0xFF)
    elif op == 0xBC: t = 'gsMoveWd(%d, 0x%04X, %s)' % (w0 & 0xFF, (w0 >> 8) & 0xFFFF, _h(w1))
    elif op == 0xBD: t = 'gsSPPopMatrix(%d)' % w1
    elif op == 0xBE: t = 'gsSPCullDisplayList(%d, %d)' % ((w0 & 0xFFFF) // 40, (w1 & 0xFFFF) // 40)
    elif op == 0xE6: t = 'gsDPLoadSync()'
    elif op == 0xE7: t = 'gsDPPipeSync()'
    elif op == 0xE8: t = 'gsDPTileSync()'
    elif op == 0xE9: t = 'gsDPFullSync()'
    elif op == 0xFD:
        t = 'gsDPSetTextureImage(%s, %s, %d, %s)' % (FMTS[(w0 >> 21) & 7] if (w0 >> 21) & 7 < 5 else (w0 >> 21) & 7,
                                                   SIZS[(w0 >> 19) & 3], (w0 & 0xFFF) + 1, _h(w1))
    elif op == 0xF5:
        t = 'gsDPSetTile(%s, %s, %d, 0x%03X, %d, %d, %d, %d, %d, %d, %d, %d)' % (
            FMTS[(w0 >> 21) & 7] if (w0 >> 21) & 7 < 5 else (w0 >> 21) & 7, SIZS[(w0 >> 19) & 3], (w0 >> 9) & 0x1FF, w0 & 0x1FF,
            (w1 >> 24) & 7, (w1 >> 20) & 15, (w1 >> 18) & 3, (w1 >> 14) & 15, (w1 >> 10) & 15, (w1 >> 8) & 3, (w1 >> 4) & 15, w1 & 15)
    elif op == 0xF0: t = 'gsDPLoadTLUTCmd(%d, %d)' % ((w1 >> 24) & 7, (w1 >> 14) & 0x3FF)
    elif op in (0xF3, 0xF4, 0xF2):
        t = 'gsDP%s(%d, %d, %d, %d, %d)' % ({0xF3: 'LoadBlock', 0xF4: 'LoadTile', 0xF2: 'SetTileSize'}[op], (w1 >> 24) & 7,
                                           (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 12) & 0xFFF, w1 & 0xFFF)
    elif op == 0xFC: t = 'gsDPSetCombine(0x%06X, %s)' % (w0 & 0xFFFFFF, _h(w1))
    elif op == 0xFA: t = 'gsDPSetPrimColor(%d, %d, %d, %d, %d, %d)' % ((w0 >> 8) & 0xFF, w0 & 0xFF, w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    elif op in (0xFB, 0xF9, 0xF8):
        t = 'gsDPSet%sColor(%d, %d, %d, %d)' % ({0xFB: 'Env', 0xF9: 'Blend', 0xF8: 'Fog'}[op], w1 >> 24, (w1 >> 16) & 255, (w1 >> 8) & 255, w1 & 255)
    elif op == 0xF7: t = 'gsDPSetFillColor(%s)' % _h(w1)
    elif op == 0xEF: t = 'gsDPSetOtherMode(0x%06X, %s)' % (w0 & 0xFFFFFF, _h(w1))
    if t is not None:
        try:
            if asm(t) == (w0, w1):
                return t
        except Exception:
            pass
    return 'gsDPWord(%s, %s)' % (_h(w0), _h(w1))


def _num(s):
    s = s.strip()
    if s in FMTS: return FMTS.index(s)
    if s in SIZS: return SIZS.index(s)
    return int(s, 0)


def asm(line):
    m = re.match(r'\s*(\w+)\((.*)\)\s*$', line)
    name, a = m.group(1), m.group(2)
    if name == 'gsSPSetOtherMode':
        k, rest = a.split(',', 1)
        v = [_num(x) for x in rest.split(',')]
        return ((0xB9 if k.strip().endswith('_L') else 0xBA) << 24) | (v[0] << 8) | v[1], v[2]
    v = [_num(x) for x in a.split(',')] if a.strip() else []
    W = lambda op, lo=0: (op << 24) | lo
    if name == 'gsDPWord': return v[0], v[1]
    if name == 'gsSPVertex': return W(0x04, ((v[2] * 2) << 16) | (v[1] << 10) | (v[1] * 16 - 1)), v[0]
    if name == 'gsSPDisplayList': return W(0x06), v[0]
    if name == 'gsSPBranchList': return W(0x06, 0x010000), v[0]
    if name == 'gsSPMatrix': return W(0x01, (v[1] << 16) | 0x40), v[0]
    if name == 'gsSP1Triangle': return W(0xBF), (v[3] << 24) | (v[0] * 2 << 16) | (v[1] * 2 << 8) | v[2] * 2
    if name == 'gsSP2Triangles':
        return W(0xB1, (v[0] * 2 << 16) | (v[1] * 2 << 8) | v[2] * 2), (v[4] * 2 << 16) | (v[5] * 2 << 8) | v[6] * 2
    simple = {'gsSPEndDisplayList': 0xB8, 'gsDPLoadSync': 0xE6, 'gsDPPipeSync': 0xE7, 'gsDPTileSync': 0xE8, 'gsDPFullSync': 0xE9}
    if name in simple: return W(simple[name]), 0
    if name == 'gsSPClearGeometryMode': return W(0xB6), v[0]
    if name == 'gsSPSetGeometryMode': return W(0xB7), v[0]
    if name == 'gsSPTexture': return W(0xBB, (v[2] << 11) | (v[3] << 8) | v[4]), (v[0] << 16) | v[1]
    if name == 'gsMoveWd': return W(0xBC, (v[1] << 8) | v[0]), v[2]
    if name == 'gsSPPopMatrix': return W(0xBD), v[0]
    if name == 'gsSPCullDisplayList': return W(0xBE, v[0] * 40), v[1] * 40
    if name == 'gsDPSetTextureImage': return W(0xFD, (v[0] << 21) | (v[1] << 19) | (v[2] - 1)), v[3]
    if name == 'gsDPSetTile':
        return (W(0xF5, (v[0] << 21) | (v[1] << 19) | (v[2] << 9) | v[3]),
                (v[4] << 24) | (v[5] << 20) | (v[6] << 18) | (v[7] << 14) | (v[8] << 10) | (v[9] << 8) | (v[10] << 4) | v[11])
    if name == 'gsDPLoadTLUTCmd': return W(0xF0), (v[0] << 24) | (v[1] << 14)
    if name in ('gsDPLoadBlock', 'gsDPLoadTile', 'gsDPSetTileSize'):
        op = {'gsDPLoadBlock': 0xF3, 'gsDPLoadTile': 0xF4, 'gsDPSetTileSize': 0xF2}[name]
        return W(op, (v[1] << 12) | v[2]), (v[0] << 24) | (v[3] << 12) | v[4]
    if name == 'gsDPSetCombine': return W(0xFC, v[0]), v[1]
    if name == 'gsDPSetPrimColor': return W(0xFA, (v[0] << 8) | v[1]), (v[2] << 24) | (v[3] << 16) | (v[4] << 8) | v[5]
    cols = {'gsDPSetEnvColor': 0xFB, 'gsDPSetBlendColor': 0xF9, 'gsDPSetFogColor': 0xF8}
    if name in cols: return W(cols[name]), (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3]
    if name == 'gsDPSetFillColor': return W(0xF7), v[0]
    if name == 'gsDPSetOtherMode': return W(0xEF, v[0]), v[1]
    raise ValueError(name)


GOOD_OPS = {0x01, 0x04, 0x06, 0xB1, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF, 0xE6, 0xE7, 0xE8, 0xF0, 0xF2,
            0xF3, 0xF4, 0xF5, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xEF}


# ---------------------------------------------------------------- analysis
class Pack:
    def __init__(self, data, game, levels=()):
        self.levels = levels
        self.d = data; self.n = len(data); self.game = game
        self.objs = {}          # start -> dict(end, cls, fmt, desc, ref, extra)
        self.notes = []

    def u32(self, o): return struct.unpack_from('>I', self.d, o)[0]
    def u16(self, o): return struct.unpack_from('>H', self.d, o)[0]

    def inr(self, p): return BASE <= p < BASE + self.n

    def add(self, s, e, cls, fmt, desc, ref, **kw):
        e = min(e, self.n)
        if s in self.objs:
            o = self.objs[s]
            o['end'] = max(o['end'], e)
            if ref not in o['ref']: o['ref'].append(ref)
            return o
        o = dict(start=s, end=e, cls=cls, fmt=fmt, desc=desc, ref=[ref]); o.update(kw)
        self.objs[s] = o
        return o

    # display list: decode linearly until end/branch; returns (end, cmds)
    def dl_walk(self, o):
        cmds = []
        while o + 8 <= self.n:
            w0, w1 = struct.unpack_from('>II', self.d, o)
            op = w0 >> 24
            if op not in GOOD_OPS: return None
            cmds.append((o, w0, w1)); o += 8
            if op == 0xB8 or (op == 0x06 and (w0 >> 16) & 0xFF): return o, cmds
        return None

    def trace_dl(self, off, model, ref, depth=0):
        """Trace a display list (file offset), registering sub-lists, vertex loads (seg 3) and texture loads."""
        r = self.dl_walk(off)
        if r is None:
            self.notes.append('DL at 0x%X does not decode' % off); return None
        end, cmds = r
        o = self.add(off, end, 'dl', 'f3dex_dl', 'F3DEX display list', ref)
        o.setdefault('model', model)
        for (p, w0, w1) in cmds:
            op = w0 >> 24
            if op == 0x06 and self.inr(w1) and depth < 16:
                self.trace_dl(w1 - BASE, model, 'gsSPDisplayList from DL 0x%X' % off, depth + 1)
            elif op == 0x04 and model is not None:
                n = (w0 >> 10) & 0x3F
                if (w1 >> 24) == 3: model['vloads'].append(w1 & 0xFFFFFF)
                elif self.inr(w1):
                    self.add(w1 - BASE, w1 - BASE + 16 * n, 'vtx', 'Vtx[%d]' % n, 'vertex array (absolute load)', 'gsSPVertex in DL 0x%X' % off)
        self.tex_state([(w0, w1) for (_, w0, w1) in cmds], model, 'DL 0x%X' % off)
        return o

    def resolve(self, addr, model):
        if (addr >> 24) == 4 and model is not None and model.get('tex'):
            return model['tex'] - BASE + (addr & 0xFFFFFF)
        if self.inr(addr): return addr - BASE
        return None

    def tex_state(self, cmds, model, where):
        """Follow SetTextureImage / LoadBlock / LoadTile / LoadTLUT / SetTile / SetTileSize and register texels and
        palettes with their render format (tile 0) and size (SetTileSize of tile 0)."""
        timg = None; pal = None; pending = None
        def flush():
            if pending and pending['a'] is not None:
                p = pending
                o = self.add(p['a'], p['a'] + p['nb'], 'texture', '%s%d' % (p['fmt'], p['bits']),
                             'texture texels (%s)' % p['how'], 'gsDP%s in %s' % (p['how'], where))
                if p.get('w') and 'img' not in o:
                    o['img'] = (p['fmt'], p['bits'], p['w'], p['h'], pal if p['fmt'] == 'CI' else None)
        for (w0, w1) in cmds:
            op = w0 >> 24
            if op == 0xFD:
                timg = ((w0 >> 21) & 7, BITS[(w0 >> 19) & 3], (w0 & 0xFFF) + 1, w1)
            elif op == 0xF3 and timg:
                flush()
                pending = dict(a=self.resolve(timg[3], model), nb=(((w1 >> 12) & 0xFFF) + 1) * timg[1] // 8,
                               fmt=FMTN.get(timg[0], '?'), bits=timg[1], how='LoadBlock')
            elif op == 0xF4 and timg:
                flush()
                lrt = (w1 & 0xFFF) >> 2
                pending = dict(a=self.resolve(timg[3], model), nb=timg[2] * (lrt + 1) * timg[1] // 8,
                               fmt=FMTN.get(timg[0], '?'), bits=timg[1], how='LoadTile', w=timg[2], h=lrt + 1)
            elif op == 0xF0 and timg:
                a = self.resolve(timg[3], model)
                if a is not None:
                    nb = (((w1 >> 14) & 0x3FF) + 1) * 2
                    self.add(a, a + nb, 'tlut', 'RGBA16 palette[%d]' % (nb // 2), 'texture palette (LoadTLUT)', 'gsDPLoadTLUTCmd in ' + where)
                    pal = a
            elif op == 0xF5 and pending and (w1 >> 24) & 7 == 0:
                pending['fmt'] = FMTN.get((w0 >> 21) & 7, '?'); pending['bits'] = BITS[(w0 >> 19) & 3]
            elif op == 0xF2 and pending and (w1 >> 24) & 7 == 0:
                w = ((w1 >> 12) & 0xFFF) // 4 + 1 - ((w0 >> 12) & 0xFFF) // 4
                h = (w1 & 0xFFF) // 4 + 1 - (w0 & 0xFFF) // 4
                if pending['how'] == 'LoadBlock': pending['w'], pending['h'] = w, h
            elif op in (0xB8,):
                flush(); pending = None
        flush()

    def parse_tbl(self, o, nverts):
        """Vertex expansion list: {u16 n, u16 m, u16 idx[m]} until n+m summed over entries == nverts."""
        uniq = tot = 0
        while tot < nverts:
            if o + 4 > self.n: return None
            n, m = self.u16(o), self.u16(o + 2); o += 4 + 2 * m; uniq += n; tot += n + m
        if tot != nverts: return None
        if o % 4 and o + 2 <= self.n and self.u16(o) == 0: o += 2
        return o, uniq

    def is_anim(self, a):
        if a + 8 > self.n: return False
        he, do, nk = self.u16(a), self.u16(a + 2), self.d[a + 5]
        if nk < 2 or he != 8 + nk or do % 8 or not 0 <= do - he < 16: return False
        keys = self.d[a + 8:a + 8 + nk]
        return keys[0] == 0 and all(keys[i] < keys[i + 1] for i in range(nk - 1)) and keys[-1] <= self.d[a + 4]

    # model header: 0x28 bytes (func_80018530 copies 10 words)
    def is_header(self, o):
        if o + 0x28 > self.n: return False
        nv = self.u16(o + 4); dl = self.u32(o + 8); vt = self.u32(o + 0xC)
        if not (0 < nv < 4096 and self.inr(dl) and self.inr(vt)): return False
        if any(self.u32(o + k) for k in (0x10, 0x18)): return False
        for k in (0x14, 0x1C, 0x24):
            w = self.u32(o + k)
            if w and not self.inr(w): return False
        if vt - BASE + nv * 16 > self.n: return False
        return self.dl_walk(dl - BASE) is not None

    def analyse(self, roots):
        d = self.d
        # 1. model headers by structure
        self.models = []
        for o in range(0, self.n - 0x28, 4):
            if self.is_header(o):
                m = dict(hdr=o, nv=self.u16(o + 4), dl=self.u32(o + 8), vtx=self.u32(o + 0xC), tbl=self.u32(o + 0x14),
                         tex=self.u32(o + 0x1C), extra=self.u32(o + 0x20), bbox=self.u32(o + 0x24), vloads=[], flags=self.u32(o))
                self.models.append(m)
        for i, m in enumerate(self.models):
            m['name'] = 'model_%05X' % m['hdr']
            ref = roots.get(m['hdr'], 'pointer graph (model header pattern)')
            self.add(m['hdr'], m['hdr'] + 0x28, 'model_hdr', 'ModelHdr(0x28)', 'model header %s: %d vertices' % (m['name'], m['nv']), ref)
            self.add(m['vtx'] - BASE, m['vtx'] - BASE + 16 * m['nv'], 'vtx', 'Vtx[%d]' % m['nv'], 'vertex array of %s (segment 3)' % m['name'], 'model header +0x0C')
            if m['bbox']:
                self.add(m['bbox'] - BASE, m['bbox'] - BASE + 24, 'bbox', 'f32[6]', 'bounding box of %s (min xyz, max xyz)' % m['name'], 'model header +0x24')
            self.trace_dl(m['dl'] - BASE, m, 'model header +0x08 (%s)' % m['name'])
        # 2. u16 tables from header +0x14 (sized later by gap)
        for m in self.models:
            m['uniq'] = None
            if m['tbl']:
                t = self.parse_tbl(m['tbl'] - BASE, m['nv'])
                if t:
                    end, uniq = t; m['uniq'] = uniq
                    self.add(m['tbl'] - BASE, end, 'table', 'u16[%d]' % ((end - m['tbl'] + BASE) // 2),
                             'vertex expansion list of %s (header +0x14): {n, m, m x u16 dup index}...; %d unique vertices' % (m['name'], uniq), 'model header +0x14')
        # 2b. vertex morph animations (sprA format) placed right after a model header (code stores them in actor+0x1BC)
        for m in self.models:
            a = m['hdr'] + 0x28
            if m['uniq'] and self.is_anim(a):
                nk = self.d[a + 5]; do = self.u16(a + 2)
                self.add(a, a + do + 4 * nk * m['uniq'], 'morph_anim', 'MorphAnim(nkey=%d, nvert=%d)' % (nk, m['uniq']),
                         'keyframed vertex morph animation for %s (sprA format; %d ticks)' % (m['name'], self.d[a + 4]), 'follows model header ' + m['name'])
        # 3. collision lists: u32 count, ptr -> ptr[count] -> 0x50-byte records
        for o in range(0, self.n - 8, 4):
            c = self.u32(o); p = self.u32(o + 4)
            if 0 < c < 2000 and self.inr(p) and (p - BASE) + 4 * c <= self.n:
                arr = [self.u32(p - BASE + 4 * k) for k in range(c)]
                if all(self.inr(a) for a in arr) and all(arr[k + 1] - arr[k] == 0x50 for k in range(c - 1)):
                    if p - BASE != o + 4 - 4 * c and p - BASE + 4 * c != o: continue
                    self.add(o, o + 8, 'coll_hdr', 'CollHdr(8)', 'collision quad list header: %d quads' % c, roots.get(o, 'pointer graph (count+pointer pattern)'))
                    self.add(p - BASE, p - BASE + 4 * c, 'ptr_table', 'u32ptr[%d]' % c, 'pointers to collision quads', 'collision header 0x%X' % o)
                    for a in arr:
                        self.add(a - BASE, a - BASE + 0x50, 'coll_quad', 'CollQuad(0x50)', 'collision quad (20 x f32)', 'pointer table 0x%X' % (p - BASE))
        # 4. level display lists (inflated .bif images at 0x80154000) that load textures / call lists in this pack
        for (rng, img) in self.levels:
            self.scan_level(img, rng)
        # 5. code / .data roots (LEVEL_REFS): pair TLUT + CI texels the way func_80004010 [Number func_80003E18] does
        self.coderefs = {o: r for o, r in roots.items() if 0 <= o < self.n}
        self.fill()

    def scan_level(self, img, rng):
        lb = 0x80154000; tag = 'level DL in gzip member ROM 0x%X' % rng[0]
        for o in range(0, len(img) - 8, 8):
            w0, w1 = struct.unpack_from('>II', img, o)
            op = w0 >> 24
            if not self.inr(w1): continue
            if op == 0x06 and (w0 & 0xFFFFFF) in (0, 0x10000):
                self.trace_dl(w1 - BASE, None, tag)
            elif op == 0x04 and (w0 & 0x3FF) + 1 == ((w0 >> 10) & 0x3F) * 16:
                n = (w0 >> 10) & 0x3F
                self.add(w1 - BASE, w1 - BASE + 16 * n, 'vtx', 'Vtx[%d]' % n, 'vertex array (absolute load)', tag)
            elif op == 0xFD and (w0 & 0x0007F000) == 0:
                cmds = []
                for q in range(o, min(o + 8 * 24, len(img) - 7), 8):
                    v0, v1 = struct.unpack_from('>II', img, q)
                    if q > o and (v0 >> 24) in (0xFD, 0xB8, 0x06, 0x04, 0xB1, 0xBF): break
                    cmds.append((v0, v1))
                self.tex_state(cmds, None, tag)

    def fill(self):
        """Resolve overlaps and classify every gap."""
        objs = sorted(self.objs.values(), key=lambda o: (o['start'], -o['end']))
        out = []; pos = 0
        for o in objs:
            if o['start'] < pos:
                if o['end'] <= pos:
                    continue
                o = dict(o); o['start'] = pos
            if o['start'] > pos:
                out += self.gap(pos, o['start'])
            out.append(o); pos = o['end']
        if pos < self.n: out += self.gap(pos, self.n)
        res = []
        for o in out:
            if res and res[-1].get('grow') and o['cls'] == 'unknown' and not o.get('rooted'):
                res[-1]['end'] = o['end']; res[-1]['fmt'] = 'u16[%d]' % ((res[-1]['end'] - res[-1]['start']) // 2); continue
            res.append(o)
        # code-referenced objects inside model-owned objects are noted on that object
        for o in res:
            hits = [r for k, r in self.coderefs.items() if o['start'] <= k < o['end']]
            for k, r in self.coderefs.items():
                if o['start'] < k < o['end'] and r not in o['ref']:
                    o['ref'].append('+0x%X: %s' % (k - o['start'], r))
                elif k == o['start'] and r not in o['ref']:
                    o['ref'].insert(0, r)
        self.map = res

    def gap(self, s, e):
        """Split a gap at code roots and at zero runs of >= 32 bytes; classify each piece."""
        d = self.d; cuts = sorted({s, e} | {k for k in self.coderefs if s < k < e})
        out = []
        for a, b in zip(cuts, cuts[1:]):
            if a in self.coderefs:
                cls, fmt, desc = self.classify(a, b, True)
                out.append(dict(start=a, end=b, cls=cls, fmt=fmt, desc=desc, rooted=True, ref=[self.coderefs[a]]))
                continue
            i = a
            while i < b:
                j = i
                while j < b and d[j] == 0: j += 1
                if j > i and (j - i >= 32 or j == b or i == a and False):
                    if j - i >= 32 or (j == b and i > a):
                        out.append(dict(start=i, end=j, cls='pad', fmt='zero', desc='zero fill / alignment', ref=['-'])); i = j; continue
                k = j
                while k < b:
                    z = k
                    while z < b and d[z] == 0: z += 1
                    if z - k >= 32 or (z == b and z > k and k > i): break
                    k = z if z > k else k + 1
                k = max(k, i + 1)
                cls, fmt, desc = self.classify(i, k, a == i and i in self.coderefs)
                out.append(dict(start=i, end=k, cls=cls, fmt=fmt, desc=desc, rooted=i in self.coderefs,
                                ref=[self.coderefs.get(i, 'not referenced by a pack pointer, a level DL or code (unused?)')]))
                i = k
        return out

    def classify(self, s, e, rooted):
        n = e - s
        if rooted:
            nxt = min([k for k in self.coderefs if k > s] + [self.n])
            if n in (0x20, 0x200) or nxt - s in (0x20, 0x200) and n <= nxt - s:
                return 'tlut', 'RGBA16 palette[%d]' % (n // 2), 'texture palette loaded by code (gsDPLoadTLUT, 16 or 256 colours)'
            return 'texture', 'code-set format', 'texels loaded by code-built display list (format/size chosen in code: see FORMAT.md)'
        return 'unknown', 'raw', self.guess(s, e)

    def guess(self, s, e):
        d = self.d[s:e]; n = len(d)
        if n >= 8 and all(d[k:k + 8] == b'\xb8' + b'\0' * 7 for k in range(0, n - 7, 8)) and n % 8 == 0:
            return 'empty display list (gsSPEndDisplayList)'
        ptrs = sum(1 for k in range(0, n - 3, 4) if self.inr(struct.unpack_from('>I', d, k)[0]))
        flt = sum(1 for k in range(0, n - 3, 4) if 0x3A000000 <= (struct.unpack_from('>I', d, k)[0] & 0x7FFFFFFF) <= 0x47000000)
        if n >= 64 and flt > n // 8: return 'float data (%d plausible f32 of %d words), %d internal pointers' % (flt, n // 4, ptrs)
        if n % 16 == 0 and n >= 32 and all(d[k + 6:k + 8] == b'\0\0' for k in range(0, n, 16)):
            return 'probably an unreferenced Vtx array (%d x 16 bytes, flag words 0)' % (n // 16)
        if n in (0x20, 0x200, 0x220): return 'probably an unreferenced RGBA16 palette (%d entries)' % (n // 2)
        if n >= 512: return 'unreferenced image-like data (texels/palettes reached only from code or level DLs), %d internal pointers' % ptrs
        return 'small unreferenced data, %d internal pointers' % ptrs


# ---------------------------------------------------------------- output
def vtx_rows(d, o, n):
    rows = []
    for k in range(n):
        x, y, z, f, s, t, r, g, b, a = struct.unpack_from('>hhhHhhBBBB', d, o + 16 * k)
        rows.append([x, y, z, f, s, t, r, g, b, a])
    return rows


def decode_tex(fmt, bits, w, h, data, pal=None):
    px = []
    for i in range(w * h):
        if bits == 4: v = (data[i >> 1] >> (0 if i & 1 else 4)) & 15
        elif bits == 8: v = data[i]
        elif bits == 16: v = (data[2 * i] << 8) | data[2 * i + 1]
        else: v = int.from_bytes(data[4 * i:4 * i + 4], 'big')
        if fmt == 'RGBA' and bits == 16:
            c = (((v >> 11) & 31) * 255 // 31, ((v >> 6) & 31) * 255 // 31, ((v >> 1) & 31) * 255 // 31, 255 if v & 1 else 0)
        elif fmt == 'RGBA' and bits == 32: c = (v >> 24, (v >> 16) & 255, (v >> 8) & 255, v & 255)
        elif fmt == 'IA' and bits == 16: c = (v >> 8,) * 3 + (v & 255,)
        elif fmt == 'IA' and bits == 8: c = ((v >> 4) * 17,) * 3 + ((v & 15) * 17,)
        elif fmt == 'IA' and bits == 4: c = ((v >> 1) * 36,) * 3 + (255 if v & 1 else 0,)
        elif fmt == 'I': c = ((v * 17) if bits == 4 else v,) * 3 + (255,)
        elif fmt == 'CI' and pal:
            p = pal[v] if v < len(pal) else 0
            c = (((p >> 11) & 31) * 255 // 31, ((p >> 6) & 31) * 255 // 31, ((p >> 1) & 31) * 255 // 31, 255 if p & 1 else 0)
        else: c = ((v * 255 // ((1 << bits) - 1)),) * 3 + (255,)
        px.append(c)
    return _Png(w, h, px)


class _Png:
    def __init__(self, w, h, px): self.w, self.h, self.px = w, h, px
    def save(self, path):
        import zlib
        raw = b''.join(b'\0' + bytes(c for p in self.px[y * self.w:(y + 1) * self.w] for c in p) for y in range(self.h))
        def chunk(t, b): return struct.pack('>I', len(b)) + t + b + struct.pack('>I', zlib.crc32(t + b) & 0xFFFFFFFF)
        open(path, 'wb').write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', self.w, self.h, 8, 6, 0, 0, 0)) +
                               chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))


def build(rom, game):
    s, e = FILES[game]
    p = Pack(rom[s:e], game, level_images(rom, game))
    roots = load_refs(game)
    p.analyse(roots)
    return p, s


def map_rows(p, s, game):
    rows = []
    for o in p.map:
        ext = 'extract_packC.py -> %s' % {'dl': 'dl/*.gbi.txt', 'vtx': 'vtx/*.json', 'model_hdr': 'tables/*.json',
                                           'bbox': 'tables/*.json', 'table': 'tables/*.json', 'coll_hdr': 'tables/*.json',
                                           'ptr_table': 'tables/*.json', 'coll_quad': 'tables/*.json', 'morph_anim': 'tables/*.json', 'texture': 'tex/*.bin+png',
                                           'tlut': 'tex/*.bin', 'pad': '(zero; checked)', 'unknown': 'raw/*.bin'}.get(o['cls'], 'raw/*.bin')
        rows.append('\t'.join([game, '0x%06X' % (s + o['start']), '0x%06X' % (s + o['end']), o['cls'], o['fmt'],
                               '%s [RAM 0x%08X]' % (o['desc'], BASE + o['start']), '; '.join(o['ref'][:3]), ext]))
    return rows


def write_obj(p, m, path):
    d = p.d; vb = [None] * 64; V = []; idx = {}; faces = []
    def walk(off, depth=0):
        r = p.dl_walk(off)
        if not r: return
        for (q, w0, w1) in r[1]:
            op = w0 >> 24
            if op == 0x06 and p.inr(w1) and depth < 16: walk(w1 - BASE, depth + 1)
            elif op == 0x04:
                n = (w0 >> 10) & 0x3F; v0 = ((w0 >> 16) & 0xFF) // 2
                base = (m['vtx'] - BASE + (w1 & 0xFFFFFF)) if (w1 >> 24) == 3 else (w1 - BASE if p.inr(w1) else None)
                for k in range(n):
                    if v0 + k < 64: vb[v0 + k] = None if base is None else base + 16 * k
            elif op in (0xBF, 0xB1):
                tris = [((w1 >> 16) & 0xFF, (w1 >> 8) & 0xFF, w1 & 0xFF)] if op == 0xBF else \
                       [((w0 >> 16) & 0xFF, (w0 >> 8) & 0xFF, w0 & 0xFF), ((w1 >> 16) & 0xFF, (w1 >> 8) & 0xFF, w1 & 0xFF)]
                for t in tris:
                    f = []
                    for a in t:
                        va = vb[a // 2] if a // 2 < 64 else None
                        if va is None or va + 16 > p.n: f = None; break
                        if va not in idx: idx[va] = len(V) + 1; V.append(va)
                        f.append(idx[va])
                    if f: faces.append(f)
    walk(m['dl'] - BASE)
    if not faces: return 0
    with open(path, 'w') as fh:
        fh.write('# %s: model header file offset 0x%X\n' % (m['name'], m['hdr']))
        for va in V:
            x, y, z, _, s_, t_ = struct.unpack_from('>hhhHhh', d, va)
            fh.write('v %d %d %d\n' % (x, y, z))
        for va in V:
            _, _, _, _, s_, t_ = struct.unpack_from('>hhhHhh', d, va)
            fh.write('vt %.5f %.5f\n' % (s_ / 32.0 / 32.0, 1 - t_ / 32.0 / 32.0))
        for f in faces: fh.write('f %s\n' % ' '.join('%d/%d' % (i, i) for i in f))
    return len(faces)


def extract(rom, game, out):
    p, s = build(rom, game)
    for sub in ('dl', 'vtx', 'tables', 'tex', 'raw', 'obj'):
        os.makedirs(os.path.join(out, sub), exist_ok=True)
    d = p.d; index = []; nobj = 0
    for o in p.map:
        a, b, c = o['start'], o['end'], o['cls']; name = '%05X_%s' % (a, c)
        if c == 'dl' and (b - a) % 8 == 0:
            with open(os.path.join(out, 'dl', name + '.gbi.txt'), 'w') as fh:
                fh.write('// RAM 0x%08X\n' % (BASE + a))
                for q in range(a, b, 8): fh.write(dis(*struct.unpack_from('>II', d, q)) + '\n')
            f = 'dl/' + name + '.gbi.txt'
        elif c == 'vtx' and (b - a) % 16 == 0:
            json.dump(vtx_rows(d, a, (b - a) // 16), open(os.path.join(out, 'vtx', name + '.json'), 'w'))
            f = 'vtx/' + name + '.json'
        elif c in ('model_hdr', 'coll_hdr', 'ptr_table', 'bbox', 'coll_quad', 'morph_anim') and (b - a) % 4 == 0:
            if c == 'bbox' or c == 'coll_quad':
                vals = ['%r' % x for x in struct.unpack('>%df' % ((b - a) // 4), d[a:b])]
                # floats round-trip exactly through repr; keep raw words for NaN-safety
                rec = {'f32': vals, 'u32': ['0x%08X' % x for x in struct.unpack('>%dI' % ((b - a) // 4), d[a:b])]}
            else:
                rec = {'u32': ['0x%08X' % x for x in struct.unpack('>%dI' % ((b - a) // 4), d[a:b])]}
                if c == 'model_hdr':
                    rec['fields'] = {'flags': rec['u32'][0], 'nverts': int(rec['u32'][1], 16) >> 16, 'dl': rec['u32'][2], 'vtx': rec['u32'][3],
                                     'u16_table': rec['u32'][5], 'tex_seg4': rec['u32'][7], 'runtime_buf_size': int(rec['u32'][8], 16), 'bbox': rec['u32'][9]}
            json.dump(rec, open(os.path.join(out, 'tables', name + '.json'), 'w'), indent=0)
            f = 'tables/' + name + '.json'
        elif c == 'table' and (b - a) % 2 == 0:
            json.dump({'u16': list(struct.unpack('>%dH' % ((b - a) // 2), d[a:b]))}, open(os.path.join(out, 'tables', name + '.json'), 'w'))
            f = 'tables/' + name + '.json'
        elif c == 'pad':
            f = None
        else:
            open(os.path.join(out, 'raw' if c not in ('texture', 'tlut') else 'tex', name + '.bin'), 'wb').write(d[a:b])
            f = ('tex/' if c in ('texture', 'tlut') else 'raw/') + name + '.bin'
            if c == 'texture' and o.get('img'):
                fmt, bits, w, h, pal = o['img']
                if w > 0 and h > 0 and w * h * bits // 8 <= b - a:
                    pl = None
                    if pal is not None:
                        pl = [struct.unpack_from('>H', d, pal + 2 * k)[0] for k in range(1 << bits) if pal + 2 * k + 2 <= len(d)]
                    try: decode_tex(fmt, bits, w, h, d[a:b], pl).save(os.path.join(out, 'tex', name + '.png'))
                    except Exception as ex: print('png fail', name, ex)
        index.append({'start': a, 'end': b, 'cls': c, 'file': f})
    nf = 0
    for m in p.models:
        nf += write_obj(p, m, os.path.join(out, 'obj', m['name'] + '.obj'))
    json.dump({'game': game, 'rom_start': s, 'size': p.n, 'base': BASE, 'objects': index}, open(os.path.join(out, 'index.json'), 'w'), indent=0)
    open(os.path.join(out, 'MAP_%s.tsv' % game), 'w').write('\n'.join(map_rows(p, s, game)) + '\n')
    cnt = collections.Counter(); byt = collections.Counter()
    for o in p.map: cnt[o['cls']] += 1; byt[o['cls']] += o['end'] - o['start']
    print('game', game, 'objects', len(p.map), 'models', len(p.models), 'obj faces', nf)
    for k in sorted(byt): print('  %-10s %5d objs %8d bytes' % (k, cnt[k], byt[k]))
    for n_ in p.notes[:20]: print('  note:', n_)


def check(rom, game, out):
    s, e = FILES[game]; orig = rom[s:e]
    idx = json.load(open(os.path.join(out, 'index.json')))
    buf = bytearray(len(orig)); pos = 0
    for o in idx['objects']:
        a, b, c, f = o['start'], o['end'], o['cls'], o['file']
        assert a == pos, 'gap at 0x%X' % pos
        if f is None: chunk = bytes(b - a)
        elif f.endswith('.gbi.txt'):
            chunk = b''.join(struct.pack('>II', *asm(l)) for l in open(os.path.join(out, f)) if l.strip() and not l.startswith('//'))
        elif f.startswith('vtx/'):
            chunk = b''.join(struct.pack('>hhhHhhBBBB', *r) for r in json.load(open(os.path.join(out, f))))
        elif f.startswith('tables/'):
            j = json.load(open(os.path.join(out, f)))
            if 'u16' in j: chunk = struct.pack('>%dH' % len(j['u16']), *j['u16'])
            elif 'f32' in j and 'nan' not in ''.join(j['f32']).lower():
                chunk = struct.pack('>%df' % len(j['f32']), *[float(x) for x in j['f32']])
                if chunk != struct.pack('>%dI' % len(j['u32']), *[int(x, 16) for x in j['u32']]): chunk = b'!'
            else: chunk = struct.pack('>%dI' % len(j['u32']), *[int(x, 16) for x in j['u32']])
        else: chunk = open(os.path.join(out, f), 'rb').read()
        buf[a:b] = chunk; pos = b
        if len(chunk) != b - a: print('size mismatch', hex(a), f); return 1
    ok = bytes(buf) == orig and pos == len(orig)
    bad = [i for i in range(len(orig)) if buf[i] != orig[i]][:5]
    print('check', game, 'OK' if ok else 'FAIL', 'bytes', len(orig), 'first diffs', [hex(x) for x in bad])
    return 0 if ok else 1


if __name__ == '__main__':
    a = sys.argv[1:]
    if a[0] == 'check':
        sys.exit(check(open(a[1], 'rb').read(), a[2], a[3]))
    if a[0] == 'map':
        p, s = build(open(a[1], 'rb').read(), a[2]); print('\n'.join(map_rows(p, s, a[2]))); sys.exit(0)
    extract(open(a[0], 'rb').read(), a[1], a[2])
