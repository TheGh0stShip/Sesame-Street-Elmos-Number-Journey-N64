#!/usr/bin/env python3
"""Carve every ROM range the Elmo games load (and the rest of the post-program ROM), inflate gzip files.

usage:
  extract_files.py ROM OUTDIR        carve + inflate into OUTDIR/<game>/..., write OUTDIR/<game>/files.tsv
  extract_files.py check ROM         verify gzip CRC32/ISIZE, re-deflate where byte-exact, verify tiling
  extract_files.py map ROM           print MAP rows (tsv) for this ROM
  extract_files.py files ROM         print FILES rows (tsv) for this ROM

Contains no ROM bytes: only addresses taken from the game code (lui/addiu constants, listed per range).
"""
import sys, os, zlib, struct, hashlib

# ---------------------------------------------------------------------------------------------
# Range tables.  (start, end, class, format, name, description, referenced-by)
# Boundaries are the D_00xxxxxx ROM symbols the code uses (start/end pairs); see FORMAT.md.
# ---------------------------------------------------------------------------------------------
LETTER = dict(
    name='letter', program_end=0x7C6B0, rom_end=0x800000,
    ranges=[
        (0x07C6B0, 0x0A54D0, 'gfx', 'raw_screen_images', 'menu22', 'mode 22 screen images: raw, PI-copied to 0x80154000 (0x200-byte TLUT followed by image, drawn by func_8001CAA8; 0x12E00-byte slots at 0x80154000/0x80166E00/0x80179C00/0x8018CA00)', 'func_8001D9D0 case 22 -> func_80017318'),
        (0x0A54D0, 0x10D0F0, 'gfx', 'raw_screen_images', 'menu21', 'mode 21 screen images: raw, PI-copied to 0x80154000', 'func_8001D9D0 case 21 -> func_80017318'),
        (0x10D0F0, 0x1BF6F0, 'gfx', 'raw_prelinked', 'common', 'shared world data loaded after every level: raw, PI-copied to 0x8024B000', 'func_8001D9D0 (all cases) -> func_80017318'),
        (0x1BF6F0, 0x204D00, 'files', 'gzip', 'streetab.bif', 'level 2 (Sesame Street hub) world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 2 -> func_80013560 (inflate to 0x80154000)'),
        (0x204D00, 0x227BA0, 'files', 'gzip', 'chute.bif', 'level 16 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 16 -> func_80013560'),
        (0x227BA0, 0x243E20, 'files', 'gzip', 'manor.bif', 'level 15 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 15 -> func_80013560'),
        (0x243E20, 0x2604B0, 'files', 'gzip', 'orchard.bif', 'level 18 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 18 -> func_80013560'),
        (0x2604B0, 0x28FDB0, 'files', 'gzip', 'garden.bif', 'level 17 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 17 -> func_80013560'),
        (0x28FDB0, 0x2ACFA0, 'files', 'gzip', 'space.bif', 'level 14 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 14 -> func_80013560'),
        (0x2ACFA0, 0x2D1FB0, 'files', 'gzip', 'planet.bif', 'level 13 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 13 -> func_80013560'),
        (0x2D1FB0, 0x2F9B60, 'files', 'gzip', 'bonusabc.bif', 'level 19 (bonus) world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001D9D0 case 19 -> func_80013560'),
        (0x2F9B60, 0x2FBD50, 'music', 'libmus_song', 'song_unref0', 'libmus song (header 0x215), not referenced by any code', 'none (unused)'),
        (0x2FBD50, 0x2FE8D0, 'audio_bank', 'libmus_ptrbank', 'sfx.ptr', 'sound-effect pointer bank "N64 PtrTablesV2"', 'func_800170B0 -> func_80017318 to 0x800B0CA0; musConfig.ptr'),
        (0x2FE8D0, 0x3E3FB0, 'audio_bank', 'libmus_wavebank', 'sfx.wbk', 'sound-effect wave bank "N64 WaveTables", ROM-resident (DMA on demand)', 'func_800170B0 musConfig.wbk = D_002FE8D0'),
        (0x3E3FB0, 0x3E4830, 'audio_bank', 'libmus_fxbank', 'sfx.fx', 'libmus default effect bank', 'func_800170B0 -> func_80017318 to 0x80115390; musConfig.default_fxbank'),
        (0x3E4830, 0x3EA160, 'audio_bank', 'libmus_ptrbank', 'music.ptr', 'music instrument pointer bank', 'func_8001892C -> func_80017318 (heap); MusPtrBankInitialize'),
        (0x3EA160, 0x4D9D80, 'audio_bank', 'libmus_wavebank', 'music.wbk', 'music wave bank, ROM-resident', 'func_8001892C (wbk = D_003EA160)'),
        (0x4D9D80, 0x4DB3E0, 'music', 'libmus_song', 'song_4D9D80', 'song', 'func_80018A60 / func_8003AB7C -> func_8001892C'),
        (0x4DB3E0, 0x4DD320, 'music', 'libmus_song', 'song_4DB3E0', 'song', 'func_80018A60 -> func_8001892C'),
        (0x4DD320, 0x4E32C0, 'music', 'libmus_song', 'song_4DD320', 'song (only its end is referenced: unused)', 'none (unused)'),
        (0x4E32C0, 0x4EA530, 'music', 'libmus_song', 'song_4E32C0', 'song', 'func_80018A60 case 19'),
        (0x4EA530, 0x4EADF0, 'music', 'libmus_song', 'song_4EA530', 'song (jingle)', 'func_8003AB7C'),
        (0x4EADF0, 0x4F1850, 'music', 'libmus_song', 'song_4EADF0', 'song', 'func_80018A60 case 18'),
        (0x4F1850, 0x4F2030, 'music', 'libmus_song', 'song_4F1850', 'song (jingle)', 'func_8003A650 case 18'),
        (0x4F2030, 0x4FA460, 'music', 'libmus_song', 'song_4F2030', 'song', 'func_80018A60 case 17'),
        (0x4FA460, 0x4FAC20, 'music', 'libmus_song', 'song_4FA460', 'song (jingle)', 'func_8003A650 case 17'),
        (0x4FAC20, 0x501130, 'music', 'libmus_song', 'song_4FAC20', 'song', 'func_80018A60 case 15'),
        (0x501130, 0x501820, 'music', 'libmus_song', 'song_501130', 'song (jingle)', 'func_8003A650 case 15'),
        (0x501820, 0x507070, 'music', 'libmus_song', 'song_501820', 'song', 'func_80018A60 case 16'),
        (0x507070, 0x507890, 'music', 'libmus_song', 'song_507070', 'song (jingle)', 'func_8003A650 case 16'),
        (0x507890, 0x50C970, 'music', 'libmus_song', 'song_507890', 'song', 'func_80018A60 cases 13/14'),
        (0x50C970, 0x50D000, 'music', 'libmus_song', 'song_50C970', 'song (jingle)', 'func_8003A650 cases 13/14'),
        (0x50D000, 0x50F9E0, 'music', 'libmus_song', 'song_50D000', 'song', 'func_8003AB7C'),
        (0x50F9E0, 0x510A10, 'music', 'libmus_song', 'song_unref1', 'libmus song (header 0x215), not referenced (only its start is an end symbol)', 'none (unused)'),
        (0x510A10, 0x61AFB0, 'speech', 'mort_clips', 'clips', '"MORT" voice/sound clip region played by the sequencer thread func_80012FA0 -> func_80051314 -> func_80069440; clip table {u32 rom, s16 ms} in .data (e.g. 0x800733C8 = ROM 0x73FC8)', '.data clip tables (func_8002AB50 etc.)'),
        (0x61AFB0, 0x741920, 'sprite_anim', 'sprite_anim', 'speech', 'speech clips loaded whole into RAM by thread func_80026920 (D_80106874 = rom, D_800A75E0 = size)', 'func_800272C8 / func_80026B70'),
    ])

