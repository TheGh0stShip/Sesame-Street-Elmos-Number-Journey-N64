# gfx report

Tools in `tools/` (see tools/README.md). `tools/run_all.sh OUT` extracts and checks both games.
Check result: letter 4237 items, number 4666 items, **0 mismatches** (every PNG re-encodes to the exact texels and TLUT,
every DL re-assembles from its macros, every vertex buffer and animation re-packs exactly).
Library self-tests: f3dex 200k random words round trip; n64tex all 10 formats round trip through PNG files.

## Counts
| | letter | number |
|---|---|---|
| textures (unique fmt/size/palette) | 945: CI8 543, CI4 109, I4 278, RGBA32 14, RGBA16 1 | 789: CI8 519, CI4 157, I4 107, RGBA32 5, RGBA16 1 |
| display lists | 1814 (1501 roots) | 2184 (1873 roots) |
| vertices | 64621 | 54053 |
| model headers (actors) | 106 | 87 |
| OBJ exports (models + root level DLs) | 1566 | 1894 |
| vertex-animation files | 24 | 32 (3 unused) |

Bytes per class in MAP.tsv (ranges inside gzip images counted in inflated bytes):
letter: texture 1486176, palette 254368, display_list 306016, vertex 1033936, model hdr/bbox/vtxmap 44008, animation 1206496,
font 2140, level_data 836526, pad 312, unknown 132; containers 1287280 ROM bytes.
number: texture 1336232, palette 265632, display_list 295808, vertex 864848, model 24804, animation 1764680, font 2140,
level_data 1218872, pad 404, unknown 440; containers 1505600.

## Program image (main .data) graphics - my remaining territory per coordinator
letter: RGBA16 32x32 0x8006D518 (func_800030A0), CI8 32x32 0x8006DFCC (HUD), font atlas I4 64x128 0x800707A8 + charset
0x800717A8 + Bitmap[80] 0x800717FC + descriptor 0x80071CFC, debug font 0x80071E60, I4 32x256 0x80072160 (func_80012164),
vertex quad 0x80074520, I4 64x64 0x80074718 (func_8001A318), and the DLs in .data (D_800744F0 etc., listed in map).
number: same set at 0x8006F7E4, 0x80071828 (atlas), 0x80072828, 0x8007287C, 0x80072D7C, 0x80072EE0 (debug), 0x800731E0,
0x80074FD0, 0x800752F4, 0x80076800. Most HUD/menu texture *data* lives in `common` and level 19/11/22 images; the
.data tables only hold {palette, image} pointer pairs.
These overlap ranges data1/data2 typed as raw objects: their MAP should defer to these lines.

## Caveats / not done
- Number sources of twin functions carry Letter symbol names; I took the real addresses from the Number disassembly for
  func_80002EA8, func_800125D4, func_800186E8, func_8001AC1C (gen_tables.py NUMBER_FIX). Other twins may still hide.
- `level_data` = level-image bytes that no graphics path reaches (object tables, collision, scripts) or graphics I did
  not find: not identified. Level images 21 and 22 are 2D screens: only the background/HUD images the code names are found.
- DL starts come from a backwards scan from each G_ENDDL; a few leading commands could be data. Model headers by pattern.
- DL check re-assembles macros; 8 commands print as gsDPWord (bit patterns the macro cannot express).
- Animation file tails (4-0xB8 bytes) are non-zero leftovers after the last whole vertex: `unknown`.
- The coordinator handed the gzip level graphics (lvlB), the packs before them (packA-C) and the animation files (sprA) to other
  agents after this run had already covered them; my map lines for those can serve as a cross-check.

## Odd things
- Number: three animation files nobody loads: ROM 0x67C2F0-0x687B30, 0x739C20-0x75F380, 0x79CD90-0x7AB810.
- Letter func_8001D1BC: CI4 full-screen background drawer with no callers.
- Debug overlay func_80001F60 prints 4 pages of position/camera/timing values with the 8x8 debug font.
- Letter font atlas: one glyph's t+height reaches row 129 of a 128-row atlas (reads into the charset string).
