#!/usr/bin/env python3
"""extract_ucode.py ROM OUTDIR [--check] : write the RSP microcode text/data sections embedded in the Elmo program image.
Locations come from the OSTask setup code (letter func_8000B470 / func_80050BC0 / func_80050DD8; number func_8000BA3C and
siblings); the build is told apart by where the F3DEX version string sits, which also validates the table."""
import sys, os, hashlib, json
RAM2ROM = 0x7FFFF400
# key: ROM offset of the F3DEX id string (gfx data + 0x2B0)
LAYOUT = {
 0x798B0: dict(game='letter', boot=(0x8006B390, 0x8006B460), gfx_text=(0x8006B460, 0x8006C890),
               aud_text=(0x8006C890, 0x8006D4E4), gfx_data=(0x80078A00, 0x80079200), aud_data=(0x80079200, 0x800794B0)),
 0x7A0C0: dict(game='number', boot=(0x8006CD00, 0x8006CDD0), gfx_text=(0x8006CDD0, 0x8006E200),
               aud_text=(0x8006E200, 0x8006EE54), gfx_data=(0x80079210, 0x80079A10), aud_data=(0x80079A10, 0x80079CC0)),
}
NAMES = dict(boot='rspboot.text.bin', gfx_text='gspF3DEX.text.bin', gfx_data='gspF3DEX.data.bin',
             aud_text='aspMain.text.bin', aud_data='aspMain.data.bin')
def main(rom_p, out, check=False):
    rom = open(rom_p, 'rb').read()
    key = rom.find(b'RSP Gfx ucode F3DEX', 0, 0x100000)
    if key not in LAYOUT: sys.exit('unrecognised build (F3DEX id string at 0x%X)' % key)
    lay = LAYOUT[key]; os.makedirs(out, exist_ok=True); man = {'game': lay['game']}
    for k, fn in NAMES.items():
        a, b = lay[k]; blob = rom[a - RAM2ROM:b - RAM2ROM]
        assert blob, k
        open(os.path.join(out, fn), 'wb').write(blob)
        man[k] = dict(file=fn, vram='0x%08X' % a, vram_end='0x%08X' % b, rom='0x%X' % (a - RAM2ROM),
                      rom_end='0x%X' % (b - RAM2ROM), size=b - a, sha1=hashlib.sha1(blob).hexdigest())
    # sanity: text tails are zero padding up to 16-byte alignment, data ends where game data starts
    for k in ('aud_text',):
        e = lay[k][1] - RAM2ROM
        man[k]['pad_after'] = '0x%X-0x%X zero' % (e, (e + 15) & ~15) if not any(rom[e:(e + 15) & ~15]) else 'NONZERO'
    json.dump(man, open(os.path.join(out, 'ucode.json'), 'w'), indent=1)
    for k in NAMES: print(f"{k:9s} {man[k]['rom']:>8s}-{man[k]['rom_end']:<8s} {man[k]['size']:6d} sha1 {man[k]['sha1']}")
    if check:
        bad = 0
        for k, fn in NAMES.items():
            a, b = lay[k]
            if open(os.path.join(out, fn), 'rb').read() != rom[a - RAM2ROM:b - RAM2ROM]: bad += 1; print('FAIL', k)
        print('check:', 'lossless (all sections re-read equal ROM bytes)' if not bad else 'FAILED'); return bad
    return 0
if __name__ == '__main__':
    a = [x for x in sys.argv[1:] if x != '--check']
    sys.exit(main(a[0], a[1], '--check' in sys.argv))
