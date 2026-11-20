"""twins.py ROM_LETTER ROM_NUMBER : for every pal+texture object r1 identified, look for byte-identical copies of
its texels and of its palette anywhere in both games (program, common block, all 16 inflated level images), and
compare each bump block with the referenced texture right after it. Prints locations only (no ROM bytes)."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from r1common import GAMES, load_images
from extract_r1 import OBJ

roms = {'letter': open(sys.argv[1], 'rb').read(), 'number': open(sys.argv[2], 'rb').read()}
imgs = {g: load_images(roms[g], g) for g in roms}

def find_all(blob, skip):
    hits = []
    for g in imgs:
        for name, (base, b) in imgs[g].items():
            i = b.find(blob)
            while i >= 0:
                if (g, name, i) != skip: hits.append('%s:%s+0x%X' % (g, name, i))
                i = b.find(blob, i + 1)
    return hits

for g in OBJ:
    for where, s, e, kind, p in OBJ[g]:
        if kind != 'citex': continue
        base, b = imgs[g][where]
        off = s - (GAMES[g]['common_rom'] if where == 'common' else 0)
        n = 32 if p['bits'] == 4 else 512
        pal, tex = b[off:off + n], b[off + n:off + (e - s)]
        print('%s %s 0x%X: texel copies %s | palette copies %s' % (g, where, s, find_all(tex, (g, where, off + n))[:6],
              find_all(pal, (g, where, off))[:6]))
        if where == 'bump.bif':
            nxt = b[off + (e - s):off + 2 * (e - s)]
            same_pal = nxt[:n] == pal
            diff = sum(1 for x, y in zip(nxt[n:], tex) if x != y)
            print('   next object (referenced) at 0x%X: same palette %s, texel bytes differing %d of %d' % (off + e - s, same_pal, diff, len(tex)))
