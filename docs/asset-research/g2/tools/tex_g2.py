#!/usr/bin/env python3
"""PNG form for the `common` pack (packC) textures and TLUTs that the rebuild still read back from .bin dumps.

usage: tex_g2.py extract ROM GAME REBUILD_TSV OUTDIR  -> OUTDIR/<game>/*.png + index.json
       tex_g2.py check   ROM GAME REBUILD_TSV OUTDIR  -> re-encode every PNG (+ recorded tail) and compare with the ROM
REBUILD_TSV = sesassets/r4/out/REBUILD_<game>.tsv: the rows with owner packC, class texture/tlut, source
extracted-verbatim are the territory. Format and size come from packC's display-list analysis (its Pack object:
SetTile of tile 0 = render format, SetTileSize of tile 0 = width/height, LoadTLUT = palette). TLUTs are written
as RGBA16 colour strips (16 colours per row). Codec: gfx/tools n64tex.py + png.py (lossless both ways).
"""
import sys, os, json
H = os.path.dirname(os.path.abspath(__file__))
SES = os.path.normpath(os.path.join(H, '..', '..'))
sys.path.insert(0, os.path.join(SES, 'gfx', 'tools'))
sys.path.insert(0, os.path.join(SES, 'packC', 'tools'))
import n64tex, png
import extract_packC as PC

def territory(tsv):
    out = []
    for l in open(tsv):
        f = l.rstrip('\n').split('\t')
        if len(f) > 6 and f[3] == 'packC' and f[5] in ('texture', 'tlut') and f[6] == 'extracted-verbatim':
            out.append((int(f[0], 16), int(f[1], 16), f[5]))
    return out

def plan(rom, game, tsv):
    p, s = PC.build(rom, game)
    d = p.d; items = []
    for a, b, cls in territory(tsv):
        o = p.objs.get(a - s)
        assert o is not None and o['start'] == a - s and o['end'] == b - s, hex(a)
        it = dict(rom_start='0x%X' % a, rom_end='0x%X' % b, ram='0x%08X' % (PC.BASE + a - s), cls=cls,
                  ref=o['ref'], name='%06X_%s' % (a, cls))
        n = b - a
        if cls == 'tlut':
            ne = n // 2
            w = 16 if ne % 16 == 0 else ne
            it.update(fmt='RGBA', bits=16, w=w, h=ne // w, size_from='LoadTLUT count (%d colours)' % ne)
        else:
            img = o.get('img')
            if img and img[2] * img[3] * img[1] // 8 <= n and img[2] > 0 and img[3] > 0:
                fmt, bits, w, h, pal = img
                it.update(fmt=fmt, bits=bits, w=w, h=h, size_from='display list (SetTile/SetTileSize tile 0)')
                if pal is not None:
                    it['tlut_ram'] = '0x%08X' % (PC.BASE + pal)
            else:
                fmt = o['fmt'].rstrip('0123456789'); bits = int(o['fmt'][len(fmt):])
                texels = n * 8 // bits
                w = next(x for x in (64, 32, 16, 8, 4, 2, 1) if texels % x == 0 and texels >= x)
                it.update(fmt=fmt, bits=bits, w=w, h=texels // w,
                          size_from='LoadBlock byte count only (no SetTileSize): width chosen as %d' % w)
        used = n64tex.nbytes(it['fmt'], it['bits'], it['w'], it['h'])
        it['tail_bytes'] = n - used
        items.append(it)
    return p, s, items

def pal_bytes(p, s, it):
    if it.get('tlut_ram') is None: return None
    a = int(it['tlut_ram'], 16) - PC.BASE
    return p.d[a:a + (32 if it['bits'] == 4 else 512)]

def main():
    mode, romp, game, tsv, od = sys.argv[1:6]
    rom = open(romp, 'rb').read(); gd = os.path.join(od, game); os.makedirs(gd, exist_ok=True)
    ip = os.path.join(gd, 'index.json')
    if mode == 'extract':
        p, s, items = plan(rom, game, tsv)
        for it in items:
            a, b = int(it['rom_start'], 16), int(it['rom_end'], 16)
            used = b - a - it['tail_bytes']
            im = n64tex.decode(it['fmt'], it['bits'], it['w'], it['h'], rom[a:a + used], pal_bytes(p, s, it))
            im.save(os.path.join(gd, it['name'] + '.png'))
            tail = rom[a + used:b]
            if tail:
                it['tail'] = {'zero': len(tail)} if not any(tail) else {'hex': tail.hex()}
        json.dump(dict(game=game, items=items), open(ip, 'w'), indent=1)
    items = json.load(open(ip))['items']
    bad = 0; tot = 0; lit = 0; fmts = {}
    for it in items:
        a, b = int(it['rom_start'], 16), int(it['rom_end'], 16)
        t, _ = n64tex.encode(it['fmt'], it['bits'], png.open_png(os.path.join(gd, it['name'] + '.png')))
        tl = it.get('tail')
        if tl: t += bytes(tl['zero']) if 'zero' in tl else bytes.fromhex(tl['hex'])
        if tl and 'hex' in tl: lit += len(tl['hex']) // 2
        tot += b - a; k = '%s%d %s' % (it['fmt'], it['bits'], it['cls']); fmts[k] = fmts.get(k, 0) + b - a
        if t != rom[a:b]:
            bad += 1; print('MISMATCH', it['name'])
    print(game, 'items', len(items), 'bytes', tot, 'literal tail bytes', lit, 'mismatches', bad, json.dumps(fmts))
    sys.exit(1 if bad else 0)

if __name__ == '__main__':
    main()
