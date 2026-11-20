#!/usr/bin/env python3
"""Re-validate the code-proven asset rules of ../FORMATS_PROVEN.md against every instance in both games.

usage: check_proven.py LETTER_ROM NUMBER_ROM

Reads only the ROMs (read-only) plus the other agents' root tables: ../../lvlD/tools/extract_level_misc.py
(mesh / anim / quad roots taken from the game code), ../../lvlC/tools/extract_level_data.py (BSP finder) and
../../sprA/MAP.tsv (ROM ranges of the streamed animations). Prints counts only; writes nothing.
"""
import sys, os, io, struct, contextlib, importlib.util
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..', '..'))


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m); return m

D = load('lvlD_tool', os.path.join(ROOT, 'lvlD', 'tools', 'extract_level_misc.py'))
C = load('lvlC_tool', os.path.join(ROOT, 'lvlC', 'tools', 'extract_level_data.py'))
from collections import defaultdict
DETAIL = defaultdict(Counter)        # value distributions printed at the end

# speaker meshes passed to Letter func_80026B70 / Number func_80027FE0 (record types 13/16/17, Number 13/14/15)
SPEAKER = {'letter': [0x802CD988, 0x802D3250, 0x802DA870], 'number': [0x8029A898, 0x802A24C8, 0x802A86D0]}


def u16(b, o): return struct.unpack_from('>H', b, o)[0]
def u32(b, o): return struct.unpack_from('>I', b, o)[0]
def s16(b, o): return struct.unpack_from('>h', b, o)[0]
def f32w(w): return struct.unpack('>f', struct.pack('>I', w))[0]
def sx(v, b): return v - (1 << b) if v >> (b - 1) else v


def unpack_code(w):
    """func_80000450: struct { s32 z:10; s32 y:11; s32 x:11; } (big-endian GCC): z = w>>22, y = (w<<10)>>21, x = (s16)(w<<5)>>5"""
    return sx(w & 0x7FF, 11), sx((w >> 11) & 0x7FF, 11), sx(w >> 22, 10)


class Stat:
    def __init__(s): s.c = Counter()
    def __call__(s, key, ok=True): s.c[(key, bool(ok))] += 1
    def show(s, title):
        print('==', title)
        for k in sorted({k for k, _ in s.c}):
            print('  %-72s ok %5d  fail %5d' % (k, s.c[(k, True)], s.c[(k, False)]))


def anim_rules(st, b, a, nvert, label, after=None):
    """b = bytes holding the anim at offset a; after = bytes from the anim's computed end to the next structure"""
    he, do, ticks, nk, fl = u16(b, a), u16(b, a + 2), b[a + 4], b[a + 5], u16(b, a + 6)
    keys = list(b[a + 8:a + 8 + nk])
    h = 8 + nk
    st('hdr_end low byte == 8 + nkey', (he & 0xFF) == h)
    st('hdr_end high byte == 0', he >> 8 == 0)
    st('data_off rule (8 + nkey + 8) & ~7  [== ((8+nkey) & ~7) + 8]', do == ((h + 8) & ~7))
    st('data_off rule round_up_8(8 + nkey)  (fails when 8+nkey is a multiple of 8)', do == ((h + 7) & ~7))
    st('data_off % 4 == 0 (func_80000450 uses (data_off + 4k) >> 2 << 2)', do % 4 == 0)
    st('header pad bytes zero', not any(b[a + h:a + do]))
    st('flags & 0x7FFF == 0', fl & 0x7FFF == 0)
    st('keys[0] == 0', keys[0] == 0)
    st('keys strictly increasing (else "Two keys with same frame #!")', all(x < y for x, y in zip(keys, keys[1:])))
    st('keys[-1] <= ticks', keys[-1] <= ticks)
    st('keys[-1] == ticks (last key row is then never read)', keys[-1] == ticks)
    if keys[-1] != ticks: DETAIL['anims with keys[-1] != ticks (where, last key, ticks)'][(label, hex(a), keys[-1], ticks)] += 1
    if he >> 8: DETAIL['anims with hdr_end high byte set'][(label, hex(a), hex(he))] += 1
    if fl & 0x8000:
        DETAIL['loop anims (where, nkey, keys[-2], keys[-1], ticks)'][(label, hex(a), nk, keys[-2], keys[-1], ticks)] += 1
        st('loop anims: key nkey-1 rows equal key 0 rows', all(
            u32(b, a + do + 4 * (i * nk + nk - 1)) == u32(b, a + do + 4 * i * nk) for i in range(nvert)))
    if after is not None:
        st('bytes after last vertex row: none', len(after) == 0)
        if after:
            st('  tail length < one vertex row (4*nkey)', len(after) < 4 * nk)
            st('  tail all zero', not any(after))
    return do + 4 * nk * nvert