NUMBER = dict(
    name='number', program_end=0x7CB20, rom_end=0x800000,
    ranges=[
        (0x07CB20, 0x0A5940, 'gfx', 'raw_screen_images', 'menu22', 'mode 22 screen images: raw, PI-copied to 0x80154000', 'func_8001E2CC case 22 -> func_80017788'),
        (0x0A5940, 0x10D560, 'gfx', 'raw_screen_images', 'menu21', 'mode 21 screen images: raw, PI-copied to 0x80154000', 'func_8001E2CC case 21 -> func_80017788'),
        (0x10D560, 0x177680, 'gfx', 'raw_prelinked', 'common', 'shared world data: raw, PI-copied to 0x8024B000', 'func_8001E2CC (all cases) -> func_80017788'),
        (0x177680, 0x1BCE90, 'files', 'gzip', 'street.bif', 'level 1 (hub) world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 1 -> func_800139D0 (inflate)'),
        (0x1BCE90, 0x1D6CA0, 'files', 'gzip', 'dunk.bif', 'level 9 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 9'),
        (0x1D6CA0, 0x206CC0, 'files', 'gzip', 'bump.bif', 'level 10 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 10'),
        (0x206CC0, 0x22EC10, 'files', 'gzip', 'mansion.bif', 'level 8 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 8'),
        (0x22EC10, 0x25AF80, 'files', 'gzip', 'maze.bif', 'level 7 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 7'),
        (0x25AF80, 0x27ADC0, 'files', 'gzip', 'ski.bif', 'level 6 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 6'),
        (0x27ADC0, 0x2B8FD0, 'files', 'gzip', 'river.bif', 'level 5 world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 5'),
        (0x2B8FD0, 0x2E6FC0, 'files', 'gzip', 'bonus123.bif', 'level 11 (bonus) world file (prelinked RAM image for 0x80154000: RGBA16/CI textures, vertices, F3DEX display lists with absolute 0x8015xxxx pointers, level tables)', 'func_8001E2CC case 11'),
        (0x2E6FC0, 0x2E9A90, 'audio_bank', 'libmus_ptrbank', 'sfx.ptr', 'sound-effect pointer bank', 'func_80017520 -> func_80017788 to 0x800B10A0; musConfig.ptr'),
        (0x2E9A90, 0x3CB0C0, 'audio_bank', 'libmus_wavebank', 'sfx.wbk', 'sound-effect wave bank, ROM-resident', 'func_80017520 musConfig.wbk'),
        (0x3CB0C0, 0x3CB880, 'audio_bank', 'libmus_fxbank', 'sfx.fx', 'libmus default effect bank', 'func_80017520 -> func_80017788 to 0x80115530'),
        (0x3CB880, 0x3D0980, 'audio_bank', 'libmus_ptrbank', 'music.ptr', 'music pointer bank', 'func_8001933C'),
        (0x3D0980, 0x4B00C0, 'audio_bank', 'libmus_wavebank', 'music.wbk', 'music wave bank, ROM-resident', 'func_8001933C (wbk = D_003D0980)'),
    ] + [(a, b, 'music', 'libmus_song', 'song_%06X' % a, 'song' + ('' if r else ' (unused: start never referenced)'), r or 'none (unused)') for a, b, r in [
        (0x4B00C0, 0x4B8E70, 'func_80019470'), (0x4B8E70, 0x4BA590, 'func_80019470 / func_8003C494'),
        (0x4BA590, 0x4BC400, 'func_80019470'), (0x4BC400, 0x4BCB50, 'func_8003BF64'),
        (0x4BCB50, 0x4C12F0, 'func_80019470'), (0x4C12F0, 0x4C1970, 'func_8003BF64'),
        (0x4C1970, 0x4C6350, 'func_80019470'), (0x4C6350, 0x4C69A0, 'func_8003BF64'),
        (0x4C69A0, 0x4CB8D0, 'func_80019470'), (0x4CB8D0, 0x4CBCD0, 'func_8003BF64'),
        (0x4CBCD0, 0x4CFEC0, 'func_80019470'), (0x4CFEC0, 0x4D0690, 'func_8003BF64'),
        (0x4D0690, 0x4D7C90, 'func_80019470'), (0x4D7C90, 0x4D83F0, 'func_8003BF64'),
        (0x4D83F0, 0x4DF7C0, 'func_80019470'), (0x4DF7C0, 0x4E0870, 'func_800287AC'),
        (0x4E0870, 0x4E1090, 'func_8003C494'), (0x4E1090, 0x4E7620, 'func_80019470'),
        (0x4E7620, 0x4E9810, 'func_8003C494')]] + [
        (0x4E9810, 0x61F510, 'speech', 'mort_clips', 'clips', '"MORT" voice/sound clip region played by func_80012FA0 -> func_80051314; clip table {u32 rom, s16 ms} in .data (ROM 0x75700..0x75B30)', '.data clip tables'),
    ] + [(a, b, 'sprite_anim', 'sprite_anim', 'anim_%06X' % a, 'sprite animation file loaded whole into RAM by the streaming thread' + ('' if r else ' (unused: start never referenced)'), r or 'none (unused)') for a, b, r in [
        (0x61F510, 0x625430, 'func_800287AC'), (0x625430, 0x6289C0, 'func_800287AC'), (0x6289C0, 0x637780, 'func_800287AC'),
        (0x637780, 0x6482F0, 'func_80027FE0'), (0x6482F0, 0x6505B0, 'func_800287AC'), (0x6505B0, 0x6634C0, 'func_80027FE0'),
        (0x6634C0, 0x674C10, 'func_800287AC'), (0x674C10, 0x67C2F0, 'func_800287AC'), (0x67C2F0, 0x687B30, None),
        (0x687B30, 0x691BB0, 'func_800287AC'), (0x691BB0, 0x6A44D0, 'func_800287AC'), (0x6A44D0, 0x6A99B0, 'func_800287AC'),
        (0x6A99B0, 0x6B92E0, 'func_800287AC'), (0x6B92E0, 0x6C6700, 'func_800287AC'), (0x6C6700, 0x6D25E0, 'func_800287AC'),
        (0x6D25E0, 0x6E3450, 'func_80027FE0'), (0x6E3450, 0x6F8CF0, 'func_800287AC'), (0x6F8CF0, 0x701190, 'func_800287AC'),
        (0x701190, 0x712000, 'func_80027FE0'), (0x712000, 0x723E50, 'func_800287AC'), (0x723E50, 0x732CF0, 'func_800287AC'),
        (0x732CF0, 0x739C20, 'func_800287AC'), (0x739C20, 0x75F380, None), (0x75F380, 0x7710B0, 'func_800287AC'),
        (0x7710B0, 0x780C00, 'func_800287AC'), (0x780C00, 0x78E0B0, 'func_800287AC'), (0x78E0B0, 0x79CD90, 'func_80027FE0'),
        (0x79CD90, 0x7AB810, None), (0x7AB810, 0x7B35C0, 'func_800287AC'), (0x7B35C0, 0x7BB850, 'func_800287AC'),
        (0x7BB850, 0x7C57E0, 'func_80027FE0'), (0x7C57E0, 0x7CE410, 'func_800287AC')]])

