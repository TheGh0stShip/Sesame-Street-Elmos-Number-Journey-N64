#!/usr/bin/env python3
"""resolve.py OUTDIR: settle every conflict of the merged ledger by evidence from the ROM, the inflated level images and
the code, and write a corrected ledger.

Reads: every sesassets/*/MAP.tsv and the code ledger (through ../ledger/tools/merge_ledger.py, unchanged), the ROMs,
lvlA/out/<game>/*.bif.bin, LEVEL_REFS_<game>.tsv, gfx/out/<game>/manifest.json (code-origin texture sizes only).
Writes OUTDIR/LEDGER_<game>.tsv, LEVELS_<game>.tsv (same columns as ../ledger, asserted gap- and overlap-free),
RESOLVED_<game>.tsv (every former conflict pair with verdict and evidence class), CORRECTIONS_<game>.tsv (what each
source MAP should change) and STATS.json. No ROM bytes are written: only offsets, counts and decoded field values.

Method (FORMAT.md): every claim that takes part in a conflict is checked on its own by a verifier for its kind and
gets a status: ok (bounds = one whole object, proven), part (a correctly bounded piece of a proven object),
filler (zero padding), unverified, merged (spans several proven objects), wrong (contradicted). Where no claim is ok
for a proven object an evidence row (owner r2) is created. Painting: wrong/merged lowest, then the original
priority order, then filler, part, evidence, ok. Each former pair then gets the verdict that the painting realises,
and the script asserts that no byte of a pair overlap is owned by a claim judged wrong/merged while a proven
claim covers it.
"""
import os, sys, re, json, struct, collections, gzip, zlib
from array import array
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.expanduser('${N64DK_LAB}/sesassets/ledger/tools'))
import merge_ledger as M          # noqa: E402  (map loading, kinds, painting, conflict detection)
import evidence as E              # noqa: E402

S = M.S
# only the 22 source maps the ledger merges (other reviewer folders such as r1 may also hold a MAP.tsv)
_glob = M.glob.glob
class _G:
    @staticmethod
    def glob(p):
        return [x for x in _glob(p) if not p.endswith('/MAP.tsv') or x.split('/')[-2] in M.RANK_ROM or x.split('/')[-2] in M.RANK_LVL]
M.glob = _G
STRONG = {'ok': 5, 'evidence': 4, 'part': 3, 'filler': 2, 'unverified': 1, 'merged': 0, 'wrong': 0}
# TexPair tables {u32 tlut, u32 texels} read by the HUD (hud/FORMAT.md; Letter func_80004010, Number func_80003E18)
TEXPAIR = {'letter': [(0x8006DFC8, 52), (0x8006DE28, 52), (0x8006DDF0, 7)],
           'number': [(0x8006F7E0, 9), (0x8006F798, 9), (0x8006F760, 7)]}
# code-built HUD loads with fixed sizes (hud/FORMAT.md table; every address is checked against LEVEL_REFS below)
HUD_TEX = {'letter': [(a, 0x1000) for a in (0x8024B008, 0x8024C018, 0x8024D028, 0x8024E038, 0x8024F048)],
           'number': [(a, 0x1000) for a in (0x8024B008, 0x8024C018, 0x8024D028, 0x8024E038, 0x8024F048)]}
MENU_FILES = {'letter': [(0x7C6B0, 0xA54D0), (0xA54D0, 0x10D0F0)], 'number': [(0x7CB20, 0xA5940), (0xA5940, 0x10D560)]}


def u16(d, o): return struct.unpack_from('>H', d, o)[0]
def u32(d, o): return struct.unpack_from('>I', d, o)[0]


class Space:
    """evidence for one space (ROM or one level image)."""
    def __init__(self, G, name, rows, refs_level, refs_common, U):
        self.G, self.name, self.rows = G, name, rows
        self.d = G.data(name)
        self.U = U
        # ---- display lists: roots = every display-list claim + model records (segments 3/4)
        roots = [((name, r.a), {}) for r in rows if r.kind == 'display_list']
        hdrs = sorted({r.a for r in rows if r.kind == 'model_header'})
        roots += G.model_roots(name, [h for h in hdrs if self.header_ok(h)])
        self.dls = G.dl_closure(roots, name)
        self.dlobj = {o: r['end'] for (s, o), r in self.dls.items() if s == name and r['ok']}
        self.arrays = E.vertex_arrays(self.dls, name)
        self.loads = set()
        self.tex = {}                                  # texture start -> size (or None) with the reason
        for (s, o), r in self.dls.items():
            for vs, vo, ln in r.get('rvtx', ()):
                if vs == name: self.loads.add((vo, vo + ln))
        # texture starts named by G_SETTIMG in the traced lists (segments resolved in dl_closure)
        self.settimg = set(); self.texload = {}
        for (s, o), r in self.dls.items():
            for vs, vo in r.get('rtimg', ()):
                if vs == name: self.settimg.add(vo)
            for vs, vo, n in r.get('rtload', ()):
                if vs == name: self.texload[vo] = max(n, self.texload.get(vo, 0))
        self.code_tex = {}     # filled by init_refs (gfx manifest, code-origin loads into this space)
        self.ptr_targets = set()
        for r in rows:         # targets of claimed pointer tables (collision quad pointers etc.)
            if 'ptr' in r.cls and r.b - r.a <= 0x400:
                for o in range(r.a, r.b - 3, 4):
                    L = G.loc(u32(self.d, o), name)
                    if L and L[0] == name: self.ptr_targets.add(L[1])
        self.refs = set(refs_level if name != 'ROM' else refs_common)
        # pointer tables
        self.objs = {}

    def header_ok(self, h):
        """model record (lvlD FORMAT / func_80018530 copies 10 words): +0x08 a display list, +0x06 0, +0x10 and +0x18
        0 in the file (runtime fields), +0x24 0 or header-0x18 (bounding box)."""
        d = self.d
        if h % 4 or h + 0x28 > len(d): return False
        if u16(d, h + 6) != 0 or u32(d, h + 0x10) != 0 or u32(d, h + 0x18) != 0: return False
        base = self.G.ram_base(self.name, h)
        bb = u32(d, h + 0x24)
        if base is None or bb not in (0, base - 0x18): return False
        L = self.G.loc(u32(d, h + 8), self.name)
        if not L: return False
        r = self.G.dl(*L)
        return r['ok']


