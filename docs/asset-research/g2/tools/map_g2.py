#!/usr/bin/env python3
"""Build ../MAP.tsv from the outputs of the g2 tools (no ROM access).  usage: map_g2.py G2_DIR AUDIO_OUT UCODE_OUT"""
import sys, os, json
g2, ao, uo = sys.argv[1:4]
L = []
def row(*f): L.append('\t'.join(str(x) for x in f))
for game in ('letter', 'number'):
    row(game, '0x000000', '0x000040', 'rom_metadata', 'N64 ROM header, 13 named fields (header.json); CRC1/CRC2 computed (CIC 6102)',
        'cartridge header: PI timing, clock, entry 0x80000400, libultra 2.0I, name, id, region', 'read by PIF/IPL3', 'boot_g2.py -> out/boot/%s/header.json' % game)
    row(game, '0x000040', '0x001000', 'boot', 'VR4300 assembly (GNU as/ld/objcopy): 716 instructions + 0x490-byte data table',
        'IPL3 CIC-NUS-6102, runs from SP DMEM 0xA4000040', 'PIF boot ROM', 'boot_g2.py -> out/boot/%s/ipl3.s' % game)
    u = json.load(open(os.path.join(uo, 'ucode_%s' % game, 'ucode.json')))
    for k, nm, d in (('boot', 'rspboot', 'RSP boot stub'), ('gfx_text', 'gspF3DEX.text', 'gspF3DEX 1.23 IMEM text'),
                     ('aud_text', 'aspMain.text', 'audio microcode IMEM text'), ('gfx_data', 'gspF3DEX.data', 'gspF3DEX 1.23 DMEM data'),
                     ('aud_data', 'aspMain.data', 'audio microcode DMEM data')):
        row(game, '0x%06X' % int(u[k]['rom'], 16), '0x%06X' % int(u[k]['rom_end'], 16), 'rsp_ucode',
            'RSP assembly (rsp_g2.py table-driven assembler)' if 'text' in nm or k == 'boot' else 'RSP DMEM data as .word/.ascii lines',
            d, 'OSTask (gfx/audio task builders)', 'rsp_g2.py -> out/rsp/%s/%s.s' % (game, nm))
    H = json.load(open(os.path.join(g2, 'out', 'adpcm', game, 'headers.json')))['samples']
    for key, hd in sorted(H.items()):
        s = json.load(open(os.path.join(ao, game, key + '.json')))
        row(game, '0x%06X' % s['rom'], '0x%06X' % (s['rom'] + 9 * s['frames']), 'audio',
            'VADPCM: frames from WAV + header rule (SDK vadpcm_enc choice, else first exact) + %d explicit header fields' % len(hd),
            'sample %s (%d frames)' % (key, s['frames']), 'libmus wave table entry', 'adpcm_g2.py -> out/adpcm/%s/headers.json + audio wav' % game)
    for it in json.load(open(os.path.join(g2, 'out', 'tex', game, 'index.json')))['items']:
        row(game, '0x%06X' % int(it['rom_start'], 16), '0x%06X' % int(it['rom_end'], 16), it['cls'],
            '%s%d %dx%d PNG' % (it['fmt'], it['bits'], it['w'], it['h']) + (' (TLUT strip)' if it['cls'] == 'tlut' else ''),
            'common pack RAM %s; size from %s' % (it['ram'], it['size_from']) + ('; palette %s' % it['tlut_ram'] if it.get('tlut_ram') else ''),
            '; '.join(it['ref'])[:200], 'tex_g2.py -> out/tex/%s/%s.png' % (game, it['name']))
    for l in list(open(os.path.join(g2, 'out', 'leftover', 'LEFTOVER_%s.tsv' % game)))[1:]:
        f = l.rstrip('\n').split('\t')
        row(game, f[1], f[2], 'leftover', 'copy of ROM bytes 0x2000 earlier (%s); %s non-zero' % (f[7], f[4]),
            'file-tail pad to 16 after %s: stale 8 KiB write buffer' % f[8], 'nothing (padding)', 'leftover_g2.py -> out/leftover/LEFTOVER_%s.tsv' % game)
hdr = 'game\trom_start\trom_end\tclass\tformat\tdescription\treference\textractor'
open(os.path.join(g2, 'MAP.tsv'), 'w').write(hdr + '\n' + '\n'.join(L) + '\n')
print('MAP rows', len(L))
