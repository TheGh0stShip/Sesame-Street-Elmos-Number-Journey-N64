#!/usr/bin/env python3
"""merge_ledger.py: merge every sesassets/*/MAP.tsv plus the code ledger into one non-overlapping tiling per game.

usage: merge_ledger.py OUTDIR [--rom letter=PATH] [--rom number=PATH]

Writes OUTDIR/LEDGER_<game>.tsv (ROM 0x0-0x800000), OUTDIR/LEVELS_<game>.tsv (the 8 inflated level images),
OUTDIR/CONFLICTS_<game>.tsv and OUTDIR/SUMMARY.md. Priority rules: see ../FORMAT.md (also RANK_* below).
If a ROM path is given, ranges no map claims are tested for all-zero (only a yes/no is recorded, never bytes);
for level images the inflated images in lvlA/out/<game>/ are used the same way when present.
The script asserts zero gaps and zero overlaps in every tiling it writes."""
import os, sys, re, json, glob, collections
from array import array
import itertools

LAB = os.path.expanduser('${N64DK_LAB}'); S = LAB + '/sesassets'
FUNCS = LAB + '/sestools/functions.json'; VER = LAB + '/sesverify/v16'
SIZE = 0x800000
GAMES = ('letter', 'number')
TEXT = {'letter': (0x1050, 0x6D5AC), 'number': (0x1050, 0x6EF1C)}   # main .text (ASSET_FACTS)

# ---- priority: higher rank paints later and wins. tier 0 (unknown-like claims) always loses to tier 1.
RANK_ROM = {'files': 10, 'gfx': 20, 'data2': 30, 'data1': 31, 'speech': 40, 'packC': 41, 'audio': 50,
            'code': 25, 'packA': 60, 'packB': 60, 'sprA': 60, 'songs': 61, 'lvlD': 62, 'ucode': 70,
            'hud': 75, 'lvlA': 90, 'r1': 45}
RANK_LVL = {'gfx': 20, 'lvlB': 30, 'lvlC': 40, 'lvlD': 65, 'lvlE': 60, 'lvlF': 60, 'lvlG': 60, 'lvlH': 60, 'r1': 66}
CONTAINER_AGENTS = {'files'}           # every non-pad row of these is a container
CONTAINER_CLASSES = {'gzip_level', 'container'}

KIND = {}
for k, names in {
    'texture': 'texture tex_image hud_texture texframe_texture unused_texture texture_block_unused font',
    'palette': 'palette tlut tex_palette hud_palette texframe_palette unused_palette',
    'display_list': 'display_list dl texload_dl gfx',
    'vertex': 'vertex vertices vtx',
    'vertex_list': 'vertex_dup_list rebuild_list morph_vertex_order vertex_order model_vtxmap vertex_runs table_vtx',
    'model_header': 'model_header model_hdr mesh_header model_header_tail mesh_header_tail model_header_ext',
    'bbox': 'bbox model_bbox mesh_bbox model_bounds bounds',
    'morph_anim': 'morph_anim sprite_anim animation anim_header anim_vertices anim_tail',
    'collision': 'coll_quad coll_quad_header coll_quad_ptrs coll_quad_set coll_hdr collider_list collider_ptrs '
                 'collider_quad coll_list_item coll_shape coll_tris bsp_leaf bsp_node',
    'level_object': 'object object_list world_list_head',
    'song': 'libmus_song music',
    'audio_bank': 'libmus_fx_bank audio_bank',
    'speech': 'speech',
    'program_data': 'data table double float jumptable string text mtx vp sprite data_ptr_table ptr_table texanim_table',
    'rsp': 'rsp_ucode rsp_data',
    'unknown': 'unknown level_data',
    'pad': 'pad',
    'elf_comment': 'comment',
    'container': 'gzip_level container files',
    'header': 'rom_metadata', 'boot': 'boot', 'code': 'code',
}.items():
    for n in names.split(): KIND[n] = k

def kind_of(ag, cls, fmt):
    if ag == 'audio' and cls == 'audio': return 'audio_bank'
    if ag == 'speech' and cls == 'audio': return 'song' if 'libmus' in fmt.lower() else 'speech'
    if ag == 'packC' and cls == 'table': return 'vertex_list'
    if ag == 'files' and cls != 'pad': return 'container'
    return KIND.get(cls, cls)