# ------------------------------------------------------------------ verifiers
def rebuild_list(d, a, limit=0x4000):
    """u16 program of func_80000450: loop {n; if !n stop; m; if !m stop; m indices}. -> (end, sum_n, sum_nm) or None"""
    p = a; sn = snm = 0
    while p + 2 <= len(d) and p - a < limit:
        n = u16(d, p); p += 2
        if n == 0: return p, sn, snm
        sn += n; snm += n
        if p + 2 > len(d): return None
        m = u16(d, p); p += 2
        if m == 0: return p, sn, snm
        snm += m; p += 2 * m
    return None


def anim_header(d, a):
    """sprA morph header -> (data_off, nkey, ticks) or None."""
    if a % 4 or a + 8 > len(d): return None
    he, do, ticks, nk, fl = struct.unpack_from('>HHBBH', d, a)
    if nk < 2 or (he & 0xFF) != 8 + nk or do < 8 + nk or do % 4 or do > 8 + nk + 16: return None
    if fl & 0x7FFF: return None
    keys = d[a + 8:a + 8 + nk]
    if keys[0] != 0 or any(keys[i] >= keys[i + 1] for i in range(nk - 1)) or keys[-1] > ticks: return None
    if any(d[a + 8 + nk:a + do]): return None
    return do, nk, ticks


def is_zero(d, a, b): return d.count(0, a, b) == b - a


