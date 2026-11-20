#!/usr/bin/env python3
"""callgraph.py ROM GAME FUNCTIONS_JSON SRCDIR OUT_TSV
Builds the call graph from the machine code itself: jal targets, lui/addiu (or ori) address loads of function starts
inside code, and 32-bit words in .data/.rodata equal to a function start (function pointers stored in data).
Each function is annotated with the first comment line of its matched source (SRCDIR/func_X.c) when present."""
import sys, json, struct, os, re
RAM2ROM = 0x7FFFF400
SEG = {'letter': (0x8006C9AC, 0x8007BAB0), 'number': (0x8006E31C, 0x8007BF20)}  # .data start .. .rodata end
def main(rom_p, game, fj, src, out):
    rom = open(rom_p, 'rb').read()
    fs = sorted((f for f in json.load(open(fj)) if f['game'] == game), key=lambda f: f['vram'])
    starts = {f['vram']: f for f in fs}
    jal, addr, data = ({v: set() for v in starts} for _ in range(3))
    owner = lambda v: next((g['vram'] for g in fs if g['vram'] <= v < g['vram'] + g['size']), None)
    for f in fs:
        hi = {}
        for o in range(f['rom'], f['rom'] + f['size'], 4):
            w = struct.unpack_from('>I', rom, o)[0]; op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31
            if op in (2, 3):  # jal, and j (tail call) leaving the function
                t = 0x80000000 | ((w & 0x3FFFFFF) << 2)
                if t in jal and t != f['vram']: jal[t].add(f['vram'])
            elif op == 15: hi[rt] = (w & 0xFFFF) << 16
            elif op in (9, 13) and rs in hi:
                imm = w & 0xFFFF
                t = (hi[rs] + (imm - 0x10000 if op == 9 and imm & 0x8000 else imm)) & 0xFFFFFFFF if op == 9 else hi[rs] | imm
                if t in addr and t != f['vram']: addr[t].add(f['vram'])
    a, b = SEG[game]
    for v in range(a & ~3, b, 4):
        w = struct.unpack_from('>I', rom, v - RAM2ROM)[0]
        if w in data: data[w].add(v)
    def desc(v):
        p = os.path.join(src, 'func_%08X.c' % v)
        if not os.path.exists(p): return ''
        for line in open(p, errors='replace'):
            m = re.match(r'\s*/\*\s*(.*?)\s*(\*/)?\s*$', line)
            if m and m.group(1) and not m.group(1).startswith('*'): return m.group(1).replace('\t', ' ')[:120]
        return ''
    with open(out, 'w') as o:
        o.write('func\tcls\tsize\tcallers_jal_or_j\taddr_taken_in_code\tptr_in_data\tunreferenced\tsource_comment\n')
        for v, f in starts.items():
            h = lambda s: ','.join('%08X' % x for x in sorted(s)) or '-'
            unref = not (jal[v] or addr[v] or data[v]) and v != fs[0]['vram']
            o.write(f"func_{v:08X}\t{f['cls']}\t{f['size']}\t{h(jal[v])}\t{h(addr[v])}\t{h(data[v])}\t{'YES' if unref else ''}\t{desc(v)}\n")
    print(game, len(starts), 'functions;', sum(1 for v in starts if not (jal[v] or addr[v] or data[v])), 'unreferenced')
if __name__ == '__main__':
    main(*sys.argv[1:6])