def num(x): return int(x, 16)
def clean(s): return re.sub(r'\s+', ' ', s or '').strip()

class Row:
    __slots__ = ('ag', 'space', 'a', 'b', 'cls', 'kind', 'fmt', 'desc', 'ref', 'ext', 'tier', 'rank')
    def __init__(s, ag, space, a, b, cls, fmt, desc, ref, ext, rank):
        s.ag, s.space, s.a, s.b, s.cls, s.fmt, s.desc, s.ref, s.ext = ag, space, a, b, cls, fmt, desc, ref, ext
        s.kind = kind_of(ag, cls, fmt); s.rank = rank
        s.tier = 0 if s.kind == 'unknown' else 1
        if s.kind == 'container': s.tier = 2 if ag == 'lvlA' else -1   # gzip members stay one row; other containers lose to any content claim

def ext_path(ag, e):
    e = clean(e)
    if not e or e in ('-',): return '-'
    return e if e.startswith(ag + '/') or ag in ('code', 'none') else ag + '/' + e

def load_maps(game, level_sizes):
    rom, lvl = [], collections.defaultdict(list)
    bad = collections.Counter()
    for m in sorted(glob.glob(S + '/*/MAP.tsv')):
        ag = m.split('/')[-2]
        if ag == 'ledger': continue
        for l in open(m, errors='replace'):
            c = l.rstrip('\n').split('\t')
            if len(c) < 4 or c[0] != game: continue
            if len(c) == 7:   # lvlE: format/description merged
                fmt, desc, ref, ext = c[4], c[4], c[5], c[6]
            else:
                c += [''] * (8 - len(c)); fmt, desc, ref, ext = c[4], c[5], c[6], c[7]
            fmt, desc, ref, ext = clean(fmt), clean(desc), clean(ref), ext_path(ag, ext)
            s0, s1 = c[1].strip(), c[2].strip()
            mm0 = re.match(r'^([A-Za-z0-9_]+\.bif)[:+](0x[0-9A-Fa-f]+)$', s0)
            mm1 = re.match(r'^([A-Za-z0-9_]+\.bif)[:+](0x[0-9A-Fa-f]+)$', s1)
            try:
                if mm0 and mm1:
                    f = mm0.group(1).lower(); a, b = num(mm0.group(2)), num(mm1.group(2))
                    if f not in level_sizes or not (0 <= a < b <= level_sizes[f]): bad[(ag, 'lvl-range')] += 1; continue
                    if ag not in RANK_LVL: bad[(ag, 'lvl-agent')] += 1; continue
                    lvl[f].append(Row(ag, f, a, b, c[3], fmt, desc, ref, ext, RANK_LVL[ag]))
                else:
                    a, b = num(s0), num(s1)
                    if not (0 <= a < b <= SIZE): bad[(ag, 'rom-range')] += 1; continue
                    rom.append(Row(ag, 'ROM', a, b, c[3], fmt, desc, ref, ext, RANK_ROM[ag]))
            except ValueError:
                bad[(ag, 'parse')] += 1
    return rom, lvl, bad

def load_code(game):
    st = {}
    p = VER + '/%s/VERIFY.tsv' % game
    for l in open(p):
        c = l.rstrip('\n').split('\t')
        if len(c) > 3: st[int(c[0], 16)] = (c[3].split(' ')[0], c[5] if len(c) > 5 else '')
    out = []
    for f in json.load(open(FUNCS)):
        if f['game'] != game: continue
        s, src = st.get(f['vram'], ('UNVERIFIED', ''))
        out.append(Row('code', 'ROM', f['rom'], f['rom'] + f['size'], 'code', 'R4300 MIPS (%s)' % f['cls'],
                       'func_%08X (%s) verify v16: %s' % (f['vram'], f['cls'], s), 'functions.json',
                       'sesverify/v16/%s/%s' % (game, src or 'src'), RANK_ROM['code']))
    return out

def paint(rows, size):
    arr = array('i', [-1]) * size
    order = sorted(range(len(rows)), key=lambda i: (rows[i].tier, rows[i].rank, -(rows[i].b - rows[i].a)))
    for i in order: arr[rows[i].a:rows[i].b] = array('i', [i]) * (rows[i].b - rows[i].a)
    return arr

