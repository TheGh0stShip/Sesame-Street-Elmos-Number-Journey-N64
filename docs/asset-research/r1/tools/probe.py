"""probe.py ROM GAME : dump the unknown ranges with context and search every pointer source for them.
Analysis helper for r1 (prints hex to the lab log only; nothing is written to the deliverables)."""
import sys, struct, zlib, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from r1common import GAMES, load_images, code_refs

rom = open(sys.argv[1], 'rb').read(); game = sys.argv[2]
G = GAMES[game]
imgs = load_images(rom, game)          # name -> (ram_base, bytes)
crefs = code_refs(rom, game)           # list of (ram, insn_addr)

def hexdump(b, base, n=None):
    for i in range(0, len(b), 16):
        print('  %08X  %s' % (base + i, ' '.join('%02X' % x for x in b[i:i + 16])))

def search(lo, hi, own):
    hits = []
    for name, (base, b) in imgs.items():
        if name.endswith('.bif') and name != own: continue
        for i in range(0, len(b) - 3, 4):
            v = struct.unpack_from('>I', b, i)[0]
            if lo - 0x40 <= v < hi + 0x40: hits.append('%s word @%08X = %08X' % (name, base + i, v))
    for v, at in crefs:
        if lo - 0x40 <= v < hi + 0x40: hits.append('code %08X builds %08X' % (at, v))
    return hits

for (where, s, e) in G['ranges']:
    base, b = imgs[where]
    ram_s, ram_e = s, e
    if where == 'common': ram_s, ram_e = s - G['common_rom'] + 0x8024B000, e - G['common_rom'] + 0x8024B000
    else: ram_s, ram_e = 0x80154000 + s, 0x80154000 + e
    off = ram_s - base
    print('=== %s %s 0x%X-0x%X RAM %08X-%08X (%d B)' % (game, where, s, e, ram_s, ram_e, e - s))
    print(' before:'); hexdump(b[off - 0x40:off], ram_s - 0x40)
    print(' range:'); hexdump(b[off:off + min(e - s, 0x260)], ram_s)
    print(' after:'); hexdump(b[off + e - s:off + e - s + 0x40], ram_e)
    for h in search(ram_s, ram_e, where)[:40]: print('  ref?', h)
