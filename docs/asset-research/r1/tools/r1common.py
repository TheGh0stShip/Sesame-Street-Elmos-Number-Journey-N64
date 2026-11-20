"""Shared tables for r1 tools: the formerly-unknown ranges and the RAM images they live in (no ROM content)."""
import struct, zlib

GAMES = {
    'letter': dict(
        text=(0x1050, 0x6D5AC, 0x80000450), data=(0x6D5AC, 0x7C6B0, 0x8006C9AC),
        common_rom=0x10D0F0, common_end=0x1BF6F0,
        levels={'streetab.bif': (0x1BF6F0, 0x204D00), 'chute.bif': (0x204D00, 0x227BA0),
                'manor.bif': (0x227BA0, 0x243E20), 'orchard.bif': (0x243E20, 0x2604B0),
                'garden.bif': (0x2604B0, 0x28FDB0), 'space.bif': (0x28FDB0, 0x2ACFA0),
                'planet.bif': (0x2ACFA0, 0x2D1FB0), 'bonusabc.bif': (0x2D1FB0, 0x2F9B60)},
        ranges=[('common', 0x14440C, 0x144410), ('common', 0x152044, 0x152050), ('common', 0x15C598, 0x15C5A0),
                ('common', 0x15FF58, 0x15FF60), ('common', 0x16035C, 0x160360), ('common', 0x196CE0, 0x196F00),
                ('common', 0x1BF3D0, 0x1BF5D0), ('common', 0x1BF6B0, 0x1BF6F0), ('manor.bif', 0x60B0, 0x6AB0)]),
    'number': dict(
        text=(0x1050, 0x6EF1C, 0x80000450), data=(0x6EF1C, 0x7CB20, 0x8006E31C),
        common_rom=0x10D560, common_end=0x177680,
        levels={'street.bif': (0x177680, 0x1BCE90), 'dunk.bif': (0x1BCE90, 0x1D6CA0),
                'bump.bif': (0x1D6CA0, 0x206CC0), 'mansion.bif': (0x206CC0, 0x22EC10),
                'maze.bif': (0x22EC10, 0x25AF80), 'ski.bif': (0x25AF80, 0x27ADC0),
                'river.bif': (0x27ADC0, 0x2B8FD0), 'bonus123.bif': (0x2B8FD0, 0x2E6FC0)},
        ranges=[('common', 0x12E3BC, 0x12E3C0), ('common', 0x1717D8, 0x1717FC), ('common', 0x171827, 0x1719DA),
                ('common', 0x171A02, 0x171A1C), ('common', 0x171A47, 0x171BEA), ('common', 0x171C22, 0x171C3C),
                ('common', 0x171CD7, 0x171DBA), ('common', 0x171E42, 0x171E5C), ('common', 0x171EF7, 0x171FF7),
                ('common', 0x172062, 0x172499),
                ('bump.bif', 0x262C0, 0x268C0), ('bump.bif', 0x30250, 0x30850), ('bump.bif', 0x3A1E0, 0x3A7E0),
                ('bump.bif', 0x44170, 0x44770), ('bump.bif', 0x4FB80, 0x4FDA0)]),
}

def inflate(rom, s, e):
    return zlib.decompressobj(31).decompress(rom[s:e])

def load_images(rom, game):
    G = GAMES[game]
    t0, t1, tr = G['text']; d0, d1, dr = G['data']
    imgs = {'text': (tr, rom[t0:t1]), 'data': (dr, rom[d0:d1]),
            'common': (0x8024B000, rom[G['common_rom']:G['common_end']])}
    for n, (s, e) in G['levels'].items():
        imgs[n] = (0x80154000, inflate(rom, s, e))
    return imgs

def code_refs(rom, game):
    """Every address built by lui rt,hi + (addiu|ori|load|store) imm(rt) within 32 instructions."""
    t0, t1, tr = GAMES[game]['text']
    w = [struct.unpack_from('>I', rom, i)[0] for i in range(t0, t1, 4)]
    out = []
    for i, x in enumerate(w):
        if x >> 26 != 0x0F: continue
        rt = (x >> 16) & 31; hi = (x & 0xFFFF) << 16
        for j in range(i + 1, min(i + 33, len(w))):
            y = w[j]; op = y >> 26; rs = (y >> 21) & 31; imm = y & 0xFFFF
            if rs == rt and op in (0x09, 0x0D, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B, 0x31, 0x39, 0x35, 0x3D):
                v = (hi | imm) if op == 0x0D else (hi + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
                out.append((v, tr + 4 * j))
            if y >> 26 == 0x0F and (y >> 16) & 31 == rt: break
    return out
