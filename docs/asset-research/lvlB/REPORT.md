# lvlB REPORT: graphics inside the gzip level files

Tool: `tools/extract_level_gfx.py ROM GAME OUTDIR` and `... check ROM GAME OUTDIR` (run with /usr/bin/python3: the lab venv has no PIL; the tool uses a dependency-free PNG module). Helpers f3dex.py / n64tex.py / png.py are a snapshot of agent gfx's tools (copied because they were being edited live). File ranges come from ../files/FILES_<game>.tsv, extra roots from ../LEVEL_REFS_<game>.tsv.

**check: OK for both games** - every PNG re-encodes to the exact texel and TLUT bytes, every DL text file reassembles to the exact bytes, 0 gsDPWord fallbacks.

MAP.tsv: 13087 rows tiling all 16 inflated files (offsets inside the inflated file).

| game | level | size | DL entries | PNG | vtx bufs | models | DL bytes | tex bytes | pal bytes | vtx bytes | model hdr | level_data | comment | common tex refs | clipped overlaps |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| letter | streetab.bif | 509070 | 311 | 94 | 201 | 1 | 28504 | 172544 | 47136 | 65760 | 36 | 195037 | 53 | 0 | 4 |
| letter | chute.bif | 259860 | 275 | 43 | 285 | 10 | 29472 | 58368 | 20992 | 97804 | 360 | 52757 | 107 | 23 | 10 |
| letter | manor.bif | 263356 | 235 | 32 | 314 | 9 | 32288 | 45056 | 15904 | 124784 | 324 | 44821 | 179 | 15 | 4 |
| letter | orchard.bif | 283384 | 380 | 56 | 352 | 6 | 39152 | 44088 | 17152 | 122320 | 216 | 60313 | 143 | 14 | 6 |
| letter | garden.bif | 362576 | 437 | 69 | 372 | 3 | 42800 | 117248 | 34848 | 121184 | 108 | 46317 | 71 | 17 | 11 |
| letter | space.bif | 245496 | 77 | 40 | 210 | 6 | 24840 | 52784 | 13280 | 93712 | 216 | 60521 | 143 | 36 | 0 |
| letter | planet.bif | 346524 | 191 | 36 | 370 | 9 | 32616 | 59904 | 9632 | 152844 | 324 | 91025 | 179 | 20 | 2 |
| letter | bonusabc.bif | 354072 | 107 | 18 | 89 | 1 | 10224 | 19456 | 9216 | 27136 | 36 | 287861 | 143 | 3 | 0 |
| number | street.bif | 509902 | 303 | 94 | 205 | 1 | 28904 | 171008 | 46688 | 67040 | 36 | 196173 | 53 | 0 | 2 |
| number | dunk.bif | 237750 | 268 | 43 | 273 | 5 | 30456 | 63496 | 21056 | 90416 | 180 | 32021 | 125 | 11 | 4 |
| number | bump.bif | 488874 | 194 | 144 | 289 | 7 | 39016 | 148992 | 66528 | 85552 | 252 | 148373 | 161 | 36 | 0 |
| number | mansion.bif | 305762 | 249 | 44 | 245 | 11 | 24040 | 58432 | 20608 | 73408 | 396 | 128645 | 233 | 10 | 1 |
| number | maze.bif | 376982 | 358 | 44 | 368 | 5 | 39624 | 61952 | 21536 | 133036 | 180 | 120529 | 125 | 2 | 5 |
| number | ski.bif | 275326 | 283 | 51 | 301 | 9 | 33816 | 30288 | 8832 | 109600 | 324 | 92269 | 197 | 21 | 6 |
| number | river.bif | 442490 | 284 | 64 | 285 | 6 | 29432 | 57424 | 20288 | 93728 | 216 | 241241 | 161 | 26 | 1 |
| number | bonus123.bif | 328682 | 140 | 29 | 207 | 6 | 21872 | 37432 | 10496 | 78944 | 216 | 179561 | 161 | 14 | 1 |

## Not done / odd
- `level_data` is large in streetab/street (~195 KB, tail of the file after 0x56254: 16-bit records, likely terrain/collision) and bonusabc (~288 KB from 0x10B50: records with floats and pointers) and Number river/bonus123: non-graphics, left to lvlA. Some unreferenced textures may hide there: a texture is only found when a traced DL loads it.
- Textures referenced in the `common` block (0x8024B000) are counted but not extracted (common is not my territory).
- "clipped overlaps": a few texture/TLUT size estimates overlap a neighbour (usually CI4 sub-palettes or LoadTile sub-rectangles); the MAP keeps the higher-priority object and marks the remainder.
- func_8000B470 refers to 0x80154200/0x80167000/0x80179E00/0x8018CC00 (0x12E00 stride): these are the menu-screen slots (menu files loaded at the same address), not level graphics.
- Every .bif ends with ELF `.comment` strings "GCC: (GNU) 2.7.2": the level images were built as C data with GCC 2.7.2 and objcopied.
- Number river.bif: with agent files's range (0x27ADC0-0x2B8FD0) it inflates to 442490 bytes (the coordinator's first table cut it at 0x2B5120).