def runs(arr):
    res = []; pos = 0
    for k, g in itertools.groupby(arr):
        n = sum(1 for _ in g); res.append((pos, pos + n, k)); pos += n
    return res

def fill_gaps(rows, arr, space, data, game):
    """unclaimed bytes become explicit rows owned by 'none' (class unknown, or pad if verifiably zero)."""
    for a, b, i in runs(arr):
        if i != -1: continue
        zero = data is not None and data.count(0, a, b) == b - a
        where = ''
        if space == 'ROM' and TEXT[game][0] <= a < TEXT[game][1]: where = ' inside main .text, between functions'
        if zero:
            r = Row('none', space, a, b, 'pad', 'zero fill', 'claimed by no map; all bytes zero (ledger check)' + where,
                    '-', '-', 0)
        else:
            r = Row('none', space, a, b, 'unknown', '-', 'claimed by no map' + where +
                    ('' if data is not None else ' (zero test not run: no image given)'), '-', '-', 0)
        rows.append(r); arr[a:b] = array('i', [len(rows) - 1]) * (b - a)

def tile(rows, arr, extra_desc=None):
    out = []
    for a, b, i in runs(arr):
        assert i >= 0, 'gap at %X' % a
        r = rows[i]; d = r.desc
        if (a, b) != (r.a, r.b): d += ' [part 0x%X-0x%X of %s row 0x%X-0x%X]' % (a, b, r.ag, r.a, r.b)
        if extra_desc: d += extra_desc(r)
        out.append((a, b, r, d))
    # zero gaps / zero overlaps
    assert out[0][0] == 0 and out[-1][1] == len(arr), 'tiling does not span the space'
    for x, y in zip(out, out[1:]): assert x[1] == y[0], 'gap/overlap at 0x%X/0x%X' % (x[1], y[0])
    assert sum(b - a for a, b, _, _ in out) == len(arr)
    return out

def conflicts(rows, arr):
    """pairs of overlapping claims from different agents that are not container/content nesting,
    not unknown-vs-identified, and not identical (same kind, same bounds)."""
    res = []; skipped = collections.Counter()
    srt = sorted(range(len(rows)), key=lambda i: (rows[i].a, -rows[i].b))
    active = []
    for i in srt:
        r = rows[i]
        if r.ag == 'none': continue
        active = [j for j in active if rows[j].b > r.a]
        for j in active:
            q = rows[j]
            if q.ag == r.ag: continue
            cq = q.kind == 'container' or q.cls in CONTAINER_CLASSES
            cr = r.kind == 'container' or r.cls in CONTAINER_CLASSES
            if cq or cr:
                if cq and cr and q.kind == r.kind:
                    if (q.a, q.b) == (r.a, r.b): continue
                elif (cq and q.a <= r.a and r.b <= q.b) or (cr and r.a <= q.a and q.b <= r.b):
                    skipped['nesting'] += 1; continue
            if 'unknown' in (q.kind, r.kind): skipped['unknown_vs_identified'] += 1; continue
            same_b = (q.a, q.b) == (r.a, r.b)
            if q.kind == r.kind and same_b: continue
            typ = 'class' if same_b else ('boundary' if q.kind == r.kind else 'class+boundary')
            oa, ob = max(q.a, r.a), min(q.b, r.b)
            own = sorted({rows[k].ag for k in set(arr[oa:ob]) if k >= 0})
            res.append((oa, ob, typ, q, r, ','.join(own)))
        active.append(i)
    return res, skipped

UNUSED_RE = re.compile(r'\b(unused|unreferenced|un-referenced|not referenced|never (?:started|drawn|read|loaded|called|reached)|'
                       r'no caller|no callers|nothing (?:references|loads|points|reaches)|referenced by no|no code symbol|'
                       r'not used|left out|leftover|orphan|cut)\b', re.I)

def unused_from_reports():
    items = []
    for p in sorted(glob.glob(S + '/*/REPORT.md')):
        ag = p.split('/')[-2]
        if ag == 'ledger': continue
        for n, l in enumerate(open(p, errors='replace'), 1):
            if UNUSED_RE.search(l) and not l.startswith('|') and len(l.strip()) > 3:
                items.append((ag, n, clean(l)))
    return items