# Letter speech clips: every boundary symbol is used as a start
_ls = [0x61AFB0, 0x620ED0, 0x624460, 0x633220, 0x644970, 0x657E70, 0x668FD0, 0x67BEE0, 0x68B260, 0x698AE0, 0x6A0760,
       0x6A2D50, 0x6B4150, 0x6C4A80, 0x6D5E80, 0x6E3190, 0x6F19C0, 0x6F9020, 0x70A5D0, 0x718E00, 0x71F230, 0x7307E0,
       0x73CBA0, 0x73F930, 0x741920]
LETTER['ranges'] = LETTER['ranges'][:-1] + [
    (a, b, 'sprite_anim', 'sprite_anim', 'anim_%06X' % a, 'sprite animation file loaded whole into RAM by thread func_80026920',
     'func_800272C8 / func_80026B70 (D_80106874/D_800A75E0)') for a, b in zip(_ls, _ls[1:])]

GAMES = {'letter': LETTER, 'number': NUMBER}


def gzip_parse(b):
    """return (header_len, deflate_end, name, payload) for a member at b[0]; raises on error."""
    if b[:3] != b'\x1f\x8b\x08': raise ValueError('no gzip magic')
    flg = b[3]; p = 10; name = b''
    if flg & 4: p += 2 + struct.unpack('<H', b[p:p + 2])[0]
    if flg & 8: e = b.index(b'\0', p); name = b[p:e]; p = e + 1
    if flg & 16: p = b.index(b'\0', p) + 1
    if flg & 2: p += 2
    d = zlib.decompressobj(-15)
    out = d.decompress(b[p:]); end = len(b) - len(d.unused_data)
    if not d.eof: raise ValueError('truncated deflate')
    crc, isz = struct.unpack('<II', b[end:end + 8])
    if (zlib.crc32(out) & 0xffffffff) != crc or isz != (len(out) & 0xffffffff):
        raise ValueError('crc/isize mismatch')
    return p, end, name.decode('latin-1'), out, flg, struct.unpack('<I', b[4:8])[0], b[8], b[9]


