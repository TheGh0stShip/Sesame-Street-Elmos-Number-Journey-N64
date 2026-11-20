# menu21: mode-21 screen pack (Letter ROM 0x0A54D0-0x10D0F0, Number ROM 0x0A5940-0x10D560)

Both games: one raw file of 0x67C20 (424992) bytes, no header, no compression. The level loader
(Letter func_8001D9D0 / Number func_8001E2CC, case 21) PI-copies it unchanged to RAM 0x80154000
(Letter func_80017318 / Number func_80017788). There are no pointers inside it; the code addresses the
pieces by fixed RAM addresses (file offset = RAM - 0x80154000).

Layout (same offsets in both games; the pieces tile the file exactly):

| file off | RAM        | piece                                   | size    |
|----------|------------|-----------------------------------------|---------|
| 0x00000  | 0x80154000 | TLUT, 256 x RGBA16                      | 0x200   |
| 0x00200  | 0x80154200 | CI8 image 320x240                       | 0x12C00 |
| 0x12E00  | 0x80166E00 | TLUT 256 + CI8 320x240                  | 0x12E00 |
| 0x25C00  | 0x80179C00 | TLUT 256 + CI8 320x240                  | 0x12E00 |
| 0x38A00  | 0x8018CA00 | TLUT 256 + CI8 320x240                  | 0x12E00 |
| 0x4B800  | 0x8019F800 | TLUT, 16 x RGBA16, then CI4 320x240     | 0x20 + 0x9600 |
| 0x54E20  | 0x801A8E20 | TLUT 256 + CI8 320x240                  | 0x12E00 |

Encodings: RGBA16 = big-endian 5551 (R bits 15-11, G 10-6, B 5-1, alpha bit 0). CI8 = one palette index per
byte, row-major, 320 bytes per row. CI4 = two pixels per byte, high nibble first.

## How it is drawn
Per frame, when the game mode (Letter D_8012EFD0 / Number D_8012F154) is 21 (0x15), the frame builder
(Letter func_8000B470 / Number func_8000BA3C) picks a slot by a screen-state variable and calls the
full-screen blitter (Letter func_8001CAA8(img, tlut) / Number func_8001D3AC): it loads the 256-entry TLUT
(G_SETTIMG RGBA16 + G_LOADTLUT 256), sets TLUT mode RGBA16, copy cycle, then draws the 320x240 screen as
10x5 tiles of 32x48 texels (G_SETTIMG CI 8b width 320, LOADTILE, TEXRECT). It is CI8 only, so the CI4 slot
at 0x4B800 cannot be drawn by it; no code references 0x8019F800.

State -> slot: Letter D_800745E4: 0->0x38A00, 1->0x12E00, 2->0x25C00, 3->0x54E20, 4/default->0x00000 (state 4
also prints a menu string over it). Number D_80075094: 0->0x25C00, 1->0x00000, 2->0x12E00, 3->0x54E20,
4/default->0x38A00 (state 4 prints text).

## Extraction
`tools/extract_packB.py ROM {letter|number} OUTDIR [extract|check|map]`: one indexed PNG per slot (PLTE =
5-bit channels expanded (v<<3|v>>2), tRNS = alpha bit as 0/255). `check` reads the PNGs back, re-packs
(channel>>3, alpha = tRNS!=0) and compares with the ROM bytes. Pure Python, zlib only.
