"""seslib.py: shared helpers for the r5 catalogue tools (no ROM content in here).
Everything is computed from the ROM file passed on the command line and from functions.json."""
import json, struct, zlib, csv

RAM2ROM = 0x7FFFF400
# program image sections (from ASSET_FACTS.md): .data start, .rodata end
SEG = {'letter': {'data': (0x8006C9AC, 0x800785B0), 'rodata': (0x800785B0, 0x8007BAB0), 'text_end': 0x8006C9AC,
                  'bss_end': 0x80140FA0},
       'number': {'data': (0x8006E31C, 0x80078DC0), 'rodata': (0x80078DC0, 0x8007BF20), 'text_end': 0x8006E31C,
                  'bss_end': 0x80140F60}}
ENTRY = 0x80000400
LOADS = set(range(0x20, 0x28)) | {0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F} | set(range(0x28, 0x30))


def game_of(rom_path):
    return 'number' if 'Number' in rom_path else 'letter'


def load_functions(fj, game):
    fs = sorted((f for f in json.load(open(fj)) if f['game'] == game), key=lambda f: f['vram'])
    return fs


def words(rom, a, b):
    for o in range(a, b, 4):
        yield o, struct.unpack_from('>I', rom, o)[0]


def scan_code(rom, fs):
    """Per function: jal/j targets, branch targets leaving the function, and every %hi/%lo address
    built in a register (lui + addiu/ori/load/store). Returns dict vram -> dict(calls=set, addrs=set(addr, insn_vram))."""
    out = {}
    for f in fs:
        v0, n = f['vram'], f['size']
        calls, addrs = set(), set()
        hi = {}
        for o, w in words(rom, f['rom'], f['rom'] + n):
            pc = v0 + (o - f['rom'])
            op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; rd = (w >> 11) & 31; imm = w & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            dest = None
            if op in (2, 3):
                calls.add(((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2))
                if op == 3: dest = 31
            elif op in (4, 5, 6, 7, 20, 21, 22, 23) or (op == 1 and rt in (0, 1, 2, 3, 16, 17, 18, 19)):
                t = (pc + 4 + (simm << 2)) & 0xFFFFFFFF
                if not (v0 <= t < v0 + n):
                    calls.add(t)
            elif op == 15:
                hi[rt] = imm << 16
                continue
            elif op == 9 and rs in hi:
                addrs.add(((hi[rs] + simm) & 0xFFFFFFFF, pc)); dest = rt
            elif op == 13 and rs in hi:
                addrs.add((hi[rs] | imm, pc)); dest = rt
            elif op in LOADS and rs in hi:
                addrs.add(((hi[rs] + simm) & 0xFFFFFFFF, pc))
                if op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37): dest = rt
            elif op in (8, 9, 10, 11, 12, 13, 14) or op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37):
                dest = rt
            elif op == 0 and (w & 63) in (0x20, 0x21, 0x2D) and (rs in hi) != (rt in hi):
                hi[rd] = hi[rs] if rs in hi else hi[rt]        # base + index: keep the %hi for the %lo use
                continue
            elif op == 0:
                dest = rd
            if dest is not None:
                hi.pop(dest, None)
        out[v0] = {'calls': calls, 'addrs': addrs}
    return out


def data_words(rom, game):
    a, b = SEG[game]['data'][0], SEG[game]['rodata'][1]
    for v in range(a & ~3, b, 4):
        yield v, struct.unpack_from('>I', rom, v - RAM2ROM)[0]


def owner_finder(fs):
    import bisect
    starts = [f['vram'] for f in fs]
    def own(v):
        i = bisect.bisect_right(starts, v) - 1
        if i >= 0 and v < fs[i]['vram'] + fs[i]['size']:
            return fs[i]['vram']
        return None
    return own


def files_table(path):
    rows = []
    with open(path) as fh:
        for r in csv.DictReader(fh, delimiter='\t'):
            rows.append((int(r['rom_start'], 16), int(r['rom_end'], 16), r['class_guess'], r.get('note', '')))
    return rows


def gunzip_member(rom, a, b):
    d = zlib.decompressobj(31)
    return d.decompress(rom[a:b])


def gzip_name(rom, a):
    if rom[a:a + 3] != b'\x1f\x8b\x08' or not rom[a + 3] & 8:
        return None
    e = rom.index(b'\0', a + 10)
    return rom[a + 10:e].decode('latin-1')