def classify(data):
    """content sniff for loaded files."""
    if data[:15] == b'N64 PtrTablesV2': return 'libmus pointer bank'
    if data[:15] == b'N64 WaveTables ': return 'libmus wave bank'
    if data[:4] == b'MORT': return 'MORT clip bank'
    if data[:4] == b'\x00\x00\x02\x15': return 'libmus song'
    return ''


def rows(rom):
    g = GAMES_BY(rom)
    out = []; cur = g['program_end']
    for (a, b, cls, fmt, name, desc, ref) in g['ranges']:
        assert a == cur, (hex(a), hex(cur)); cur = b
        if fmt == 'gzip':
            p, end, nm, data, *_ = gzip_parse(rom[a:b])
            out.append((a, a + end + 8, cls, 'gzip(deflate, FNAME "%s")' % nm, name, desc + '; inflates to %d bytes' % len(data), ref))
            if a + end + 8 < b:
                out.append((a + end + 8, b, 'pad', 'junk', name + '.pad',
                            'alignment fill to 16 bytes after the gzip trailer (non-zero leftover bytes; inside the loaded range, ignored by inflate)', ref))
        else:
            out.append((a, b, cls, fmt, name, desc, ref))
    # empty tail
    last = cur
    tail = rom[last:g['rom_end']]
    assert tail.count(0) == len(tail), 'tail not empty'
    out.append((last, g['rom_end'], 'pad', 'zero', 'tail', 'unused cartridge space, all 0x00', 'none'))
    return g, out


