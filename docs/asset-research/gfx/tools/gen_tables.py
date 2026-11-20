"""gen_tables.py SRC_DIR GAME OUT.json
Freeze what the matched C code says about graphics into a JSON of code facts (addresses, command words,
table bases); no ROM bytes. extract_gfx.py reads gfx_<game>.json next to itself."""
import os, re, sys, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import csrc_gfx

# which loaded level image a function's level-range (0x80154000..) addresses belong to: read from the
# D_8012EFD0 (letter) / D_8012F154 (number) level-id checks guarding each call
LEVEL_OF = {
    'letter': {'func_80004010': 'level19', 'func_8001A318': 'level22', 'func_8000B470': 'level21'},
    'number': {'func_80003E18': 'level11', 'func_8001AC1C': 'level22', 'func_8000BA3C': 'level21'},
}
# Number Journey twin functions whose matched source still carries Letter's symbol names: the real Number
# addresses, read from the Number disassembly (sestools s.sh dis) of each function
NUMBER_FIX = {
    'func_80002EA8': {0x8006D518: 0x80076800},
    'func_800125D4': {0x80072160: 0x800731E0},
    'func_800186E8': {0x800744F0: 0x80074FA0, 0x80074508: 0x80074FB8, 0x80074560: 0x80075010},
    'func_8001AC1C': {0x80074664: 0x80075240, 0x80074714: 0x800752F0, 0x80074718: 0x800752F4, 0x800747F4: 0x800753D0,
                      0x800748C4: 0x800754A0, 0x8017B7A0: 0x8017B1A0, 0x8017B9A0: 0x8017B3A0},
}
TABLE_END_FIX = {0x80075240: 0x800752F0, 0x800752F0: 0x800753D0, 0x800753D0: 0x800754A0, 0x800754A0: 0x800754B8}
BG = {'letter': ('func_8001CAA8', 'func_8000B470'), 'number': ('func_8001D3AC', 'func_8000BA3C')}

def main(src, game, out):
    syms = set()
    progs = []
    for f in sorted(os.listdir(src)):
        txt = open(os.path.join(src, f)).read()
        syms.update(int(x, 16) for x in re.findall(r'\bD_(80[0-9A-F]{6})\b', txt))
        fn = f[:-2]
        pr = csrc_gfx.pairs(os.path.join(src, f))
        if not any((w0 >> 24) == 0xFD for _, w0, _, _ in pr):
            continue
        seq = []
        for ln, w0, w1, kind in pr:
            if w1 is not None:
                seq.append([w0, w1, ln])
                continue
            m = re.match(r'\(?\w*\)?\s*D_(80[0-9A-F]{6})\[.*\]\.m([04])$', kind.replace('(u32) ', ''))
            if m:
                seq.append([w0, {'tbl': int(m.group(1), 16), 'field': int(m.group(2))}, ln])
            else:
                seq.append([w0, None, ln])
        fix = NUMBER_FIX.get(fn, {}) if game == 'number' else {}
        for x in seq:
            if isinstance(x[1], int) and x[1] in fix: x[1] = fix[x[1]]
            if isinstance(x[1], dict) and x[1]['tbl'] in fix: x[1]['tbl'] = fix[x[1]['tbl']]
        progs.append({'func': fn, 'level': LEVEL_OF[game].get(fn), 'pairs': seq})
    ss = sorted(syms)
    tables = {}
    for p in progs:
        for w0, w1, ln in p['pairs']:
            if isinstance(w1, dict):
                b = w1['tbl']
                nxt = TABLE_END_FIX.get(b) if game == 'number' and b in TABLE_END_FIX else min([s for s in ss if s > b + 4] or [b + 8])
                tables['%08X' % b] = nxt
    bgs = []
    bfn, caller = BG[game]
    for line in open(os.path.join(src, caller + '.c')):
        m = re.search(bfn + r'\(D_(80[0-9A-F]{6}), D_(80[0-9A-F]{6})\)', line)
        if m:
            t = (int(m.group(1), 16), int(m.group(2), 16))
            if t not in [tuple(x) for x in bgs]:
                bgs.append(list(t))
    # streamed animation files: (ptr, len) pairs written before the anim loader thread runs
    lenvar = {'letter': 'D_800A75E0', 'number': 'D_800A7A50'}[game]
    anims = set()
    for f in sorted(os.listdir(src)):
        for line in open(os.path.join(src, f)):
            if lenvar + ' = ' in line:
                ds = re.findall(r'D_(00[0-9A-F]{6})', line)
                if len(ds) == 2:
                    anims.add((int(ds[1], 16), int(ds[0], 16)))
    json.dump({'game': game, 'anim_files': sorted(anims), 'programs': progs, 'tables': tables, 'backgrounds': bgs,
               'background_func': bfn, 'background_level': 'level21'}, open(out, 'w'), indent=1)

if __name__ == '__main__':
    main(*sys.argv[1:4])
