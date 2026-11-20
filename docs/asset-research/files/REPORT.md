# files agent report

Done (both games): MAP.tsv tiles Letter 0x7C6B0..0x800000 (68 rows) and Number 0x7CB20..0x800000 (77 rows) with no gaps
(asserted by `tools/extract_files.py check`). FILES_letter.tsv / FILES_number.tsv list every loadable range with its loader.
`tools/extract_files.py ROM OUTDIR` carves all ranges and inflates the 16 gzip files; `check` verifies CRC32+ISIZE and tiling.

Bytes per class (Letter / Number):
- files (gzip .bif): 1,287,202 / 1,505,535 (inflate to 2.62 MB / 2.97 MB)
- gfx (raw screens + common prelinked block): 1,323,072 / 1,026,912
- audio_bank (sfx ptr/wave/fx, music ptr/wave): 1,957,936 / 1,872,128
- music (libmus songs): 233,088 / 235,344
- speech (MORT region): 1,090,976 / 1,268,992
- sprite_anim (whole-file loads at the end): 1,206,640 / 1,765,120
- pad (gzip alignment junk + zero tail): 780,078 / 203,825 (tail: Letter 0x741920.., Number 0x7CE410..)

Odd / unused:
- No file table: ranges are linker symbol pairs. The v8L number sources for func_80017520 carry Letter addresses
  (0x2FBD50...); real Number values from ROM: sfx ptr 0x2E6FC0, wbk 0x2E9A90, fx 0x3CB0C0..0x3CB880.
- Unreferenced: Letter songs 0x2F9B60..0x2FBD50, 0x4DD320..0x4E32C0, 0x50F9E0..0x510A10; Number sprite_anim files
  0x67C2F0..0x687B30, 0x739C20..0x75F380, 0x79CD90..0x7AB810.
- gzip members are followed by non-zero leftover bytes up to 16-byte alignment; gzip FNAMEs give original file names
  (streetab, chute, manor, orchard, garden, space, planet, bonusabc / street, dunk, bump, mansion, maze, ski, river, bonus123 .bif).
- The MORT clip region is byte-identical between the games over L 0x510A10..0x531664 = N 0x4E9810..0x50A464.
- Re-deflate is not byte-exact with zlib; losslessness rests on CRC32/ISIZE.

Not done: inner layout of .bif / common / screens (gfx), libmus internals (audio/music), MORT and speech clip
sub-boundaries inside the clip region (speech: use the .data tables).

## Corrections to the coordinator's FILES_<game>.tsv (v1 kept in out/FILES_*.coord_v1.tsv)
Boundaries now come from lui/addiu constants in the ROM's .text (tools/romrefs.py), not from source symbol names.
Three of the v1 split points are RAM addresses, not ROM addresses:
- 0x0A8D50 (both games) is the bss matrix D_800A8D50 (func_80012164 G_MTX): no split; one file 0xA54D0..0x10D0F0 (L) / 0xA5940..0x10D560 (N), loaded by mode 21.
- 0x2FD600 (Letter) is the heap start D_802FD600 (func_800409C8 size arithmetic): sfx ptr bank is one file 0x2FBD50..0x2FE8D0.
- 0x2B5120 (Number) is its heap start: river.bif is 0x27ADC0..0x2B8FD0 (v1 had a fake 16 KB "unknown").
Number audio used Letter addresses (sources share Letter names). Real Number layout: sfx ptr 0x2E6FC0..0x2E9A90, sfx wbk 0x2E9A90..0x3CB0C0,
fx bank 0x3CB0C0..0x3CB880, music ptr 0x3CB880..0x3D0980, music wbk 0x3D0980..0x4B00C0 (v1 rows 14-20 were split at 0x2FBD50/0x2FE8D0/0x3E3FB0/0x3E4830, none referenced by Number code).
Letter 0x50F9E0..0x61AFB0 is not one song: unused song 0x50F9E0..0x510A10, then the MORT speech region 0x510A10..0x61AFB0 (magic at 0x510A10).
Number row 40 (0x4E9810..0x61F510) is the MORT speech region.
Added: column loaded_by; fx banks classed libmus_fx_bank (L 0x3E3FB0, N 0x3CB0C0).
Not covered by any owner: the unused songs (songs agent can take them), gzip trailing junk and the zero tails (pad, in MAP.tsv).
