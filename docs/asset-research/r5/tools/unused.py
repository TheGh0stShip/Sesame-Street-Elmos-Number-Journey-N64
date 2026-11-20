#!/usr/bin/env python3
"""unused.py LETTER_ROM NUMBER_ROM FUNCTIONS_JSON SESASSETS_DIR OUTDIR

Re-verifies, item by item, every unused / unreferenced content claim collected in ledger/SUMMARY.md, plus a full
pass over the songs, speech clips and morph-animation files, the "123" icon, the Factor 5 screen and the object
ids that have code but no placement. For each item it searches:
  code   every %hi/%lo address build (lui + addiu/ori/load/store) in ALL functions, for any value inside the item
         (so base+offset accesses are found too); the builder is marked reachable or not (OUTDIR/graph_<game>.json);
  data   every 32-bit word of the program .data/.rodata with a value inside the item;
  image  every 32-bit word of the image the item lives in (the same level image, plus the common block that is
         loaded with every level; or the same menu file) with a value inside the item, also as a segment-4 address
         when the item gives one;
  rom    for ROM-addressed files (songs, clips, animations): code builds and data words equal to the start offset
         (how the game names a file), and separately the end offset.
Verdict CONFIRMED (no reference of any kind) or STRUCK (a reference exists: listed). Writes OUTDIR/UNUSED_<game>.tsv.
Limit: addresses computed at run time from an unrelated base (index * stride) are not seen; items where the code
uses such arithmetic say so in the note."""
import sys, os, re, json, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from seslib import *

SEG4 = {('number', 'river.bif', 0x655F8): 0x13D8, ('number', 'river.bif', 0x65618): 0x13F8, ('number', 'ski.bif', 0x370E8): 0xAA0, ('number', 'ski.bif', 0x37108): 0xAC0}
# manual review notes from reading the matched C (verdict override, note)
REVIEW = {
    ('number', 'ROM', 0xA3CC0): ('STRUCK', 'Number draws this "123" icon: its mode-22 drawer func_8001AC1C builds 0x8017B1A0 (TLUT) and 0x8017B3A0 (texels) where Letter func_8001A318 builds the ABC icon 0x8017B7A0/0x8017B9A0; packA read Number through Letter symbol names. The unused icon in Number is the ABC one (item r5 0xA42C0)'),
    ('letter', 'ROM', 0x184968): ('CONFIRMED', 'claim "no pointer anywhere" is wrong: D_80075D00[9] (word 0x80075D24) points at it, but func_8002AB50 indexes D_80075D00 by actor+0x1D4 and index 9 is never stored there: func_800356D0 is called only with the constants 0,2,3,4,5,7,8,10-19; func_8002A120 stores 2; func_80023118/func_80026B70 store 0; the generic setter func_80035720 is unreachable'),
}
# consumers whose addresses belong to one RAM image only (from the matched C): the level HUD TexPair tables point into
# the level images, the mode-22 drawer into menu22
LEVEL_FUNCS = {'letter': {0x80004010}, 'number': {0x80003E18}}
MENU22_FUNCS = {'letter': {0x8001A318}, 'number': {0x8001AC1C}}
ROW = re.compile(r'^- (letter|number) (ROM|\S+\.bif) 0x([0-9A-F]+)-0x([0-9A-F]+) (\w+) \[(\w+)\]: (.*)$')
SKIP_KINDS = {'pad', 'anim_tail'}
EXTRA = [  # (game, where, start, end, owner, kind, text, seg4_offset)
    ('number', 'ROM', 0x7470D0, 0x758920, 'sprA', 'morph_anim', 'animation packed after 0x739C20 inside its file (58 keys, 309 verts)', None),
    ('number', 'ROM', 0x758920, 0x75F380, 'sprA', 'morph_anim', 'animation packed after 0x7470D0 inside the 0x739C20 file (22 keys, 309 verts)', None),
    ('number', 'maze.bif', 0x4B190, 0x5BFF4, 'lvlH', 'morph_anim', 'maze morph animation 0x8019F190 (51 keys, 198 ticks; lvlH: unreferenced)', None),
    ('number', 'ROM', 0xA42C0, 0xA48C0, 'r5', 'texture', 'mode-22 menu "ABC" icon (TLUT 0x8017B7A0 + CI8 32x32 0x8017B9A0) in Number, whose drawer uses the "123" icon instead', None),
]


