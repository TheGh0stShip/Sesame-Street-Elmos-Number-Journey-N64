import sys, os, pickle
sys.argv = [sys.argv[0]] + sys.argv[1:]
import rebuild_rom as R
from n64dk.elf import Elf
from n64dk.match import compare as cmp
from n64dk.match.symbols import SymbolMap
game = sys.argv[1]; v = int(sys.argv[2], 16)
rom = open(R.ROMS[game], 'rb').read()
F = {f['vram']: f for f in R.json.load(open(R.L + '/sestools/functions.json')) if f['game'] == game and f['rom'] is not None}
f = F[v]; o = R.S + '/r4/out/%s/code/obj/func_%08X.o' % (game, v)
elf = Elf.load(o)
for s in elf.sections: print(s.name, len(s.data))
syms = SymbolMap()
tmp = {'.text': v}
for k, s in enumerate(('.rodata', '.rdata', '.lit4', '.lit8', '.data', '.sdata')): tmp[s] = 0x81000000 + k * 0x100000
for s in ('.rodata', '.data', '.lit8', '.lit4'):
    if elf.section(s) is not None and len(elf.section(s).data):
        b = cmp.relocate(elf, s, tmp, syms); print(s, b.hex())
        p = rom.find(b.rstrip(b'\0')[:16]); print(' rom find first16 at', hex(p))
for r in R.__dict__.get('x', []): pass
from n64dk.match.compare import relocations
for r in relocations(elf, '.text'): print(r)
import struct
names = {}
for l in open(R.L + '/sesagents/kit/%s/symbols/sdk_symbols.txt' % game):
    m = R.re.match(r'\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)', l)
    if m: names[m.group(1)] = int(m.group(2), 16)
syms.update(names)
rs = [r for r in relocations(elf, '.text') if r.section and r.section != '.text']
print('section relocs', [(r.offset, r.type, r.section, r.addend) for r in rs][:20])
for c in sys.argv[3:]:
    b = cmp.relocate(elf, '.text', {'.text': v, '.rodata': int(c, 16)}, syms)
    t = rom[f['rom']:f['rom'] + len(b)]
    d = [(k, b[k:k+4].hex(), t[k:k+4].hex()) for k in range(0, len(b), 4) if b[k:k+4] != t[k:k+4]]
    print(c, len(d), d[:12])
if len(sys.argv) > 3:
    a = int(sys.argv[3], 16) - 0x7FFFF400; print('rom at cand', rom[a:a+96].hex())
