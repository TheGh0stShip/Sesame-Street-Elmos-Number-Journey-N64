#!/usr/bin/env python3
"""decode_rest.py extract|check ROM GAME OUTDIR
Decoded forms for the pieces the earlier rebuilds still read back as raw dumps:
  ROM rows   r4 REBUILD rows with source extracted-verbatim that g2 did not take over: packC `dl` (Letter
             0x1BF5D0), r1 objects (vertex / bbox / model header / CI4 texture + TLUT / pad), gfx pad and
             model_vtxmap terminator words.
  level rows g3 LEVELREBUILD rows with source extracted-verbatim (lvlE/lvlF/lvlG/lvlH): textures, display lists,
             vertices, collision quads and quad sets, unused texture block, anim tails.
Forms (OUTDIR/<game>/rest/, index.json lists every item):
  dl      F3DEX macro text, one command per line (gfx/tools/f3dex.py + gsSPSetOtherMode, see dl_assemble;
          gsDPWord lines would count as literal)
  tex     CI PNG (indices; grey palette, TLUT not part of the item) via gfx/tools n64tex.py + png.py
  r1      r1's own files (PNG / vtx json / bbox json / model header json): encoded here from r1/out/<game>
  vtx     text, one Vtx per line: x y z flag s t r g b a (decimal)
  quad    JSON: type, w1, mask, w3, plane[4] (f32), corners[4][3] (f32)
  quadset JSON: ptrs[] (RAM addresses), n, base (RAM address of ptrs[0])
  zero    nothing stored: N zero bytes (pad, vtxmap terminator + alignment)
  literal JSON hex (stale bytes / never-loaded block of unknown format): counted as literal
extract decodes from the ROM (level rows from the inflated ROM member); check re-encodes every item from its
file only and compares. encode_item(item, dir) is the importable encoder. No ROM bytes in this tool."""
import sys, os, json, struct, zlib, re
H = os.path.dirname(os.path.abspath(__file__)); S = os.path.normpath(os.path.join(H, '..', '..'))
sys.path.insert(0, os.path.join(S, 'gfx', 'tools'))
import f3dex, n64tex, png
LVLBASE = 0x80154000

def members(game):
    out = {}
    for l in open(S + '/ledger/LEDGER_%s.tsv' % game):
        c = l.rstrip('\n').split('\t')
        if len(c) > 5 and c[5] == 'lvlA' and c[2] == 'gzip_level':
            out[re.search(r'(\w+\.bif)', c[4]).group(1)] = (int(c[0], 16), int(c[1], 16))
    return out

def inflate(rom, s, e):
    n = rom.index(b'\0', s + 10) + 1
    d = zlib.decompressobj(-15); return d.decompress(rom[n:e]) + d.flush()

def lvlE_fmt():
    f = {}
    for l in open(S + '/lvlE/MAP.tsv'):
        c = l.rstrip('\n').split('\t')
        m = re.match(r'fmt (\d) siz (\d) (\d+)x(\d+)', c[4]) if len(c) > 4 else None
        if m and c[3] == 'tex_image': f[c[1]] = (int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)))
    return f

def plan(game):
    """the item list (where, start, end, kind, params) from r4's and g3's REBUILD tables"""
    items = []
    for l in list(open(S + '/r4/out/REBUILD_%s.tsv' % game))[1:]:
        c = l.rstrip('\n').split('\t')
        if c[6] != 'extracted-verbatim' or c[3] in ('ucode',) or c[5] in ('texture', 'tlut') and c[3] == 'packC': continue
        s, e = int(c[0], 16), int(c[1], 16)
        if c[3] == 'packC' and c[5] == 'dl': k = 'dl'
        elif c[3] == 'r1': k = 'r1'
        elif c[5] == 'pad': k = 'zero'
        elif c[5] == 'model_vtxmap': k = 'zero'
        else: k = 'literal'
        items.append(dict(where='rom', start=s, end=e, kind=k, owner=c[3], cls=c[5]))
    tf = lvlE_fmt()
    for l in list(open(S + '/g3/out/LEVELREBUILD_%s.tsv' % game))[1:]:
        c = l.rstrip('\n').split('\t')
        if not c[7].startswith('extracted-verbatim'): continue
        s, e = int(c[1], 16), int(c[2], 16); cls = c[5]; it = dict(where=c[0], start=s, end=e, owner=c[4], cls=cls)
        if cls == 'tex_image':
            fm, sz, w, h = tf['%s:0x%X' % (c[0], s)]
            it.update(kind='tex', fmt=n64tex.FMT[fm], bits=n64tex.SIZ[sz], w=w, h=h)
        elif cls in ('texload_dl', 'display_list'): it['kind'] = 'dl'
        elif cls == 'vertices': it['kind'] = 'vtx'
        elif cls == 'coll_quad': it['kind'] = 'quad'
        elif cls == 'coll_quad_set': it['kind'] = 'quadset'
        else: it['kind'] = 'literal'
        items.append(it)
    return items