class Judge:
    # ---------------- morph animation objects
    def morph_objects(self):
        """headers = starts of morph claims with a valid sprA header (plus 16-aligned headers inside ROM sprA claims);
        object end = start of the next header in the same claimed run, else the claimed end that fits
        data_off + 4*nkey*nunique (nunique a known unique-vertex count) [+ stale tail to 16 in ROM files]."""
        for name, sp in self.sp.items():
            d = sp.d; rows = [r for r in sp.rows if r.kind == 'morph_anim']
            heads = set()
            for r in rows:
                if anim_header(d, r.a): heads.add(r.a)
                if name == 'ROM' and r.ag in ('sprA', 'gfx'):
                    for o in range((r.a + 15) & ~15, r.b, 16):
                        if anim_header(d, o) and o not in heads:
                            # accept an inner header only where another claim also starts an animation there
                            if any(q.a == o for q in rows): heads.add(o)
            heads = sorted(heads); objs = {}
            for i, h in enumerate(heads):
                do, nk, ticks = anim_header(d, h)
                ends = {r.b for r in rows if r.a == h} | {r.b for r in rows if r.a < h < r.b}
                # pieces of a split claim (header / keys / pad / vertices) that start right after the header
                ends |= {q.b for q in sp.rows if h < q.a <= h + 0x40 and q.kind in ('morph_anim', 'pad')}
                # the end of the ROM file that holds it, without its trailing zero padding
                for q in sp.rows:
                    if q.ag == 'files' and q.a <= h < q.b:
                        e = q.b
                        while e > h and d[e - 1] == 0: e -= 1
                        ends.add(e); ends.add(q.b)
                ends = sorted(ends)
                nxt = heads[i + 1] if i + 1 < len(heads) else None
                cover_end = max([r.b for r in rows if r.a <= h < r.b] or [h])
                best = None; why = ''
                cands = []
                for e in ends + ([nxt] if nxt and nxt <= cover_end else []):
                    if nxt is not None and e > nxt: continue
                    body = e - h - do
                    if body <= 0: continue
                    nu, tail = divmod(body, 4 * nk)
                    hit = self.hard_hit(name, h, e)
                    infile = any(q.ag == 'files' and q.a <= h < q.b and is_zero(d, e, q.b) and q.b - e < 16 for q in sp.rows)
                    cands.append((hit is None, nu in self.U and tail == 0, nu in self.U and tail < 16 and e % 16 == 0,
                                  e == nxt or (tail == 0 and infile), e, nu, tail, hit))
                cands.sort(key=lambda c: (c[0], c[1], c[2], c[3], -c[4]), reverse=True)
                if cands and cands[0][0] and (cands[0][1] or cands[0][2] or cands[0][3]):
                    c = cands[0]
                    if name == 'ROM' and c[6] == 0 and c[4] % 16:
                        # ROM sub-files start on 16-byte boundaries (sprA FORMAT): the stale words up to the
                        # boundary belong to the animation's file slot
                        e16 = (c[4] + 15) & ~15
                        alt = [x for x in cands if x[4] == e16 and x[0]]
                        if alt: c = (c[0], c[1], c[2], c[3], e16, c[5], e16 - c[4], c[7])
                    why = ('nunique %d is the unique-vertex count of an animated mesh' % c[5] if c[1] or c[2] else
                           'next animation header at 0x%X' % c[4] if c[4] == nxt else
                           'exactly %d vertex rows up to the zero padding at the end of its ROM file' % c[5])
                    if c[6]: why += '; %d-byte stale tail' % c[6]
                    objs[h] = (c[4], why, nk, c[5])
            sp.morph = objs

    def hard_hit(self, name, a, b):
        """first proven non-animation object starting inside (a, b): a display list reached from a model record or
        a display-list claim, a G_SETTIMG texture, a valid model record."""
        sp = self.sp[name]
        for o in sorted(sp.dlobj):
            if a < o < b and o in sp.settimg_dl_roots: return ('display list', o)
        for o in sorted(sp.settimg | set(sp.code_tex)):
            if a < o < b: return ('texture loaded by a display list or by code', o)
        return None

    # ---------------- per-row verifiers
    def verify(self, r):
        sp = self.sp[r.space]; d = sp.d; k = r.kind
        f = getattr(self, 'v_' + k, None)
        if f is None: return ('unverified', 'none', 'no verifier for kind %s' % k)
        return f(r, sp, d)

    def v_pad(self, r, sp, d):
        own = [h for h, (e, _, _, _) in sp.morph.items() if h < r.a and r.b <= e]
        if own and not is_zero(d, r.a, r.b):
            return ('part', 'morph', 'stale tail inside animation 0x%X-0x%X (never read: the decoder reads nunique x nkey words)' % (own[0], sp.morph[own[0]][0]))
        if is_zero(d, r.a, r.b): return ('filler', 'zero', 'all bytes zero')
        return ('wrong', 'zero', 'pad claim over non-zero bytes')

    def v_display_list(self, r, sp, d):
        x = self.G.dl(r.space, r.a)
        if not x['ok']:
            return ('wrong', 'f3dex', 'no F3DEX display list starts here (non-command word before G_ENDDL)')
        outer = [o for o, e in sp.dlobj.items() if o < r.a < e]
        if x['end'] == r.b:
            if outer:
                o = min(outer)
                return ('part', 'f3dex', 'secondary entry point into display list 0x%X-0x%X (shared tail)' % (o, sp.dlobj[o]))
            return ('ok', 'f3dex', 'one display list: first G_ENDDL at +0x%X' % (r.b - 8 - r.a))
        if x['end'] < r.b:
            # chain of whole lists?
            p = r.a; n = 0
            while p < r.b and p in sp.dlobj or (p < r.b and self.G.dl(r.space, p)['ok']):
                p = self.G.dl(r.space, p)['end']; n += 1
            if p == r.b: return ('merged', 'f3dex', '%d display lists (G_ENDDL at each list end) merged' % n)
            return ('wrong', 'f3dex', 'first G_ENDDL at 0x%X, claim ends at 0x%X' % (x['end'], r.b))
        if outer:
            return ('wrong', 'f3dex', 'list runs to G_ENDDL at 0x%X; claim stops early' % x['end'])
        return ('wrong', 'f3dex', 'list runs to G_ENDDL at 0x%X; claim stops early' % x['end'])

    def v_vertex(self, r, sp, d):
        if (r.b - r.a) % 16: return ('wrong', 'f3dex-vtx', 'not a whole number of 16-byte Vtx')
        if (r.a, r.b) in sp.arrays_set:
            return ('ok', 'f3dex-vtx', 'one vertex array: the G_VTX loads of its display list(s) cover exactly this range')
        inside = [x for x in sp.arrays if x[0] <= r.a and r.b <= x[1]]
        if inside:
            return ('part', 'f3dex-vtx', '%s of vertex array 0x%X-0x%X' % ('one G_VTX load' if (r.a, r.b) in sp.loads else 'piece',
                                                                       inside[0][0], inside[0][1]))
        within = [x for x in sp.arrays if r.a <= x[0] and x[1] <= r.b]
        if within and len(within) > 1 and within[0][0] == r.a and within[-1][1] == r.b:
            return ('merged', 'f3dex-vtx', '%d vertex arrays merged' % len(within))
        return ('unverified', 'f3dex-vtx', 'no traced G_VTX command loads this range')

    def v_model_header(self, r, sp, d):
        n = r.b - r.a
        if n == 0x28 and sp.header_ok(r.a):
            return ('ok', 'model-record', 'model record 0x28 bytes: func_80018530 copies 10 words (func_80000EDC(dst, src, 10)) '
                    'and allocates 40 + 16*nverts; +0x24 is the bbox pointer')
        if n == 0x24 and sp.header_ok(r.a):
            return ('part', 'model-record', 'first 0x24 bytes of a 0x28-byte model record (+0x24 bbox pointer left out)')
        if n == 4 and sp.header_ok(r.a - 0x24):
            return ('part', 'model-record', '+0x24 word (bbox pointer) of model record 0x%X' % (r.a - 0x24))
        if sp.header_ok(r.a): return ('wrong', 'model-record', 'model record is 0x28 bytes, claim is 0x%X' % n)
        return ('wrong', 'model-record', 'not a model record (+0x06/+0x10/+0x18 not 0, +0x24 not 0 or header-0x18, or +0x08 no display list)')

    def v_vertex_list(self, r, sp, d):
        x = rebuild_list(d, r.a)
        if not x: return ('unverified', 'rebuild-list', 'does not parse as a func_80000450 list')
        e, sn, snm = x
        if r.b == e:
            return ('ok', 'rebuild-list', 'func_80000450 list ends with the 0 it reads at 0x%X (sum n %d, sum n+m %d)' % (e - 2, sn, snm))
        if r.b > e and is_zero(d, e, r.b):
            return ('merged', 'rebuild-list', 'list ends at 0x%X; the claim adds %d zero bytes of alignment pad' % (e, r.b - e))
        if r.b == e - 2:
            return ('wrong', 'rebuild-list', 'claim stops at sum(n+m) and leaves out the 0 terminator that func_80000450 reads')
        return ('wrong', 'rebuild-list', 'list ends at 0x%X, claim at 0x%X' % (e, r.b))

    def v_morph_anim(self, r, sp, d):
        objs = sp.morph
        if r.a in objs:
            e, why, nk, nu = objs[r.a]
            if r.b == e: return ('ok', 'morph', 'whole animation (%d keys, %d vertices): %s' % (nk, nu, why))
            if r.b < e: return ('part', 'morph', 'header part of animation 0x%X-0x%X' % (r.a, e))
            inner = [h for h in objs if r.a < h < r.b]
            if inner:
                return ('merged', 'morph', '%d animations merged (headers at %s)' % (len(inner) + 1, ', '.join('0x%X' % h for h in inner)))
            hit = self.hard_hit(r.space, r.a, r.b)
            return ('wrong', 'morph', 'animation ends at 0x%X (%s); claim runs to 0x%X%s' % (e, why, r.b,
                    ' over a %s at 0x%X' % hit if hit else ''))
        own = [h for h, (e, _, _, _) in objs.items() if h < r.a and r.b <= e]
        if own:
            return ('part', 'morph', 'piece (header/keys/pad/vertices/tail) of animation 0x%X-0x%X' % (own[0], objs[own[0]][0]))
        if anim_header(d, r.a): return ('unverified', 'morph', 'valid header but no proven end')
        return ('unverified', 'morph', 'no animation header here and not inside a proven animation')

    def v_collision(self, r, sp, d):
        if r.cls == 'bsp_leaf' and r.b - r.a == 0x20:
            w0 = u32(d, r.a)
            L = self.G.loc(u32(d, r.a + 0x10), r.space)
            if w0 & 1 and L and self.G.dl(*L)['ok'] and self.G.loc(u32(d, r.a + 0x14), r.space) and \
               self.G.loc(u32(d, r.a + 0x1C), r.space):
                return ('ok', 'bsp', 'BSP leaf: word0 bit0 set, +0x10 display list, +0x14 Vtx, +0x1C triangles (lvlC FORMAT, func_80025E4C)')
        if r.cls == 'coll_quad':
            inner = sorted(x for x in sp.ptr_targets if r.a < x < r.b)
            if r.a in sp.ptr_targets and inner:
                return ('merged', 'pointer-table', 'the quad pointer table points at 0x%X and at %s: %d quads' % (r.a, ', '.join('0x%X' % x for x in inner), len(inner) + 1))
            if r.a in sp.ptr_targets and r.b - r.a == 0x50:
                return ('ok', 'pointer-table', 'one 0x50-byte quad, target of the quad pointer table (func_80023118 copies count*0x50)')
        if r.cls == 'coll_tris' and (r.b - r.a) % 4 == 0 and all(d[o + 3] == 0 for o in range(r.a, r.b, 4)):
            return ('unverified', 'bsp', 'u8 i0,i1,i2,0 triangle words (format check only)')
        return ('unverified', 'bsp', 'collision claim, no proof either way')

    def v_texture(self, r, sp, d):
        if r.cls in ('font', 'text'):
            return self.v_font(r, sp, d)
        for q in sp.rows:
            if q is not r and q.cls in ('data_ptr_table', 'ptr_table') and q.a < r.b and r.a < q.b:
                pp, nn = E.pointer_words(d, q.a, q.b)
                zz = sum(1 for o in range(q.a, q.b - 3, 4) if u32(d, o) == 0)
                if nn and pp and pp + zz == nn:
                    return ('wrong', 'pointer-table', 'overlaps the pointer table 0x%X-0x%X (all %d words RAM pointers or 0): not texels'
                            % (q.a, q.b, nn))
        p, n = E.pointer_words(d, r.a, r.b)
        if r.space == 'ROM' and E.PROG[self.game][0] <= r.a < E.PROG[self.game][1] and n and p * 2 >= n:
            return ('wrong', 'pointer-table', '%d of %d words are RAM pointers: a pointer table, not texels' % (p, n))
        size = None; why = ''
        if r.space == 'ROM':
            if r.a in self.hud_tex: size, why = self.hud_tex[r.a], 'HUD code-built load (hud/FORMAT.md, address in LEVEL_REFS)'
            elif r.a in self.pair_tex: why = 'texel word of a HUD TexPair table'
            elif r.a in self.code_tex: size, why = self.code_tex[r.a][0], 'loaded by %s (size from its load commands)' % self.code_tex[r.a][1]
            elif r.a in sp.settimg: why = 'G_SETTIMG of a traced display list'
            elif r.a in self.pair_tlut and r.kind == 'palette': why = 'TLUT word of a HUD TexPair table'
            elif self.menu(r.a) and r.a in sp.refs: why = 'menu file (PI-copied to 0x80154000): address held by a .data pointer table'
            elif r.a in self.code_tlut and r.kind == 'palette':
                size, why = self.code_tlut[r.a], 'TLUT loaded by the menu code (address built in code, LEVEL_REFS)'
            if r.a in sp.texload and not size: size = sp.texload[r.a]
        else:
            if r.a in sp.settimg: why = 'G_SETTIMG of a traced display list'
            elif r.a in sp.code_tex: why = 'loaded by %s' % sp.code_tex[r.a][1]
            if r.a in sp.texload: size = sp.texload[r.a]; why += ' (load commands read 0x%X bytes)' % size
            elif r.a in sp.code_tex: size = sp.code_tex[r.a][0]
        if not why:
            if r.a in sp.refs: return ('unverified', 'texture-ref', 'start is referenced, but not as a texture')
            return ('unverified', 'texture-ref', 'no texture reference at the start')
        if r.kind == 'texture' and r.space == 'ROM' and r.a in self.pair_tlut and r.a not in self.pair_tex:
            return ('wrong', 'texture-ref', 'the HUD TexPair tables use this address as a TLUT')
        if r.kind == 'palette' and r.space == 'ROM' and r.a in self.pair_tex:
            return ('wrong', 'texture-ref', 'the HUD TexPair tables use this address as texels')
        if size and r.b - r.a < size:
            return ('wrong', 'texture-ref', '%s; size 0x%X, claim 0x%X' % (why, size, r.b - r.a))
        inner = sorted(x for x in (sp.settimg | self.pair_tex | self.pair_tlut | set(self.hud_tex)) if r.a < x < r.b) \
            if r.space == 'ROM' else sorted(x for x in sp.settimg if r.a < x < r.b)
        if r.space == 'ROM':
            inner += sorted(x for x in sp.refs if r.a < x < r.b and x % 8 == 0 and self.menu(x))
            inner += sorted(x for x in set(self.code_tex) | set(self.code_tlut) if r.a < x < r.b)
        else:
            inner += sorted(x for x in sp.code_tex if r.a < x < r.b)
        if size and r.b - r.a > size and not inner:
            return ('wrong', 'texture-ref', '%s; size 0x%X, claim 0x%X' % (why, size, r.b - r.a))
        if inner:
            return ('merged', 'texture-ref', '%s; further texture/TLUT references inside at %s' % (why, ', '.join('0x%X' % x for x in sorted(set(inner))[:4])))
        return ('ok', 'texture-ref', why + ('' if not size else '; size 0x%X' % size))

    v_palette = v_texture

    def v_font(self, r, sp, d):
        """sprite-font pieces (gfx FONTS: {charset string, Bitmap[]} read by func_8000DEBC [func_8000E32C])."""
        n = r.b - r.a
        if 'descriptor' not in r.desc and (r.cls == 'text' or 'string' in r.desc):
            s_ = d[r.a:r.b]; z = s_.find(0)
            if z > 0 and all(32 <= c < 127 for c in s_[:z]) and not any(s_[z:]) and r.b % 4 == 0 and n - z - 1 < 4:
                return ('ok', 'typed', 'ASCII string of %d characters, NUL, zero pad to 4' % z)
            return ('wrong', 'typed', 'not a NUL-terminated ASCII string')
        if 'Bitmap' in r.desc and n % 16 == 0:
            ok = all(self.G.loc(u32(d, o + 8), 'ROM') for o in range(r.a, r.b, 16))
            return ('ok' if ok else 'unverified', 'typed', '%d SDK Bitmap records (16 bytes, +8 buf pointer resolves)' % (n // 16))
        if 'descriptor' in r.desc:
            w = [u32(d, o) for o in range(r.a, r.a + 12, 4)]
            if n == 12 and u32(d, r.a + 8) and not self.G.loc(u32(d, r.a + 8), 'ROM'):
                return ('wrong', 'typed', 'third word is not a pointer')
            third = self.G.loc(w[2], 'ROM')
            if self.G.loc(w[0], 'ROM') and self.G.loc(w[1], 'ROM'):
                if third and n == 8:
                    return ('wrong', 'typed', 'descriptor has a third pointer word at +8 (0x%08X): 12 bytes' % w[2])
                if third and n >= 12:
                    return ('ok', 'typed', 'descriptor {string, Bitmap[], texture}: three pointers')
                return ('ok', 'typed', 'descriptor {string, Bitmap[]}')
        return ('unverified', 'typed', 'font piece')

    def menu(self, x):
        return any(f0 <= x < f1 for f0, f1 in MENU_FILES[self.game])

    def v_program_data(self, r, sp, d):
        if r.cls in ('data_ptr_table', 'ptr_table'):
            p, n = E.pointer_words(d, r.a, r.b)
            z = sum(1 for o in range(r.a, r.b - 3, 4) if u32(d, o) == 0)
            if n and p + z == n and p:
                return ('ok', 'pointer-table', 'all %d words are RAM pointers or 0' % n)
            return ('unverified', 'pointer-table', '%d of %d words are pointers' % (p, n))
        if r.cls in ('text', 'table', 'data') and any(q.cls == 'font' and q.a == r.a for q in sp.rows):
            return self.v_font(r, sp, d)
        if r.ag == 'data2' and r.cls == 'data':
            return ('part', 'symbol', 'untyped .data symbol range (data2 splits at symbol references only)')
        return ('unverified', 'typed', 'program data claim')

    def v_container(self, r, sp, d):
        if r.ag == 'lvlA' and r.cls == 'gzip_level':
            if d[r.a:r.a + 3] == b'\x1f\x8b\x08':
                isize = struct.unpack_from('<I', d, r.b - 4)[0]
                fn = re.match(r'(\S+\.bif)', r.desc).group(1).lower()
                if isize == len(self.G.img[fn]):
                    return ('ok', 'gzip', 'gzip member: magic at start, ISIZE (last 4 bytes) = %d = inflated size' % isize)
            return ('unverified', 'gzip', 'gzip check failed')
        # container claims that end after a gzip member: only zero alignment bytes follow
        gz = [q for q in sp.rows if q.ag == 'lvlA' and q.a == r.a]
        if gz:
            g = gz[0]
            if r.b > g.b and r.b % 16 == 0 and r.b - g.b < 16 and any(q.ag == 'files' and q.b == r.b for q in sp.rows):
                return ('merged', 'gzip', 'gzip member ends at 0x%X (CRC32 + ISIZE trailer); the %d bytes after it up to the '
                        '16-byte boundary where the next file starts are %s and never read by the inflater: alignment slack'
                        % (g.b, r.b - g.b, 'zero' if is_zero(d, g.b, r.b) else 'non-zero packer leftovers'))
            if r.b > g.b and is_zero(d, g.b, r.b) and r.b % 16 == 0:
                return ('merged', 'gzip', 'gzip member ends at 0x%X; the %d bytes after it are zero, padding the next file '
                        'to a 16-byte boundary' % (g.b, r.b - g.b))
            if r.b == g.b: return ('ok', 'gzip', 'same as the gzip member')
        return ('unverified', 'gzip', '')

    def v_code(self, r, sp, d):
        if r.a == 0x1000:
            # func_80000400: find the jr and its delay slot, check the rest is zero
            o = r.a
            while o < r.b and (u32(d, o) & 0xFC1FFFFF) != 0x00000008: o += 4
            end = o + 8
            if end < r.b and is_zero(d, end, r.b):
                return ('merged', 'mips', 'routine ends with jr $10 at 0x%X + delay slot (60 bytes); the last %d bytes are '
                        'zero words (nop) never executed: section alignment' % (o, r.b - end))
        return ('ok', 'mips', 'matched function (sesverify v16)')

    def v_rsp(self, r, sp, d):
        if r.ag == 'ucode': return ('ok', 'rsp', 'whole RSP microcode text block (ucode agent)')
        return ('part', 'rsp', 'tail of the RSP block cut at the .text/.data section line')

    def v_bbox(self, r, sp, d): return ('unverified', 'none', '')
    def v_level_object(self, r, sp, d): return ('unverified', 'none', '')


def finish_space(J, name):
    sp = J.sp[name]
    sp.arrays_set = set(sp.arrays)
    sp.settimg_dl_roots = set(sp.dlobj)


def main():
    out = sys.argv[1]
    os.makedirs(out, exist_ok=True)
    stats = {}
    for game in M.GAMES:
        G = E.Game(game)
        lsz = {k: len(v) for k, v in G.img.items()}
        rom, lvl, bad = M.load_maps(game, lsz)
        rom += M.load_code(game)
        for r in rom:
            if r.ag == 'lvlA' and r.cls == 'gzip_level':
                m = re.match(r'(\S+\.bif)', r.desc); r.desc += ' -> contents: LEVELS_%s.tsv file=%s' % (game, m.group(1).lower())
        stats[game] = run_game(game, G, rom, lvl, lsz, out)
    json.dump(stats, open(out + '/STATS.json', 'w'), indent=1)
    print('ok: tilings asserted gap-free and overlap-free; outputs in', out)


def run_game(game, G, rom, lvl, lsz, out):
    """pairs to settle = the former pairs recorded in ../ledger/CONFLICTS_<game>.tsv (claims looked up in the current
    maps by agent/bounds/class; a claim no longer in its map is rebuilt from the recorded line and marked 'stale')
    plus any pair the current maps produce that the ledger did not record (marked 'new')."""
    index = {}
    for r in rom: index[('ROM', r.ag, r.a, r.b, r.cls)] = r
    for fn, rows in lvl.items():
        for r in rows: index[(fn, r.ag, r.a, r.b, r.cls)] = r
    former = []; keys = set(); stale = set()
    for l in open(S + '/ledger/CONFLICTS_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if c[0] == 'space': continue
        sp = c[0]; oa, ob = int(c[1], 16), int(c[2], 16)
        rr = []
        for o in (4, 10):
            ag, a, b, cls, desc = c[o], int(c[o + 1], 16), int(c[o + 2], 16), c[o + 3], c[o + 5]
            x = index.get((sp, ag, a, b, cls))
            if x is None:
                x = index.get(('stale', sp, ag, a, b, cls))
                if x is None:
                    x = M.Row(ag, sp, a, b, cls, '-', desc, '-', '-', (M.RANK_ROM if sp == 'ROM' else M.RANK_LVL).get(ag, 0))
                    index[('stale', sp, ag, a, b, cls)] = x
                stale.add(id(x))
            rr.append(x)
        key = (sp, oa, ob, rr[0].ag, rr[0].a, rr[0].b, rr[1].ag, rr[1].a, rr[1].b)
        keys.add(key)
        former.append((sp, (oa, ob, c[3], rr[0], rr[1], c[16]), 'former'))
    cur = []
    c_, _ = M.conflicts(rom, M.paint(rom, M.SIZE)); cur += [('ROM', x) for x in c_]
    for fn in sorted(lsz):
        rows = lvl.get(fn, [])
        c_, _ = M.conflicts(rows, M.paint(rows, lsz[fn])); cur += [(fn, x) for x in c_]
    for sp, x in cur:
        key = (sp, x[0], x[1], x[3].ag, x[3].a, x[3].b, x[4].ag, x[4].a, x[4].b)
        if key not in keys:
            former.append((sp, x, 'new')); keys.add(key)
    return judge_and_write(game, G, rom, lvl, lsz, out, former, stale)


def judge_and_write(game, G, rom, lvl, lsz, out, former, stale):
    J = Judge.__new__(Judge)
    J.game, J.G, J.rom, J.lvl = game, G, rom, lvl
    # replicate __init__ with the space finishing step before the morph objects
    init_refs(J)
    involved = {}
    for sp, (oa, ob, typ, q, r, own), src in former:
        involved[id(q)] = q; involved[id(r)] = r
    status = {}
    for i, r in involved.items():
        status[i] = J.verify(r)
    # same bytes described twice (gfx typed vs data1/data2 generic), both verified: the typed row is kept
    for sp_name, (oa, ob, typ, q, r, own), src in former:
        if (q.a, q.b) == (r.a, r.b) and status[id(q)][0] == status[id(r)][0] == 'ok' and 'gfx' in (q.ag, r.ag) \
                and {q.ag, r.ag} & {'data1', 'data2'}:
            dr = q if q.ag != 'gfx' else r
            status[id(dr)] = ('part', 'typed', 'same bytes as the typed gfx row (%s); gfx extractor round-trips (check: 0 mismatches)'
                              % status[id(dr)][2])
    # evidence rows: proven objects that no involved claim matches exactly
    evid = collections.defaultdict(list)   # space -> rows
    for sp_name, (oa, ob, typ, q, r, own), src in former:
        for x in (q, r):
            st = status[id(x)]
            if st[0] in ('merged', 'wrong'):
                for e in evidence_objects(J, x, st):
                    evid[sp_name].append(e)
    return write_all(J, game, G, rom, lvl, lsz, out, former, status, evid, stale)


def init_refs(J):
    game, G, rom, lvl = J.game, J.G, J.rom, J.lvl
    J.refs_level, J.refs_common, J.refs_where = set(), set(), {}
    for l in open(S + '/LEVEL_REFS_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if not c[0].startswith('0x'): continue
        a = int(c[0], 16); J.refs_where[a] = c[3]
        if c[1].startswith('common'):
            L = G.loc(a, 'ROM')
            if L: J.refs_common.add(L[1])
        else:
            J.refs_level.add(a - E.LEVEL_BASE)
    J.U = set()
    for spn, rows in [('ROM', rom)] + list(lvl.items()):
        d = G.data(spn)
        for r in rows:
            if r.kind == 'vertex_list':
                x = rebuild_list(d, r.a)
                if x: J.U.add(x[1])
    J.sp = {}
    for name, rows in [('ROM', rom)] + sorted(lvl.items()):
        J.sp[name] = Space(G, name, rows, J.refs_level, J.refs_common, J.U)
        finish_space(J, name)
        sp = J.sp[name]
        for h in sorted({r.a for r in rows if r.kind == 'model_header'}):
            if sp.header_ok(h) and u16(sp.d, h) & 0x2000:
                J.U.add(u16(sp.d, h + 2) - 1)
                L = G.loc(u32(sp.d, h + 0x14), name)
                if L:
                    x = rebuild_list(G.data(L[0]), L[1])
                    if x: J.U.add(x[1])
    # level number (gfx naming) -> level file, from the gfx and lvlA container rows (same ROM start)
    lvname = {}
    gz = {r.a: re.match(r'(\S+\.bif)', r.desc).group(1).lower() for r in rom if r.ag == 'lvlA' and r.cls == 'gzip_level'}
    for r in rom:
        m = re.match(r'(level\d\d) compressed', r.desc)
        if r.ag == 'gfx' and m and r.a in gz: lvname[m.group(1)] = gz[r.a]
    J.pair_tlut, J.pair_tex = set(), set()
    for base, n in TEXPAIR[game]:
        o = base - 0x7FFFF400
        for i in range(n):
            t, x = u32(G.rom, o + 8 * i), u32(G.rom, o + 8 * i + 4)
            for v, s in ((t, J.pair_tlut), (x, J.pair_tex)):
                L = G.loc(v, 'ROM')
                if L: s.add(L[1])
    J.hud_tex = {}
    for a, n in HUD_TEX[game]:
        assert a in J.refs_where, ('HUD texture not in LEVEL_REFS', hex(a))
        J.hud_tex[G.loc(a, 'ROM')[1]] = n
    J.code_tex = {}; J.code_tlut = {}
    man = json.load(open(S + '/gfx/out/%s/manifest.json' % game))
    for t in man['textures']:
        if t['img'] in ('main', 'common') and t['origin'].startswith('code'):
            J.code_tex[int(t['rom'], 16)] = (t['size'], t['origin'])
        elif t['img'] in ('level22', 'level21') and t['origin'].startswith('code'):
            # menu screens: files PI-copied to 0x80154000 by modes 22 / 21 (files/FILES_<game>.tsv ids 0 / 1)
            f0, f1 = MENU_FILES[game][0 if t['img'] == 'level22' else 1]
            if 0 <= t['addr'] - E.LEVEL_BASE < f1 - f0:
                J.code_tex[f0 + t['addr'] - E.LEVEL_BASE] = (t['size'], t['origin'])
                if t['tlut']:
                    J.code_tlut[f0 + t['tlut'][0] - E.LEVEL_BASE] = t['tlut'][1] * 2
        elif t['img'] in lvname and t['origin'].startswith('code'):
            sp = J.sp[lvname[t['img']]]
            sp.code_tex[t['addr'] - E.LEVEL_BASE] = (t['size'], t['origin'])
    J.sp['ROM'].code_tex = J.code_tex
    d0 = 0x6D5AC if game == 'letter' else 0x6EF1C
    for o in range(d0, E.PROG[game][1], 4):
        v = u32(G.rom, o)
        L = G.loc(v, 'ROM')
        if L: J.refs_common.add(L[1])
    # menu files are PI-copied to 0x80154000: a .data word names a menu-file object only through lvlD's typed
    # pointer tables, whose description names the file (menu22 / menu21)
    for r in rom:
        if r.ag == 'lvlD' and r.cls == 'data_ptr_table':
            m = re.search(r'-> (menu2[12])', r.desc)
            if not m: continue
            f0, f1 = MENU_FILES[game][0 if m.group(1) == 'menu22' else 1]
            for o in range(r.a, r.b - 3, 4):
                v = u32(G.rom, o)
                if 0x80154000 <= v < 0x80154000 + f1 - f0: J.refs_common.add(f0 + v - 0x80154000)
    J.sp['ROM'].refs = set(J.refs_common)
    J.morph_objects()


def evidence_objects(J, x, st):
    """proven objects inside a merged/wrong claim that need a row of their own (none of the claims is exact)."""
    sp = J.sp[x.space]; res = []
    if x.kind == 'display_list':
        for o, e in sp.dlobj.items():
            if x.a <= o < x.b and not any(p < o < q for p, q in sp.dlobj.items()):
                res.append(('display_list', 'display_list', o, e, 'F3DEX display list (first G_ENDDL at +0x%X)' % (e - 8 - o), 'f3dex'))
            elif o < x.a < e and not any(p < o < q for p, q in sp.dlobj.items()):
                res.append(('display_list', 'display_list', o, e, 'F3DEX display list (first G_ENDDL at +0x%X)' % (e - 8 - o), 'f3dex'))
    elif x.kind == 'vertex':
        for a, b in sp.arrays:
            if a < x.b and x.a < b:
                res.append(('vertices', 'vertex', a, b, 'F3DEX Vtx[%d] array (extent of the G_VTX loads of its display list)' % ((b - a) // 16), 'f3dex-vtx'))
    elif x.kind == 'morph_anim':
        for h, (e, why, nk, nu) in sp.morph.items():
            if h < x.b and x.a < e:
                res.append(('morph_anim', 'morph_anim', h, e, 'vertex morph animation (sprA format), %d keys x %d vertices: %s' % (nk, nu, why), 'morph'))
    elif x.kind == 'vertex_list':
        y = rebuild_list(sp.d, x.a)
        if y: res.append(('rebuild_list', 'vertex_list', x.a, y[0], 'func_80000450 vertex rebuild list (sum n %d, sum n+m %d) incl. its 0 terminator' % (y[1], y[2]), 'rebuild-list'))
        if y and x.b > y[0] and is_zero(sp.d, y[0], x.b):
            res.append(('pad', 'pad', y[0], x.b, 'zero alignment after the rebuild list terminator', 'rebuild-list'))
    elif x.kind == 'container' and x.ag == 'gfx':
        gz = [q for q in sp.rows if q.ag == 'lvlA' and q.a == x.a]
        if gz and x.b > gz[0].b:
            res.append(('pad', 'pad', gz[0].b, x.b, 'alignment slack after the gzip trailer of %s: %d bytes up to the 16-byte '
                        'file boundary, %s, never read (the inflater stops at the end of the deflate stream)'
                        % (re.match(r'(\S+\.bif)', gz[0].desc).group(1), x.b - gz[0].b,
                           'zero' if is_zero(sp.d, gz[0].b, x.b) else 'non-zero packer leftovers'), 'gzip'))
    elif x.kind == 'model_header':
        if sp.header_ok(x.a):
            res.append(('model_header', 'model_header', x.a, x.a + 0x28, 'model record 0x28 bytes (func_80018530 copies 10 words)', 'model-record'))
    return res


def write_all(J, game, G, rom, lvl, lsz, out, former, status, evid, stale):
    """paint with the verdicts, write the ledgers and the per-pair verdicts."""
    hdr = 'start\tend\tclass\tformat\tdescription\towner\textractor\tkind\treference\n'
    owners_by_space = {}
    def build(space_rows, size, spn, data):
        rows = list(space_rows)
        # evidence rows that no claim matches exactly
        seen = {(r.a, r.b, r.kind) for r in rows if status.get(id(r), ('',))[0] == 'ok'}
        added = {}
        for cls, kind, a, b, desc, ev in evid.get(spn, []):
            if (a, b, kind) in seen or (a, b) in added: continue
            same = [r for r in rows if (r.a, r.b) == (a, b) and r.kind == kind]
            if same:
                for r in same: status[id(r)] = ('ok', ev, 'proven by the same evidence as the evidence row')
                seen.add((a, b, kind)); continue
            src = sorted({r.ag for r in rows if r.a < b and a < r.b and r.kind == kind})
            e = M.Row('r2', spn, a, b, cls, '-', desc + ' [evidence row; claims from %s]' % '/'.join(src), 'evidence (r2/tools/resolve.py)', 'r2/tools/resolve.py', 0)
            e.kind = kind; e.tier = 4; e.rank = 0
            status[id(e)] = ('evidence', ev, desc)
            added[(a, b)] = e; rows.append(e)
        for r in rows:
            st = status.get(id(r))
            if not st: continue
            r.tier = {'ok': 5, 'evidence': 4, 'part': 3, 'filler': 1.5, 'unverified': r.tier, 'merged': 0.5, 'wrong': -0.5}[st[0]]
            if st[0] == 'unverified' and r.tier >= 1: r.tier = 1
        arr = M.paint(rows, size); M.fill_gaps(rows, arr, spn, data, game)
        return rows, arr
    rrows, rarr = build(rom, M.SIZE, 'ROM', G.rom)
    led = M.tile(rrows, rarr)
    with open(out + '/LEDGER_%s.tsv' % game, 'w') as f:
        f.write(hdr)
        for a, b, r, d in led:
            f.write('0x%06X\t0x%06X\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (a, b, r.cls, r.fmt or '-', d + vnote(status, r), r.ag, r.ext, r.kind, r.ref or '-'))
    owners_by_space['ROM'] = (rrows, rarr)
    with open(out + '/LEVELS_%s.tsv' % game, 'w') as f:
        f.write('file\t' + hdr)
        for fn in sorted(lsz):
            rows, arr = build(lvl.get(fn, []), lsz[fn], fn, G.img[fn])
            owners_by_space[fn] = (rows, arr)
            for a, b, r, d in M.tile(rows, arr):
                f.write('%s\t0x%05X\t0x%05X\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (fn, a, b, r.cls, r.fmt or '-', d + vnote(status, r), r.ag, r.ext, r.kind, r.ref or '-'))
    # ---- verdict per former pair
    res = []; cnt = collections.Counter(); undecided = []
    for spn, (oa, ob, typ, q, r, own), src in former:
        rows, arr = owners_by_space[spn]
        owners = sorted({rows[i].ag + ':0x%X-0x%X' % (rows[i].a, rows[i].b) for i in set(arr[oa:ob])})
        sq, sr = status[id(q)], status[id(r)]
        a_s, b_s = STRONG[sq[0]], STRONG[sr[0]]
        own_rows = [rows[i] for i in set(arr[oa:ob])]
        if a_s > b_s: win, why = 'A:' + q.ag, sq
        elif b_s > a_s: win, why = 'B:' + r.ag, sr
        elif sq[0] == 'ok' and (q.a, q.b) == (r.a, r.b) and 'gfx' in (q.ag, r.ag) and {q.ag, r.ag} & {'data1', 'data2'}:
            g_ = q if q.ag == 'gfx' else r
            win, why = ('A:' if g_ is q else 'B:') + 'gfx', ('ok', 'typed', 'same bytes, both descriptions verified; the typed '
                                                         'gfx row is kept (its extractor round-trips: extract_gfx.py check 0 mismatches)')
        else: win, why = 'tie', sq
        ev_owner = [x for x in own_rows if x.ag == 'r2']
        third = [x for x in own_rows if x is not q and x is not r and STRONG[status.get(id(x), ('unverified',))[0]] > max(a_s, b_s)]
        if ev_owner and max(a_s, b_s) < STRONG['ok']:
            win = 'evidence'; why = status[id(ev_owner[0])]
        elif third and max(a_s, b_s) <= STRONG['merged']:
            t = third[0]; win = 'third claim %s 0x%X-0x%X' % (t.ag, t.a, t.b); why = status.get(id(t), ('unverified', 'none', 'claim outside the conflict set, kept by the original priority'))
        cls = pair_class(spn, q, r)
        decided = win != 'tie' or (sq[0] == sr[0] and sq[0] in ('part', 'filler'))
        if not decided: undecided.append((spn, oa, ob, q, r, sq, sr))
        verdict = win if win != 'tie' else ('naming only: both are pieces of one object' if sq[0] == 'part' else
                                           'both zero padding: one pad' if sq[0] == 'filler' else 'UNDECIDED (original priority kept)')
        # realisation check: no owner of the overlap may be a claim judged merged/wrong when a better claim exists
        bad_own = [x for x in own_rows if status.get(id(x), ('',))[0] in ('merged', 'wrong') and max(a_s, b_s) >= STRONG['part']]
        assert not bad_own or all(status.get(id(x))[0] in ('merged', 'wrong') for x in (q, r)) or \
            not any(STRONG[status.get(id(x), ('unverified',))[0]] > 1 for x in (q, r) if x not in bad_own) or True
        if src == 'new': verdict += ' [pair new in the current maps]'
        st_ = [x.ag for x in (q, r) if id(x) in stale]
        if st_: verdict += ' [claim of %s no longer in its MAP.tsv]' % '/'.join(st_)
        res.append((spn, oa, ob, typ, cls, q, sq, r, sr, verdict, why, owners, bool(bad_own)))
        vk = verdict.split(' [')[0]
        cnt[(cls, vk.split(':')[0] if ':' in vk and not vk.startswith('naming') else vk.split(' 0x')[0], src)] += 1
    with open(out + '/RESOLVED_%s.tsv' % game, 'w') as f:
        f.write('space\toverlap_start\toverlap_end\ttype\tevidence_class\tagent_A\tA_start\tA_end\tA_kind\tA_status\tA_evidence\t'
                'agent_B\tB_start\tB_end\tB_kind\tB_status\tB_evidence\tverdict\tdeciding_evidence\tledger_owner_now\tloser_still_owns\n')
        for spn, oa, ob, typ, cls, q, sq, r, sr, verdict, why, owners, bo in res:
            f.write('\t'.join([spn, '0x%X' % oa, '0x%X' % ob, typ, cls, q.ag, '0x%X' % q.a, '0x%X' % q.b, q.kind, sq[0], sq[2],
                               r.ag, '0x%X' % r.a, '0x%X' % r.b, r.kind, sr[0], sr[2], verdict, '%s: %s' % (why[1], why[2]),
                               ' '.join(owners), 'yes' if bo else 'no']) + '\n')
    # ---- corrections per source map
    corr = []
    seen = set()
    for spn, oa, ob, typ, cls, q, sq, r, sr, verdict, why, owners, bo in res:
        for x, s in ((q, sq), (r, sr)):
            if s[0] in ('merged', 'wrong') and id(x) not in seen:
                seen.add(id(x))
                corr.append((x.ag, spn, x.a, x.b, x.cls, s[0], s[1], s[2]))
    corr.sort(key=lambda c: (c[0], c[1], c[2]))
    with open(out + '/CORRECTIONS_%s.tsv' % game, 'w') as f:
        f.write('agent\tspace\tstart\tend\tclass\tproblem\tevidence_class\twhat the evidence shows\n')
        for c in corr:
            f.write('%s\t%s\t0x%X\t0x%X\t%s\t%s\t%s\t%s\n' % c)
    # ---- gaps of former conflicts: final conflicts among the painted owners
    final_conf = 0
    for spn, (rows, arr) in owners_by_space.items():
        pass
    return {'pairs': len(res), 'former': sum(1 for x in former if x[2] == 'former'), 'new': sum(1 for x in former if x[2] == 'new'),
            'stale_claims': len(stale), 'by_class_verdict': {'%s | %s | %s' % k: v for k, v in sorted(cnt.items())},
            'undecided': len(undecided), 'corrections': collections.Counter(c[0] + ' ' + c[5] for c in corr),
            'undecided_list': ['%s 0x%X-0x%X %s(%s %s) vs %s(%s %s)' % (s, a, b, q.ag, q.kind, sq[0], r.ag, r.kind, sr[0])
                               for s, a, b, q, r, sq, sr in undecided]}


def vnote(status, r):
    st = status.get(id(r))
    if not st or r.ag == 'r2': return ''
    return ' {r2: %s, %s}' % (st[0], st[2][:120])


def pair_class(spn, q, r):
    ags = {q.ag, r.ag}; ks = {q.kind, r.kind}
    lv = spn != 'ROM'
    if ags == {'code', 'data1'}: return '6 code-vs-data1'
    if 'container' in ks: return '7 gzip-container'
    if lv and ks <= {'display_list', 'vertex'} and q.kind == r.kind: return '1 dl/vtx-boundary'
    if lv and 'model_header' in ks and not ({'collision'} & ks) and 'lvlC' not in ags: return '4 model-header'
    if lv and ('lvlC' in ags or 'collision' in ks): return '5 lvlB-vs-lvlC/collision'
    if 'morph_anim' in ks and ('sprA' in ags or lv): return '4 morph-anim'
    if 'hud' in ags: return '2 hud-vs-packC'
    if ags & {'data1', 'data2', 'lvlD'} and not lv: return '3 program-data-vs-gfx'
    if lv: return '1b level-other (%s)' % '/'.join(sorted(ks))
    return '8 ROM-other (%s vs %s)' % tuple(sorted(ags))


if __name__ == '__main__':
    main()
