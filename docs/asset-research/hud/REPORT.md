# hud report

Done for both games: every byte of the territory (packC's code-set texture ranges and the code-rooted TLUTs between
them) is in MAP.tsv; `tools/run.sh` extract + check is byte-exact (0 mismatches) for both.

| class | Letter count | Letter bytes | Number count | Number bytes |
|---|---|---|---|---|
| texture | 124 | 93,568 | 40 | 46,720 |
| tlut | 119 | 56,128 | 33 | 12,096 |
| morph_anim | 7 | 123,060 | 4 | 81,296 |
| dl (empty end-DL stubs) | 5 | 40 | 7 | 56 |
| pad | 7 | 52 | 8 | 64 |
| unknown | 0 | 0 | 0 | 0 |
| total | 262 | 272,848 | 92 | 140,232 |

Textures by kind: Letter: 52 big glyphs A-Z,a-z (CI8 32x32), 52 small glyphs (CI8 16x16), 7 CI4 32x32 icons, 1 empty
slot (CI8 16x16), 4x RGBA32 32x32 (one 64x64 picture), 4x CI8 24x24 (one 48x48 picture), 3 CI4 16x16 icons, 1 RGBA32
32x32 fade tile. Number: the same with 9+9 digit glyphs (1-9) and 2 extra RGBA32 tiles.

## Corrections to packC
- packC counted the 7 CI4 32x32 icon images of TexPair table L 0x8006DDF0 [N 0x8006F760] as `tlut` (they are 0x200 bytes,
  like a 256-colour palette); they are texels. Their partners (0x20) are the TLUTs.
- 10 packC "textures" (Letter 6, Number 4) are vertex morph animations (actor anim lists), plus one more in Letter.
- The 16-byte gaps packC put inside each RGBA32 range are 8 zero bytes + an empty display list, not texels.

## Unused / odd
- Number 0x80250058 and 0x80251068 (ROM 0x1125B8, 0x1135C8): two RGBA32 32x32 tiles nothing references. Letter has no
  such tiles.
- Letter ROM 0x184968-0x188C48 (RAM 0x802C2878): a morph animation (16 keys, 29 ticks, 267 vertices) that no pointer
  reaches; it has the same header as the referenced Number animations 0x8028B4A8/0x8028F788, so it is probably the
  Letter twin of Number's fourth animation, left out of Letter's table.
- G_LOADTLUT always loads 256 entries, so CI4 sprites read 0x1E0 bytes past their 16-colour TLUT (harmless).
- Number has only 9 glyph pairs, but the empty-slot and icon art is laid out identically to Letter.