def check_game(rom, game, st_anim, st_mesh, st_quad, st_bsp, st_rec, tails):
    with contextlib.redirect_stdout(io.StringIO()):
        imgs, cols, unplaced, cammesh = D.build(rom, game)
    # ---- meshes, rebuild lists, bboxes
    uniq_of = {}
    for name, c in cols.items():
        img = c.img
        for (kind, a), j in c.items.items():
            if kind != 'mesh_header': continue
            fl, f2, nv, ws = j['flags'], j['field2'], j['nverts'], j['work_size']
            rb, bb = int(j['rebuild_list'], 16), int(j['bbox'], 16)
            anim = bool(rb)
            st_mesh('flags 0x2000 <=> rebuild list present', bool(fl & 0x2000) == anim)
            st_mesh('flags 0x8000 clear in ROM', not fl & 0x8000)
            st_mesh('+0x06 == 0', j['field6'] == 0)
            st_mesh('+0x10, +0x18 == 0 in ROM', int(j['rt_vtx_copy'], 16) == 0 and int(j['rt_vtx_end'], 16) == 0)
            st_mesh('work_size != 0 <=> animated', bool(ws) == anim)
            if anim:
                DETAIL['mesh work_size - nverts*16 (animated)'][ws - 16 * nv] += 1
                st_mesh('animated: work_size >= nverts * 16 (func_80018530 copy fits)', ws >= 16 * nv)
                st_mesh('animated: work_size >= 2 * nverts * 16 (type-7 in-place memset of work_size bytes at +0x18 stays inside)', ws >= 32 * nv)
                rj = c.items[('rebuild_list', rb)]
                tot, pos, okidx = 0, 0, True
                for n, idx in rj['runs']:
                    pos += n
                    for i in idx:
                        okidx &= i < pos
                        pos += 1
                st_mesh('rebuild: sum(n+m) == nverts', rj['total'] == nv)
                st_mesh('rebuild: every duplicate index < vertices written so far', okidx)
                st_mesh('+0x02 == unique + 1 (animated)', f2 == rj['unique'] + 1)
                st_mesh('+0x02 == unique (animated)', f2 == rj['unique'])
                uniq_of[(name, a)] = rj['unique']
                DETAIL['unique vertex counts of animated meshes (game, unique)'][(game, rj['unique'])] += 1
            else:
                st_mesh('+0x02 == nverts + 1 (static)', f2 == nv + 1)
                DETAIL['static mesh +0x02 - nverts'][f2 - nv] += 1
            if a in [m for m, q in D.T7[game].values()]: DETAIL['type-7 meshes (game, image, mesh, animated, work_size, nverts*16)'][(game, name, hex(a), anim, ws, 16 * nv)] += 1
            if bb:
                st_mesh('bbox pointer == header - 0x18', bb == a - 0x18)
                box = [f32w(img.w(bb + 4 * i)) for i in range(6)]
                st_mesh('bbox min <= max on all 3 axes', all(box[i] <= box[i + 3] for i in range(3)))
                vt = int(j['vtx'], 16)
                if img.has(vt, 16 * nv):
                    vs = [[s16(img.d, vt - img.base + 16 * i + 2 * k) for k in range(3)] for i in range(nv)]
                    lo = [min(v[k] for v in vs) for k in range(3)]; hi = [max(v[k] for v in vs) for k in range(3)]
                    st_mesh('bbox == exact min/max of the ROM Vtx x,y,z', lo == box[:3] and hi == box[3:])
                    st_mesh('bbox contains the ROM Vtx x,y,z', all(box[k] <= lo[k] and hi[k] <= box[k + 3] for k in range(3)))
            else:
                st_mesh('bbox pointer == 0', True)
    # ---- level/common anims (vertex count = unique count of the mesh they are paired with in the code)
    for name, c in cols.items():
        img = c.img
        items = sorted(c.items.values(), key=lambda j: j['off'])
        for i, j in enumerate(items):
            if j['kind'] != 'morph_anim': continue
            nxt = items[i + 1]['off'] if i + 1 < len(items) else len(img.d)
            end = j['off'] + j['data_off'] + 4 * j['nkey'] * j['nvert']
            after = img.d[end:nxt] if end <= nxt < end + 4 * j['nkey'] else None   # only a gap shorter than one row is a tail
            anim_rules(st_anim, img.d, j['off'], j['nvert'], '%s %s %s' % (game, name, j['ram']), after)
            if 'note' in j:
                st_anim('nvert == mesh unique count (lvlD cut: next structure starts earlier)', False)
            else:
                st_anim('nvert == mesh unique count', True)
            if after:
                tails['%s %s' % (game, name)].append((j['ram'], len(after), 'zero' if not any(after) else 'nonzero'))
    # ---- ROM streamed anims (speaker meshes)
    sp = D.Collector(imgs['common'])
    su = []
    for m in SPEAKER[game]:
        mj = sp.model(m, 'speaker')
        su.append(sp.items[('rebuild_list', int(mj['rebuild_list'], 16))]['unique'])
    rows = [l.rstrip('\n').split('\t') for l in open(os.path.join(ROOT, 'sprA', 'MAP.tsv')) if l.startswith(game + '\t')]
    starts = [int(r[1], 16) for r in rows if r[4] == 'vanim header']
    ends = {}
    rows.sort(key=lambda r: int(r[1], 16))
    cur = None
    for r in rows:
        s, e = int(r[1], 16), int(r[2], 16)
        if r[4] == 'vanim header': cur = s
        if cur is not None and r[3] == 'sprite_anim': ends[cur] = e      # vertex rows; trailing 'pad' rows = tail
    for s in starts:
        nk, do = rom[s + 5], u16(rom, s + 2)
        e = ends[s]
        nvf = (e - s - do) // (4 * nk)
        st_anim('ROM anim: floor((size-data_off)/(4*nkey)) is a speaker-mesh unique count %s' % su, nvf in su)
        if nvf not in su: DETAIL['ROM anims whose row count is no speaker mesh (start, nvert, nkey, size)'][(game, hex(s), nvf, nk, e - s)] += 1
        nxt = [x for x in starts if x > s]
        filend = min(nxt) if nxt else None
        # the file (code start/end pair) may hold several anims; the tail runs to the next anim or to the MAP end
        fe = filend if filend else max(int(r[2], 16) for r in rows)
        ln = anim_rules(st_anim, rom, s, nvf, '%s ROM' % game, rom[s + do + 4 * nk * nvf:fe])
        tl = rom[s + ln:fe]
        if tl:
            ws = [u32(tl, 4 * k) for k in range(len(tl) // 4)]
            body = set(struct.unpack_from('>%dI' % (nk * nvf), rom, s + do))
            st_anim('ROM tail: every tail word also occurs in the same anim\'s vertex data', all(w in body for w in ws))
            # a stale copy of the previous rows? compare with the words exactly one row (4*nkey) earlier
            st_anim('ROM tail: tail == the bytes 4*nkey earlier (repeat of the last row\'s start)',
                    tl == rom[s + ln - 4 * nk:s + ln - 4 * nk + len(tl)])
            tails['%s ROM' % game].append(('0x%X' % s, len(tl), 'zero' if not any(tl) else 'nonzero'))
    # ---- collision quads
    for name, c in cols.items():
        for (kind, a), j in c.items.items():
            if kind != 'coll_quad': continue
            w = [int(x, 16) for x in j['words']]
            st_quad('+0x00 == 3 (type 3 = quad, as func_80023118 writes into the runtime item)', w[0] == 3)
            st_quad('+0x04 == 0 (runtime owner actor)', w[1] == 0)
            DETAIL['quad +0x08 value'][hex(w[2])] += 1
            st_quad('+0x0C == 0', w[3] == 0)
            p = [[f32w(x) for x in w[8 + 3 * k:11 + 3 * k]] for k in range(4)]
            a1, a2 = [p[1][k] - p[0][k] for k in range(3)], [p[2][k] - p[1][k] for k in range(3)]
            n = [a2[1] * a1[2] - a2[2] * a1[1], a2[2] * a1[0] - a2[0] * a1[2], a2[0] * a1[1] - a2[1] * a1[0]]
            l = sum(x * x for x in n) ** 0.5 or 1
            n = [x / l for x in n]; d = sum(n[k] * p[0][k] for k in range(3))
            stored = [f32w(x) for x in w[4:8]]
            st_quad('+0x10..+0x1C == func_80020A24 plane (n, d) of the quad\'s own corners (1e-3)',
                    all(abs(stored[k] - (n + [d])[k]) <= 1e-3 * max(1, abs((n + [d])[k])) for k in range(4)))
    # ---- BSP (lvlC finder) and object records
    for name, img in imgs.items():
        if name == 'common': continue
        ci = C.Img(img.d)
        for sa, sw, (nodes, leaves) in C.find_shapes(ci):
            st_bsp('shape +0x00 == 12, +0x04 == 0, +0x0C == 0', sw[0] == 12 and sw[1] == 0 and sw[3] == 0)
            st_bsp('shape +0x08 mask == 0x80000000', sw[2] == 0x80000000)
            idx = Counter()
            def box(a): return [s16(img.d, a - img.base + 4 + 2 * k) for k in range(6)]
            leafverts = {}
            for a, w in leaves.items():
                idx[w[0] >> 16] += 1
                b = box(a)
                st_bsp('leaf box +4<=+6, +8<=+A, +C<=+E', b[0] <= b[1] and b[2] <= b[3] and b[4] <= b[5])
                vt, nt, tr = w[5], w[6], w[7]
                vs, padok = [], True
                for t in range(nt):
                    q = img.d[tr - img.base + 4 * t: tr - img.base + 4 * t + 4]
                    padok &= q[3] == 0
                    for i in q[:3]:
                        vs.append([s16(img.d, vt - img.base + 16 * i + 2 * k) for k in range(3)])
                st_bsp('tri byte 3 == 0', padok)
                leafverts[a] = vs
                st_bsp('leaf box == exact min/max (x:+4/+6 y:+8/+A z:+C/+E) of its triangles\' vertices',
                       [min(v[0] for v in vs), max(v[0] for v in vs), min(v[1] for v in vs), max(v[1] for v in vs),
                        min(v[2] for v in vs), max(v[2] for v in vs)] == b)
                st_bsp('leaf box contains its triangles\' vertices', all(
                    b[0] <= v[0] <= b[1] and b[2] <= v[1] <= b[3] and b[4] <= v[2] <= b[5] for v in vs))
            def sub(a):
                if a in leaves: return leafverts[a]
                return sum((sub(c) for c in nodes[a][8:10] if c), [])
            for a, w in nodes.items():
                idx[w[0] >> 16] += 1
                b = box(a)
                st_bsp('node box +4<=+6, +8<=+A, +C<=+E', b[0] <= b[1] and b[2] <= b[3] and b[4] <= b[5])
                vs = sub(a)
                st_bsp('node box contains all vertices below it', all(
                    b[0] <= v[0] <= b[1] and b[2] <= v[1] <= b[3] and b[4] <= v[2] <= b[5] for v in vs))
                n = [f32w(x) for x in w[4:8]]
                st_bsp('node plane |n| == 1 (1e-3)', abs(sum(x * x for x in n[:3]) - 1) < 1e-3)
                fr = sub(w[8]) if w[8] else []
                bk = sub(w[9]) if w[9] else []
                dist = lambda v: n[0] * v[0] + n[1] * v[1] + n[2] * v[2] - n[3]
                st_bsp('node +0x20 subtree vertices on the n.v-d >= -1 side (front)', all(dist(v) >= -1 for v in fr))
                st_bsp('node +0x24 subtree vertices on the n.v-d <= +1 side (back)', all(dist(v) <= 1 for v in bk))
                cen = lambda vs: [[sum(vs[3 * t + q][k] for q in range(3)) / 3 for k in range(3)] for t in range(len(vs) // 3)]
                st_bsp('node +0x20 subtree triangle centroids n.c-d >= -1 (front)', all(dist(v) >= -1 for v in cen(fr)))
                st_bsp('node +0x24 subtree triangle centroids n.c-d <= +1 (back)', all(dist(v) <= 1 for v in cen(bk)))
            st_bsp('word0 >> 16 index unique within the tree', all(v == 1 for v in idx.values()))
        # object records
        lst = D.OBJLIST[name]
        heads = set()
        while img.w(lst):
            r = img.w(lst); lst += 4
            typ, oid = img.w(r), img.w(r + 4)
            heads.add(img.w(r + 0x30))
            st_rec('type word < 0x10000 (handlers copy only the u16 at +2)', typ < 0x10000)
            st_rec('types seen: %s %d' % (game, typ), True)
            st_rec('id word < 0x10000 (u16 at +6 copied; types 9/11 read the s32)', oid < 0x10000)
        st_rec('one world-list head per level (+0x30)', len(heads) == 1)


def main():
    if len(sys.argv) != 3: sys.exit(__doc__)
    sts = [Stat() for _ in range(5)]
    tails = {}
    from collections import defaultdict
    tails = defaultdict(list)
    for game, path in (('letter', sys.argv[1]), ('number', sys.argv[2])):
        rom = open(path, 'rb').read()
        check_game(rom, game, *sts, tails)
    for s, t in zip(sts, ['morph animations', 'mesh headers / rebuild lists / bboxes', 'collision quads (ROM)',
                          'BSP nodes / leaves', 'object records']):
        s.show(t)
    for k, v in DETAIL.items():
        print('== %s' % k)
        for x, n in sorted(v.items(), key=lambda t: str(t[0])):
            print('  %-60s x%d' % (x, n))
    print('== bytes after the last vertex row (anim, length, content)')
    for k, v in sorted(tails.items()):
        c = Counter((n, z) for _, n, z in v)
        print('  %-18s %3d anims with a tail: %s' % (k, len(v), ', '.join('%dB %s x%d' % (n, z, m) for (n, z), m in sorted(c.items()))))


if __name__ == '__main__':
    main()