def dl_assemble(line):
    """f3dex.assemble, plus gsSPSetOtherMode (f3dex.assemble cannot parse its G_SETOTHERMODE_L/H argument and its
    disasm() then falls back to gsDPWord): w0 = op(B9 L / BA H)<<24 | shift<<8 | length, w1 = data"""
    t = line.split('/*')[0].strip().rstrip(',').strip()
    m = re.match(r'gsSPSetOtherMode\((G_SETOTHERMODE_[LH]),\s*(\w+),\s*(\w+),\s*(\w+)\)$', t)
    if m:
        return ((0xB9 if m.group(1).endswith('L') else 0xBA) << 24) | (int(m.group(2), 0) << 8) | int(m.group(3), 0), int(m.group(4), 0)
    return f3dex.assemble(line)

def dl_disasm(w0, w1):
    t = f3dex._disasm(w0, w1)
    try:
        if dl_assemble(t) == (w0, w1): return t
    except Exception: pass
    return f3dex.disasm(w0, w1)

def name_of(it): return '%s_%06X_%s' % (it['where'].replace('.bif', ''), it['start'], it['cls'])

def decode_item(it, b, d):
    """write the decoded form of bytes b for item it into directory d; may downgrade kind (zero/literal)"""
    k = it['kind']; nm = name_of(it)
    if k == 'r1': return
    if k in ('zero', 'literal') or (k != 'r1' and not any(b)):
        if not any(b): it['kind'] = 'zero'; return
        it['kind'] = 'literal'; it['file'] = nm + '.json'
        json.dump(dict(hex=b.hex()), open(os.path.join(d, it['file']), 'w')); return
    if k == 'dl':
        it['file'] = nm + '.dl.txt'
        open(os.path.join(d, it['file']), 'w').write(''.join(dl_disasm(*struct.unpack('>II', b[i:i + 8])) + '\n' for i in range(0, len(b), 8)))
    elif k == 'tex':
        it['file'] = nm + '.png'
        n64tex.decode(it['fmt'], it['bits'], it['w'], it['h'], b).save(os.path.join(d, it['file']))
    elif k == 'vtx':
        it['file'] = nm + '.vtx.txt'
        open(os.path.join(d, it['file']), 'w').write(''.join(' '.join(map(str, struct.unpack('>hhhHhhBBBB', b[i:i + 16]))) + '\n' for i in range(0, len(b), 16)))
    elif k == 'quad':
        w = struct.unpack('>4I', b[:16]); f = struct.unpack('>16f', b[16:])
        it['file'] = nm + '.json'
        json.dump(dict(type=w[0], w1=w[1], mask=w[2], w3=w[3], plane=list(f[:4]), corners=[list(f[4 + 3 * i:7 + 3 * i]) for i in range(4)]),
                  open(os.path.join(d, it['file']), 'w'))
    elif k == 'quadset':
        w = struct.unpack('>%dI' % (len(b) // 4), b)
        it['file'] = nm + '.json'
        json.dump(dict(ptrs=['0x%08X' % x for x in w[:-2]], n=w[-2], base='0x%08X' % w[-1]), open(os.path.join(d, it['file']), 'w'))

def encode_item(it, d, game):
    """rebuild the item's bytes from its file only. Returns (bytes, literal_byte_count)."""
    k = it['kind']; n = it['end'] - it['start']
    if k == 'zero': return bytes(n), 0
    p = os.path.join(d, it.get('file', ''))
    if k == 'literal': return bytes.fromhex(json.load(open(p))['hex']), n
    if k == 'dl':
        out = b''; lit = 0
        for l in open(p):
            if l.strip():
                out += struct.pack('>II', *dl_assemble(l)); lit += 8 if l.startswith('gsDPWord') else 0
        return out, lit
    if k == 'tex': return n64tex.encode(it['fmt'], it['bits'], png.open_png(p))[0], 0
    if k == 'vtx':
        return b''.join(struct.pack('>hhhHhhBBBB', *map(int, l.split())) for l in open(p) if l.strip()), 0
    if k == 'quad':
        j = json.load(open(p))
        return struct.pack('>4I', j['type'], j['w1'], j['mask'], j['w3']) + struct.pack('>16f', *(j['plane'] + sum(j['corners'], []))), 0
    if k == 'quadset':
        j = json.load(open(p)); return struct.pack('>%dI' % (len(j['ptrs']) + 2), *([int(x, 16) for x in j['ptrs']] + [j['n'], int(j['base'], 16)])), 0
    if k == 'r1': return encode_r1(it, game), 0
    raise ValueError(k)

_R1 = {}
def encode_r1(it, game):
    """the r1 objects covering [start, end) re-encoded from r1's extracted files, sliced to the item"""
    if game not in _R1:
        d = S + '/r1/out/' + game; m = bytearray(); objs = []
        for o in json.load(open(d + '/index.json')):
            if o['where'] != 'common': continue
            s, e = int(o['start'], 16), int(o['end'], 16)
            if o['kind'] == 'pad': b = bytes(e - s)
            elif o['kind'] == 'citex':
                t, tl = n64tex.encode('CI', 4 if '_CI4_' in o['file'] else 8, png.open_png(d + '/' + o['file'])); b = tl + t
            elif o['kind'] == 'vtx': b = b''.join(struct.pack('>hhhHhhBBBB', *r) for r in json.load(open(d + '/' + o['file'])))
            elif o['kind'] == 'bbox': j = json.load(open(d + '/' + o['file'])); b = struct.pack('>6f', *(j['min'] + j['max']))
            elif o['kind'] == 'mhdr': b = struct.pack('>10I', *[int(x, 16) for x in json.load(open(d + '/' + o['file']))['words']])
            objs.append((s, b))
        _R1[game] = objs
    for s, b in _R1[game]:
        if s <= it['start'] and it['end'] <= s + len(b): return b[it['start'] - s:it['end'] - s]
    raise KeyError('r1 object for 0x%X' % it['start'])

def main():
    mode, romp, game, od = sys.argv[1:5]
    d = os.path.join(od, game, 'rest'); os.makedirs(d, exist_ok=True); ip = os.path.join(d, 'index.json')
    rom = open(romp, 'rb').read(); mem = members(game); imgs = {}
    def src(it):
        if it['where'] == 'rom': return rom
        if it['where'] not in imgs: imgs[it['where']] = inflate(rom, *mem[it['where']])
        return imgs[it['where']]
    if mode == 'extract':
        items = plan(game)
        for it in items: decode_item(it, src(it)[it['start']:it['end']], d)
        json.dump(items, open(ip, 'w'), indent=0)
    items = json.load(open(ip)); bad = 0; tot = {}
    for it in items:
        b, lit = encode_item(it, d, game)
        ok = b == src(it)[it['start']:it['end']]; bad += not ok
        k = (it['where'] == 'rom', it['kind']); tot[k] = tot.get(k, 0) + it['end'] - it['start']
        tot[('literal-bytes', it['where'] == 'rom')] = tot.get(('literal-bytes', it['where'] == 'rom'), 0) + lit
        if not ok: print('MISMATCH', name_of(it))
    print(game, 'items', len(items), 'mismatches', bad, {'%s/%s' % k: v for k, v in sorted(tot.items(), key=str)})
    sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
