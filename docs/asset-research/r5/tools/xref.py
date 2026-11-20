#!/usr/bin/env python3
"""xref.py ROM FUNCTIONS_JSON FILESDIR LO [HI] [--words]

Every place that refers to RAM addresses in [LO, HI] (HI defaults to LO):
  code: %hi/%lo address builds (lui + addiu/ori/load/store), with the instruction kind (addr/load/store);
  data: 32-bit words in .data/.rodata whose value is in range;
  assets (--words): 32-bit words inside the loaded asset images (inflated level images, raw packs).
Also accepts ROM offsets: give LO with a 'rom:' prefix (e.g. rom:0x4DD320) to search for the pair of
lui/addiu halves building that 24-bit ROM offset (how the game names asset files)."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from seslib import *


def main():
    a = sys.argv[1:]
    rom_p, fj, fd = a[0], a[1], a[2]
    flags = [x for x in a[3:] if x.startswith('--')]
    pos = [x for x in a[3:] if not x.startswith('--')]
    romkind = pos[0].startswith('rom:')
    lo = int(pos[0].replace('rom:', ''), 16)
    hi = int(pos[1].replace('rom:', ''), 16) if len(pos) > 1 else lo
    game = game_of(rom_p)
    rom = open(rom_p, 'rb').read()
    fs = load_functions(fj, game)
    for f in fs:
        hiregs = {}
        for o, w in words(rom, f['rom'], f['rom'] + f['size']):
            pc = f['vram'] + o - f['rom']
            op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; imm = w & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            if op == 15:
                hiregs[rt] = imm << 16; continue
            t = None
            if op == 9 and rs in hiregs: t = (hiregs[rs] + simm) & 0xFFFFFFFF; kind = 'addr'
            elif op == 13 and rs in hiregs: t = hiregs[rs] | imm; kind = 'addr'
            elif op in LOADS and rs in hiregs:
                t = (hiregs[rs] + simm) & 0xFFFFFFFF; kind = 'store' if op >= 0x28 and op not in (0x31, 0x35, 0x37) else 'load'
            if t is not None and lo <= t <= hi:
                print('code\t%08X\tfunc_%08X\t%s\t%08X' % (pc, f['vram'], kind, t))
            if op in (8, 9, 10, 11, 12, 13, 14) or 0x20 <= op <= 0x27:
                if not (op in (9, 13) and rt == rs): hiregs.pop(rt, None)
                elif t is not None: hiregs.pop(rt, None)
    if not romkind:
        for v, w in data_words(rom, game):
            if lo <= w <= hi: print('data\t%08X\tword\t-\t%08X' % (v, w))
    if '--words' in flags:
        for st, en, cls, note in files_table(os.path.join(fd, 'FILES_%s.tsv' % game)):
            img = gunzip_member(rom, st, en) if cls == 'gzip_level' else rom[st:en] if cls == 'gfx_pack' else None
            if img is None: continue
            for o in range(0, len(img) - 3, 4):
                w = int.from_bytes(img[o:o + 4], 'big')
                if lo <= w <= hi: print('asset\tfile@%06X+%X\tword\t-\t%08X' % (st, o, w))


if __name__ == '__main__':
    main()
