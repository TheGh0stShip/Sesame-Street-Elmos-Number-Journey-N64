"""Blob table for both games: the RAM images the gfx code addresses.
Level blobs are loaded at 0x80154000 by func_8001D9D0 (letter) / func_8001E2CC (number),
raw (func_80017318 DMA) or gzip (func_80013560 / func_800139D0 inflate, 9-byte header + FNAME skipped).
The common block is DMA'd raw to 0x8024B000 after every level load."""
import zlib, struct

LEVEL_BASE = 0x80154000
COMMON_BASE = 0x8024B000

# (level id, rom_start, rom_end, compressed)
LEVELS = {
 'letter': [(22,0x7C6B0,0xA54D0,False),(21,0xA54D0,0x10D0F0,False),
            (2,0x1BF6F0,0x204D00,True),(16,0x204D00,0x227BA0,True),(15,0x227BA0,0x243E20,True),
            (18,0x243E20,0x2604B0,True),(17,0x2604B0,0x28FDB0,True),(14,0x28FDB0,0x2ACFA0,True),
            (13,0x2ACFA0,0x2D1FB0,True),(19,0x2D1FB0,0x2F9B60,True)],
 'number': [(22,0x7CB20,0xA5940,False),(21,0xA5940,0x10D560,False),
            (1,0x177680,0x1BCE90,True),(9,0x1BCE90,0x1D6CA0,True),(10,0x1D6CA0,0x206CC0,True),
            (8,0x206CC0,0x22EC10,True),(7,0x22EC10,0x25AF80,True),(6,0x25AF80,0x27ADC0,True),
            (5,0x27ADC0,0x2B8FD0,True),(11,0x2B8FD0,0x2E6FC0,True)],
}
COMMON = {'letter': (0x10D0F0,0x1BF6F0), 'number': (0x10D560,0x177680)}
MAIN = {'letter': (0x1050, 0x7C6B0, 0x80000450), 'number': (0x1050, 0x7CB20, 0x80000450)}

def gunzip(b):
    """returns (data, consumed_len, header_len)"""
    assert b[:3] == b'\x1f\x8b\x08', 'not gzip'
    flg = b[3]; p = 10
    if flg & 4: p += 2 + struct.unpack('<H', b[p:p+2])[0]
    if flg & 8:
        while b[p]: p += 1
        p += 1
    if flg & 16:
        while b[p]: p += 1
        p += 1
    if flg & 2: p += 2
    d = zlib.decompressobj(-15)
    out = d.decompress(b[p:]) + d.flush()
    used = len(b) - len(d.unused_data)
    return out, used + 8, p, b[10:p].rstrip(b'\0').decode('latin1')

def game_of(rom):
    t = rom[0x20:0x34].decode('latin1')
    return 'number' if 'NUM' in t.upper() or rom[0x3B:0x3F] != rom[0x3B:0x3F] else ('letter' if 'LET' in t.upper() else t)

def images(rom, game):
    """yield (name, base_ram, bytes, rom_start, rom_end, compressed, extra)"""
    s, e, ram = MAIN[game]
    yield ('main', ram, rom[s:e], s, e, False, {})
    cs, ce = COMMON[game]
    yield ('common', COMMON_BASE, rom[cs:ce], cs, ce, False, {})
    for lid, s, e, c in LEVELS[game]:
        if c:
            data, used, hl, name = gunzip(rom[s:e])
            yield ('level%02d' % lid, LEVEL_BASE, data, s, e, True, {'gz_used': used, 'gz_header': hl, 'gz_name': name})
        else:
            yield ('level%02d' % lid, LEVEL_BASE, rom[s:e], s, e, False, {})