def GAMES_BY(rom):
    s = rom[0x20:0x34].decode('latin-1').upper()
    return NUMBER if ('NUMBER' in s or 'JOURNEY' in s) else LETTER


def main(argv):
    if len(argv) >= 2 and argv[0] in ('check', 'map', 'files'):
        mode, romp = argv[0], argv[1]; outdir = None
    elif len(argv) == 2:
        mode, romp, outdir = 'extract', argv[0], argv[1]
    else:
        print(__doc__); return 2
    rom = open(romp, 'rb').read()
    g, rs = rows(rom); gn = g['name']
    if mode == 'map':
        for (a, b, cls, fmt, name, desc, ref) in rs:
            print('\t'.join([gn, '0x%06X' % a, '0x%06X' % b, cls, fmt, '%s: %s' % (name, desc), ref,
                             'files/tools/extract_files.py' if cls in ('files', 'pad', 'gfx', 'music', 'speech', 'audio_bank') else '']))
        return 0
    if mode == 'files':
        for (a, b, cls, fmt, name, desc, ref) in rs:
            if cls == 'pad': continue
            ex = ''
            if fmt.startswith('gzip'):
                ex = 'out/%s/%s' % (gn, name)
            print('\t'.join([gn, name, '0x%06X' % a, '0x%06X' % b, str(b - a), cls, fmt, desc, ref, ex]))
        return 0
    if mode == 'check':
        bad = 0
        for (a, b, cls, fmt, name, desc, ref) in rs:
            if not fmt.startswith('gzip'): continue
            p, end, nm, data, flg, mt, xfl, os_ = gzip_parse(rom[a:b])
            ok = 'crc32+isize ok'
            # try byte-exact re-deflate with zlib levels/strategies
            orig = rom[a + p:a + end]; exact = None
            for lvl in (9, 6, 8, 7, 5, 4, 3, 2, 1):
                for mem in (8, 9):
                    c = zlib.compressobj(lvl, zlib.DEFLATED, -15, mem)
                    if c.compress(data) + c.flush() == orig: exact = (lvl, mem); break
                if exact: break
            print('%s %-14s 0x%06X %8d -> %8d  %s  redeflate=%s' % (gn, name, a, end - p, len(data), ok,
                  'byte-exact zlib level %d memLevel %d' % exact if exact else 'not byte-exact (stored data proven by CRC32)'))
        print('tiling ok: 0x%X..0x%X, %d ranges' % (g['program_end'], g['rom_end'], len(rs)))
        return bad
    # extract
    od = os.path.join(outdir, gn); os.makedirs(od, exist_ok=True)
    with open(os.path.join(od, 'files.tsv'), 'w') as f:
        f.write('name\trom_start\trom_end\tclass\tformat\tsha1\tinflated_size\n')
        for (a, b, cls, fmt, name, desc, ref) in rs:
            if cls == 'pad': continue
            raw = rom[a:b]; sub = os.path.join(od, cls); os.makedirs(sub, exist_ok=True)
            isz = ''
            if fmt.startswith('gzip'):
                p, end, nm, data, *_ = gzip_parse(raw)
                open(os.path.join(sub, name), 'wb').write(data); isz = str(len(data))
                open(os.path.join(sub, name + '.gz'), 'wb').write(raw)
            else:
                open(os.path.join(sub, name + '.bin'), 'wb').write(raw)
            f.write('%s\t0x%06X\t0x%06X\t%s\t%s\t%s\t%s\n' % (name, a, b, cls, fmt, hashlib.sha1(raw).hexdigest(), isz))
    print('extracted', len(rs), 'ranges to', od)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