def main():
    out = sys.argv[1]; roms = {}
    args = sys.argv[2:]
    while args:
        if args[0] == '--rom': g, p = args[1].split('=', 1); roms[g] = p; args = args[2:]
        else: sys.exit('bad argument ' + args[0])
    os.makedirs(out, exist_ok=True)
    summ = ['# Ledger summary (generated by tools/merge_ledger.py; do not edit)', '']
    unused_rows = []
    for game in GAMES:
        lsizes = {}
        for p in glob.glob(S + '/lvlA/out/%s/*.bif.bin' % game):
            lsizes[os.path.basename(p)[:-4].lower()] = os.path.getsize(p)
        assert len(lsizes) == 8, (game, lsizes)
        rom, lvl, bad = load_maps(game, lsizes)
        rom += load_code(game)
        data = open(roms[game], 'rb').read() if game in roms else None
        if data is not None: assert len(data) == SIZE
        # ---- ROM ledger
        gz = {}   # gzip member name -> lvlA row
        for r in rom:
            if r.ag == 'lvlA' and r.cls == 'gzip_level':
                m = re.match(r'(\S+\.bif)', r.desc); gz[m.group(1).lower()] = r
                r.desc += ' -> contents: LEVELS_%s.tsv file=%s' % (game, m.group(1).lower())
        arr = paint(rom, SIZE); fill_gaps(rom, arr, 'ROM', data, game)
        led = tile(rom, arr)
        hdr = 'start\tend\tclass\tformat\tdescription\towner\textractor\tkind\treference\n'
        with open(out + '/LEDGER_%s.tsv' % game, 'w') as f:
            f.write(hdr)
            for a, b, r, d in led:
                f.write('0x%06X\t0x%06X\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (a, b, r.cls, r.fmt or '-', d, r.ag, r.ext, r.kind, r.ref or '-'))
        cf, sk = conflicts(rom, arr)
        allcf = [('ROM', x) for x in cf]
        # ---- level ledgers
        lv_tiles = {}; sk_l = collections.Counter()
        for fn in sorted(lsizes):
            rows = lvl.get(fn, []); n = lsizes[fn]
            img = None
            p = S + '/lvlA/out/%s/%s.bin' % (game, fn)
            if os.path.exists(p): img = open(p, 'rb').read(); assert len(img) == n
            la = paint(rows, n); fill_gaps(rows, la, fn, img, game)
            lv_tiles[fn] = (rows, tile(rows, la))
            c2, s2 = conflicts(rows, la); sk_l.update(s2)
            allcf += [(fn, x) for x in c2]
        with open(out + '/LEVELS_%s.tsv' % game, 'w') as f:
            f.write('file\t' + hdr)
            for fn in sorted(lv_tiles):
                for a, b, r, d in lv_tiles[fn][1]:
                    f.write('%s\t0x%05X\t0x%05X\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (fn, a, b, r.cls, r.fmt or '-', d, r.ag, r.ext, r.kind, r.ref or '-'))
        with open(out + '/CONFLICTS_%s.tsv' % game, 'w') as f:
            f.write('space\toverlap_start\toverlap_end\ttype\tagent_A\tA_start\tA_end\tA_class\tA_kind\tA_description\t'
                    'agent_B\tB_start\tB_end\tB_class\tB_kind\tB_description\tledger_owner\n')
            for sp, (oa, ob, typ, q, r, own) in allcf:
                f.write('%s\t0x%X\t0x%X\t%s\t%s\t0x%X\t0x%X\t%s\t%s\t%s\t%s\t0x%X\t0x%X\t%s\t%s\t%s\t%s\n' % (
                    sp, oa, ob, typ, q.ag, q.a, q.b, q.cls, q.kind, q.desc[:160], r.ag, r.a, r.b, r.cls, r.kind, r.desc[:160], own))
        # ---- summary
        summ += ['## %s' % game, '']
        if bad: summ += ['Map lines skipped (unparseable or out of range): ' + ', '.join('%s %s %d' % (k[0], k[1], v) for k, v in sorted(bad.items())), '']
        for title, tl, tot in [('ROM (LEDGER_%s.tsv)' % game, led, SIZE)] + \
                [('level image %s (%d bytes inflated)' % (fn, lsizes[fn]), lv_tiles[fn][1], lsizes[fn]) for fn in sorted(lv_tiles)]:
            kc = collections.Counter(); oc = collections.Counter()
            for a, b, r, d in tl: kc[r.kind] += b - a; oc[r.ag] += b - a
            summ += ['### %s' % title, '', 'bytes per kind: ' + ', '.join('%s %d (%.2f%%)' % (k, v, 100.0 * v / tot) for k, v in kc.most_common()), '',
                     'bytes per owner: ' + ', '.join('%s %d (%.2f%%)' % (k, v, 100.0 * v / tot) for k, v in oc.most_common()), '']
        lt = collections.Counter(); lo = collections.Counter(); ltot = 0
        for fn in lv_tiles:
            for a, b, r, d in lv_tiles[fn][1]: lt[r.kind] += b - a; lo[r.ag] += b - a; ltot += b - a
        summ += ['### all 8 level images together (%d bytes)' % ltot, '',
                 'bytes per kind: ' + ', '.join('%s %d (%.2f%%)' % (k, v, 100.0 * v / ltot) for k, v in lt.most_common()), '',
                 'bytes per owner: ' + ', '.join('%s %d (%.2f%%)' % (k, v, 100.0 * v / ltot) for k, v in lo.most_common()), '']
        cc = collections.Counter((sp == 'ROM' and 'ROM' or 'levels', x[2], *sorted((x[3].ag, x[4].ag))) for sp, x in allcf)
        summ += ['### conflicts (CONFLICTS_%s.tsv): %d pairs' % (game, len(allcf)), '',
                 'not counted as conflicts: ROM %s; levels %s' % (dict(sk), dict(sk_l)), '', '| space | type | agents | pairs |', '|---|---|---|---|']
        summ += ['| %s | %s | %s vs %s | %d |' % (k[0], k[1], k[2], k[3], v) for k, v in sorted(cc.items(), key=lambda x: -x[1])]
        summ += ['']
        # unknown list (adjacent unknown rows merged)
        def merged_unknown(tl):
            res = []
            for a, b, r, d in tl:
                if r.kind != 'unknown': continue
                if res and res[-1][1] == a: res[-1][1] = b; res[-1][2].add(r.ag)
                else: res.append([a, b, {r.ag}, d])
            return res
        u = merged_unknown(led)
        summ += ['### still unknown / unidentified: ROM, %d ranges, %d bytes' % (len(u), sum(b - a for a, b, _, _ in u)), '']
        summ += ['- 0x%06X-0x%06X (%d B, %s): %s' % (a, b, b - a, '/'.join(sorted(o)), d[:200]) for a, b, o, d in u]
        summ += ['']
        for fn in sorted(lv_tiles):
            u = merged_unknown(lv_tiles[fn][1])
            summ += ['### still unknown / unidentified: %s %s, %d ranges, %d bytes' % (game, fn, len(u), sum(b - a for a, b, _, _ in u)), '']
            summ += ['- 0x%05X-0x%05X (%d B, %s): %s' % (a, b, b - a, '/'.join(sorted(o)), d[:160]) for a, b, o, d in u]
            summ += ['']
        # unused rows in the maps
        for sp, rows in [('ROM', rom)] + [(fn, lv_tiles[fn][0]) for fn in sorted(lv_tiles)]:
            for r in rows:
                if r.ag != 'none' and (UNUSED_RE.search(r.desc) or UNUSED_RE.search(r.ref) or r.cls.startswith('unused')):
                    unused_rows.append((game, sp, r))
    summ += ['## Cut and unused content', '', '### stated in the agents\' REPORT.md files', '']
    summ += ['- %s (REPORT.md line %d): %s' % x for x in unused_from_reports()]
    summ += ['', '### map rows whose description or reference says unused / unreferenced (%d)' % len(unused_rows), '']
    summ += ['- %s %s 0x%X-0x%X %s [%s]: %s | %s' % (g, sp, r.a, r.b, r.ag, r.cls, r.desc[:180], r.ref[:100]) for g, sp, r in unused_rows]
    open(out + '/SUMMARY.md', 'w').write('\n'.join(summ) + '\n')
    print('ok: tilings asserted gap-free and overlap-free; outputs in', out)

if __name__ == '__main__':
    main()