def main(lrom, nrom, fj, sesdir, outdir):
    rows = {'letter': [], 'number': []}
    for line in open(os.path.join(sesdir, 'ledger', 'SUMMARY.md')):
        m = ROW.match(line.rstrip('\n'))
        if not m or m.group(6) in SKIP_KINDS: continue
        g, where, a, b, owner, kind, text = m.groups()
        rows[g].append((where, int(a, 16), int(b, 16), owner, kind, text[:150], None, 'ledger'))
    seen = {(g, r[0], r[1]) for g in rows for r in rows[g]}
    for g in ('letter', 'number'):
        for r in csv.DictReader(open(os.path.join(sesdir, 'ledger', 'LEVELS_%s.tsv' % g)), delimiter='\t'):
            d = (r['class'] + ' ' + r['description']).lower()
            if r['kind'] in SKIP_KINDS or 'alignment' in d or 'slack' in d or not re.search(r'unused|unreferenced|no load reaches|nothing (loads|references)', d): continue
            a = int(r['start'], 16)
            if (g, r['file'], a) in seen: continue
            seen.add((g, r['file'], a))
            rows[g].append((r['file'], a, int(r['end'], 16), r['owner'], r['kind'], r['description'][:150], SEG4.get((g, r['file'], a)), 'ledger-levels'))
    for g, where, a, b, owner, kind, text, seg in EXTRA:
        rows[g].append((where, a, b, owner, kind, text, seg, 'report'))
    for game, rp in (('letter', lrom), ('number', nrom)):
        rom = open(rp, 'rb').read()
        fs = load_functions(fj, game)
        graph = json.load(open(os.path.join(outdir, 'graph_%s.json' % game)))
        code = scan_code(rom, fs)
        builds = []
        for v, c in code.items():
            for t, pc in c['addrs']: builds.append((t, v, pc))
        builds.sort()
        import bisect
        bkeys = [x[0] for x in builds]
        dw = sorted((w, a) for a, w in data_words(rom, game))
        dkeys = [x[0] for x in dw]
        files = files_table(os.path.join(sesdir, 'files', 'FILES_%s.tsv' % game))
        images, imgbase, filename = {}, {}, {}
        for a, b, cls, note in files:
            if cls == 'gzip_level':
                nm = gzip_name(rom, a); images[nm] = gunzip_member(rom, a, b); imgbase[nm] = 0x80154000; filename[a] = nm
            elif cls == 'gfx_pack':
                nm = note.split(':')[0]; images[nm] = rom[a:b]; filename[a] = nm
                imgbase[nm] = 0x8024B000 if nm == 'common' else 0x80154000
        starts = {}
        for r in csv.DictReader(open(os.path.join(sesdir, 'ledger', 'LEVELS_%s.tsv' % game)), delimiter='\t'):
            starts.setdefault(r['file'], set()).add(int(r['start'], 16))
        for r in csv.DictReader(open(os.path.join(sesdir, 'ledger', 'LEDGER_%s.tsv' % game)), delimiter='\t'):
            st = int(r['start'], 16)
            for a, b, cls, note in files:
                if cls == 'gfx_pack' and a <= st < b: starts.setdefault(filename[a], set()).add(st - a)

        dobj = []
        for f in ('data1', 'data2'):
            for r in csv.DictReader(open(os.path.join(sesdir, f, 'MAP.tsv')), delimiter='\t'):
                if r['game'] == game:
                    dobj.append((int(r['rom_start'], 16) + RAM2ROM, int(r['rom_end'], 16) + RAM2ROM,
                                 {int(x, 16) for x in re.findall(r'func_([0-9A-F]{8})', r['referenced_by'])}))

        def consumers(h):
            m1 = re.search(r'func_([0-9A-F]{8})', h)
            if h.startswith('code') and m1: return {int(m1.group(1), 16)}
            m2 = re.match(r'data@([0-9A-F]{8})', h)
            if m2:
                a = int(m2.group(1), 16)
                for lo, hi, fns in dobj:
                    if lo <= a < hi: return fns
            return set()

        def owners_of(addr):
            """images (loaded at 0x80154000 / common) that have an object starting at addr"""
            out = []
            for nm, base in imgbase.items():
                if base <= addr < base + len(images[nm]) and (addr - base) in starts.get(nm, ()): out.append(nm)
            return out
        iwords = {}
        for nm, img in images.items():
            l = sorted((int.from_bytes(img[o:o + 4], 'big'), o) for o in range(0, len(img) - 3, 4))
            iwords[nm] = (l, [x[0] for x in l])

        def rng(keys, lst, lo, hi):
            i = bisect.bisect_left(keys, lo); out = []
            while i < len(keys) and keys[i] < hi: out.append(lst[i]); i += 1
            return out

        def file_of_rom(off):
            for a, b, cls, note in files:
                if a <= off < b: return a, b, cls, note
            return None

        def check(where, a, b, kind, text, seg):
            hits = []
            ram_lo = ram_hi = None
            img = None
            m = re.search(r'RAM 0x([0-9A-F]{8})', text)
            if where != 'ROM':
                img = where; ram_lo = imgbase[img] + a; ram_hi = imgbase[img] + b
            else:
                f = file_of_rom(a)
                if f and f[2] == 'gfx_pack':
                    img = filename[f[0]]; ram_lo = imgbase[img] + a - f[0]; ram_hi = ram_lo + (b - a)
                elif m:
                    ram_lo = int(m.group(1), 16); ram_hi = ram_lo + (b - a)
                elif SEG[game]['data'][0] <= a + RAM2ROM < SEG[game]['rodata'][1]:
                    ram_lo, ram_hi = a + RAM2ROM, b + RAM2ROM
            searched = []
            if ram_lo is not None:
                searched.append('code+data RAM %08X-%08X' % (ram_lo, ram_hi))
                for t, v, pc in rng(bkeys, builds, ram_lo, ram_hi):
                    hits.append('code %s@%08X->%08X%s' % ('func_%08X' % v, pc, t, '' if graph['%08X' % v]['reach_any'] else '(unreachable fn)'))
                for w, da in rng(dkeys, dw, ram_lo, ram_hi):
                    hits.append('data@%08X->%08X' % (da, w))
                scan = [img] if img and img not in ('menu21', 'menu22') else []
                if img and img in imgbase and imgbase[img] == 0x80154000 and img.endswith('.bif'): scan.append('common')
                if not img and ram_lo >= 0x8024B000: scan.append('common')
                for nm in scan:
                    l, k = iwords[nm]
                    for w, o in rng(k, l, ram_lo, ram_hi):
                        hits.append('word %s+%X->%08X' % (nm, o, w))
                    if seg is not None:
                        for w, o in rng(k, l, 0x04000000 + seg, 0x04000000 + seg + 1):
                            hits.append('segword %s+%X->%08X' % (nm, o, w))
                searched.append('words of ' + (', '.join(scan) or 'no image'))
                if seg is not None: searched.append('segment-4 address 0x%08X' % (0x04000000 + seg))
            else:
                searched.append('code builds + data words == ROM start 0x%06X' % a)
                for t, v, pc in rng(bkeys, builds, a, a + 1):
                    hits.append('code func_%08X@%08X builds start%s' % (v, pc, '' if graph['%08X' % v]['reach_any'] else '(unreachable fn)'))
                for w, da in rng(dkeys, dw, a, a + 1):
                    hits.append('data@%08X == start' % da)
            if ram_lo is not None:
                kept, coll = [], []
                for h in hits:
                    mm = re.search(r'->([0-9A-F]{8})', h); t = int(mm.group(1), 16)
                    if h.startswith('word ') and img and h.split()[1].startswith(img + '+') and ram_lo <= imgbase[img] + int(h.split('+')[1].split('-')[0], 16) < ram_hi:
                        continue                                   # pointer inside the item itself
                    if t == 0x80154000 and ram_lo == 0x80154000:
                        coll.append('file base only'); continue    # load address of the whole file
                    if t >= 0x8024B000 or not img or img == 'common':
                        kept.append(h); continue
                    cs = consumers(h)
                    if cs and img.endswith('.bif') and cs <= MENU22_FUNCS[game]:
                        coll.append('%s=menu22 drawer table' % h.split()[0]); continue
                    if cs and img == 'menu22' and cs <= LEVEL_FUNCS[game]:
                        coll.append('%s=level HUD table/code' % h.split()[0][:40]); continue
                    own = owners_of(t)
                    if own and img not in own:
                        coll.append('%s=object in %s' % (h.split()[0] if not h.startswith('code') else h.split()[1].split('@')[0], '/'.join(sorted(own))[:60]))
                    else:
                        kept.append(h)
                hits = kept
                if coll: searched.append('ignored (same RAM, other file): ' + '; '.join(sorted(set(coll)))[:400])
            else:
                kept = []
                for h in hits:
                    fn = re.search(r'func_([0-9A-F]{8})@([0-9A-F]{8})', h)
                    if fn:
                        v, pc = int(fn.group(1), 16), int(fn.group(2), 16)
                        # ROM offsets are passed as (start, end) pairs built next to each other: the closest other
                        # ROM-offset build decides whether this one is a start (partner above) or an end (partner below)
                        near = sorted((abs(p2 - pc), t2) for t2, p2 in code[v]['addrs'] if 0x1000 <= t2 <= 0x800000 and t2 != a)
                        if near and near[0][1] < a and near[0][0] <= 0x40:
                            searched.append('func_%08X@%08X builds 0x%06X next to 0x%06X: the end bound of that file, not a reference' % (v, pc, a, near[0][1])); continue
                    kept.append(h)
                hits = kept
            endref = []
            if ram_lo is None:
                for t, v, pc in rng(bkeys, builds, b, b + 1): endref.append('func_%08X' % v)
                for w, da in rng(dkeys, dw, b, b + 1): endref.append('data@%08X' % da)
            return hits, searched, endref

        out = []
        for where, a, b, owner, kind, text, seg, src in rows[game]:
            hits, searched, endref = check(where, a, b, kind, text, seg)
            out.append((where, a, b, owner, kind, text, src, hits, searched, endref))
        # full passes: songs, sprite_anim files, speech clips
        for a, b, cls, note in files:
            if cls in ('libmus_song', 'sprite_anim'):
                hits, searched, endref = check('ROM', a, b, cls, '', None)
                out.append(('ROM', a, b, 'files', cls, 'every %s file (full pass)' % cls, 'full-pass', hits, searched, endref))
        for r in csv.DictReader(open(os.path.join(sesdir, 'speech', 'CLIPS_%s.tsv' % game)), delimiter='\t'):
            a, b = int(r['rom_start'], 16), int(r['rom_end'], 16)
            hits, searched, endref = check('ROM', a, b, 'speech_clip', '', None)
            out.append(('ROM', a, b, 'speech', 'speech_clip', 'speech clip %s (%ss)' % (r['id'], r['seconds']), 'full-pass', hits, searched, []))
        # object ids with code but no placement
        objl = {'letter': {'streetab.bif': 0x801A9F5C, 'chute.bif': 0x801934D4, 'manor.bif': 0x8019411C, 'orchard.bif': 0x8019906C,
                           'garden.bif': 0x801AC70C, 'space.bif': 0x8018FD80, 'planet.bif': 0x801A87FC, 'bonusabc.bif': 0x801650D4},
                'number': {'street.bif': 0x801AA2A4, 'dunk.bif': 0x8017D82C, 'bump.bif': 0x80174B50, 'mansion.bif': 0x80177D44,
                           'maze.bif': 0x8018F46C, 'ski.bif': 0x80176BAC, 'river.bif': 0x80197E10, 'bonus123.bif': 0x80167F40}}[game]
        placed = set()
        for nm, lst in objl.items():
            img = images[nm]; w = lambda x: int.from_bytes(img[x - 0x80154000:x - 0x80154000 + 4], 'big')
            p = lst
            while w(p):
                r = w(p); placed.add((w(r), int.from_bytes(img[r - 0x80154000 + 6:r - 0x80154000 + 8], 'big'), nm)); p += 4
        ids = {'letter': [(7, 0x2005), (7, 0x3503), (7, 0x3505)], 'number': [(12, 0x8103), (12, 0x6102)]}[game]
        for t, i in ids:
            pl = sorted(nm for tt, ii, nm in placed if tt == t and ii == i)
            li = []
            for f in fs:
                for o, wd in words(rom, f['rom'], f['rom'] + f['size']):
                    if (wd >> 26) in (9, 13) and ((wd >> 21) & 31) == 0 and (wd & 0xFFFF) == i:
                        li.append('func_%08X@%08X' % (f['vram'], f['vram'] + o - f['rom']))
            hits = ['placed in ' + ','.join(pl)] if pl else []
            out.append(('objlist', i, i, 'lvlD', 'object_id', 'type %d id 0x%04X: handler code exists, no placement claimed' % (t, i), 'report',
                        hits, ['type/id of every record of the 8 level object lists (record +0 type, +6 id)',
                               'li/ori constants 0x%04X in code: %s (switch cases and compares; a spawn would pass it as an argument)' % (i, ','.join(li) or 'none')], []))
        with open(os.path.join(outdir, 'UNUSED_%s.tsv' % game), 'w') as o:
            o.write('game\twhere\tstart\tend\towner\tkind\tclaim\tsource\tverdict\treferences_found\tsearched\tend_offset_referenced_by\n')
            n = {}
            for where, a, b, owner, kind, text, src, hits, searched, endref in out:
                if src == 'full-pass': verdict = 'used' if hits else 'UNUSED'
                else: verdict = 'STRUCK' if hits else 'CONFIRMED'
                rv = REVIEW.get((game, where, a))
                if rv: verdict = rv[0]; searched = searched + ['REVIEW: ' + rv[1]]
                n[verdict] = n.get(verdict, 0) + 1
                o.write('%s\t%s\t0x%X\t0x%X\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' % (game, where, a, b, owner, kind, text.replace('\t', ' '), src, verdict,
                        '; '.join(hits[:8]) + (' (+%d more)' % (len(hits) - 8) if len(hits) > 8 else '') or '-', '; '.join(searched), ','.join(endref) or '-'))
        print(game, len(out), 'items', n)


if __name__ == '__main__':
    main(*sys.argv[1:6])
