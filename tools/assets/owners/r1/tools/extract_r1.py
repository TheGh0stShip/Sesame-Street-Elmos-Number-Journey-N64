"""extract_r1.py ROM GAME OUTDIR [check]

Extracts every range that the ledger had left `unknown` (agent r1's territory) from Elmo's Letter Adventure
(GAME=letter) or Elmo's Number Journey (GAME=number) and writes it as a standard file:
  pal+texture objects -> OUTDIR/<game>/<where>_<start>_CI4|CI8_WxH.png (palette in PLTE/tRNS) + a 4x view copy
  vertex arrays        -> .json (Vtx records), bbox/model header -> .json, alignment pad -> listed in index.json
`check` re-encodes every output back to bytes and compares with the ROM (level ranges after inflating the level).
No ROM content lives in this file: all bytes are read from ROM at run time.
"""
import sys, os, json, struct
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, '..', '..', 'gfx', 'tools'))
import n64tex, png
from r1common import GAMES, inflate

# Objects that tile the formerly-unknown ranges.  where: 'common' (ROM offset) or a .bif name (offset in the
# inflated RAM image, base 0x80154000).  kind: pad | citex (pal+texels) | vtx | bbox | mhdr
OBJ = {
 'letter': [
  ('common', 0x14440C, 0x144410, 'pad', {}),
  ('common', 0x152044, 0x152050, 'pad', {}),
  ('common', 0x15C598, 0x15C5A0, 'pad', {}),
  ('common', 0x15FF58, 0x15FF60, 'pad', {}),
  ('common', 0x16035C, 0x160360, 'pad', {}),
  ('common', 0x196CE0, 0x196F00, 'citex', dict(bits=4, w=32, h=32)),
  ('common', 0x1BF3D0, 0x1BF5D0, 'vtx', {}),
  ('common', 0x1BF6B0, 0x1BF6C8, 'bbox', {}),
  ('common', 0x1BF6C8, 0x1BF6F0, 'mhdr', {}),
  ('manor.bif', 0x60B0, 0x6AB0, 'citex', dict(bits=8, w=32, h=64)),
 ],
 'number': [
  ('common', 0x12E3BC, 0x12E3C0, 'pad', {}),
  ('common', 0x1717D8, 0x1717E0, 'pad', {}),
 ] + [('common', 0x1717E0 + k * 0x220, 0x171A00 + k * 0x220, 'citex', dict(bits=4, w=32, h=32)) for k in range(6)] + [
  ('bump.bif', 0x262C0, 0x268C0, 'citex', dict(bits=8, w=32, h=32)),
  ('bump.bif', 0x30250, 0x30850, 'citex', dict(bits=8, w=32, h=32)),
  ('bump.bif', 0x3A1E0, 0x3A7E0, 'citex', dict(bits=8, w=32, h=32)),
  ('bump.bif', 0x44170, 0x44770, 'citex', dict(bits=8, w=32, h=32)),
  ('bump.bif', 0x4FB80, 0x4FDA0, 'citex', dict(bits=4, w=32, h=32)),
 ],
}

def scale(im, k):
    w, h = im.size
    out = png.Img(im.mode, (w * k, h * k), None, im.palette, im.trns)
    out.px = [im.px[(y // k) * w + (x // k)] for y in range(h * k) for x in range(w * k)]
    return out

def main():
    rom = open(sys.argv[1], 'rb').read(); game = sys.argv[2]; outdir = os.path.join(sys.argv[3], game)
    check = len(sys.argv) > 4 and sys.argv[4] == 'check'
    os.makedirs(os.path.join(outdir, 'view'), exist_ok=True)
    G = GAMES[game]; cache = {}
    def img(where):
        if where == 'common': return rom
        if where not in cache: cache[where] = inflate(rom, *G['levels'][where])
        return cache[where]
    index = []; bad = 0
    for where, s, e, kind, p in OBJ[game]:
        b = img(where)[s:e]
        name = '%s_%06X' % (where.replace('.bif', ''), s)
        ram = (s - G['common_rom'] + 0x8024B000) if where == 'common' else 0x80154000 + s
        ent = dict(where=where, start='0x%X' % s, end='0x%X' % e, ram='0x%08X' % ram, kind=kind)
        if kind == 'pad':
            ok = b == bytes(len(b)); ent['zero_bytes'] = len(b)
        elif kind == 'citex':
            n = 32 if p['bits'] == 4 else 512
            pal, tex = b[:n], b[n:]
            assert len(tex) == p['w'] * p['h'] * p['bits'] // 8, name
            im = n64tex.decode('CI', p['bits'], p['w'], p['h'], tex, pal)
            fn = '%s_CI%d_%dx%d.png' % (name, p['bits'], p['w'], p['h'])
            path = os.path.join(outdir, fn); im.save(path)
            scale(im, 4).save(os.path.join(outdir, 'view', fn))
            t2, tl2 = n64tex.encode('CI', p['bits'], png.open_png(path))
            ok = (tl2 + t2) == b
            px = list(im.getdata()); used = sorted(set(px))
            ent.update(file=fn, palette_bytes=n, texel_bytes=len(tex), min_index=used[0], max_index=used[-1],
                       distinct_indices=len(used),
                       zero_palette_entries=sum(1 for i in range(0, n, 2) if pal[i:i + 2] == b'\0\0'))
        elif kind == 'vtx':
            recs = [list(struct.unpack('>hhhHhhBBBB', b[i:i + 16])) for i in range(0, len(b), 16)]
            fn = name + '_vtx.json'; json.dump(recs, open(os.path.join(outdir, fn), 'w'))
            back = json.load(open(os.path.join(outdir, fn)))
            ok = b''.join(struct.pack('>hhhHhhBBBB', *r) for r in back) == b
            ent.update(file=fn, vertices=len(recs), flags_all_zero=all(r[3] == 0 for r in recs))
        elif kind == 'bbox':
            v = list(struct.unpack('>6f', b)); fn = name + '_bbox.json'
            json.dump(dict(min=v[:3], max=v[3:]), open(os.path.join(outdir, fn), 'w'))
            d = json.load(open(os.path.join(outdir, fn))); ok = struct.pack('>6f', *(d['min'] + d['max'])) == b
            ent.update(file=fn)
        elif kind == 'mhdr':
            w = struct.unpack('>10I', b); fn = name + '_model_header.json'
            json.dump(dict(words=['0x%08X' % x for x in w],
                           fields={'+00 u16 ?': w[0] >> 16, '+02 u16 ?': w[0] & 0xFFFF, '+04 u16 nvert': w[1] >> 16,
                                   '+08 dl': '0x%08X' % w[2], '+0C vtx': '0x%08X' % w[3], '+1C vtx': '0x%08X' % w[7],
                                   '+24 bbox': '0x%08X' % w[9]}), open(os.path.join(outdir, fn), 'w'), indent=1)
            d = json.load(open(os.path.join(outdir, fn))); ok = struct.pack('>10I', *[int(x, 16) for x in d['words']]) == b
            ent.update(file=fn)
        ent['roundtrip_ok'] = ok; bad += not ok
        index.append(ent)
        print('%-6s %-10s 0x%06X-0x%06X %-5s %s %s' % (game, where, s, e, kind, ent.get('file', ''), 'OK' if ok else 'MISMATCH'))
    json.dump(index, open(os.path.join(outdir, 'index.json'), 'w'), indent=1)
    if check:
        print('check:', 'all %d objects byte-exact' % len(index) if not bad else '%d MISMATCH' % bad)
        sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
