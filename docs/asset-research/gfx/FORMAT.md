# Graphics formats (Elmo's Letter Adventure / Number Journey)

Microcode F3DEX 1.23 (GBI 1.x opcodes). All graphics are pre-linked RAM images: pointers inside them are absolute
KSEG0 addresses; there is no file header or resource table. Assets are found by (a) display lists, (b) model headers,
(c) code that builds display lists itself.

## RAM images
| image | ROM (letter / number) | RAM | loaded by |
|---|---|---|---|
| main .data | 0x6D5AC.. / 0x6EF1C.. | 0x8006C9AC / 0x8006E31C | boot |
| common | 0x10D0F0-0x1BF6F0 / 0x10D560-0x177680 | 0x8024B000 | DMA after every level load, func_8001D9D0 / func_8001E2CC |
| level image | 10 per game (2 raw: ids 22, 21; 8 gzip `*.bif`) | 0x80154000 (max 0x86800) | same functions, switch on level id |

Gzip members: standard gzip (flag FNAME, e.g. `chute.bif`); the game skips the 10-byte header + name in func_80013560
(letter) and inflates with its own zlib. Level ids: letter 22,21 raw; 2 streetab,16 chute,15 manor,18 orchard,17 garden,
14 space,13 planet,19 bonusabc. Number: 22,21 raw; 1 street,9 dunk,10 bump,8 mansion,7 maze,6 ski,5 river,11 bonus123.
Note FILES_letter.tsv splits raw level 21 at 0xA8D50; the game loads 0xA54D0-0x10D0F0 as one image.

## Display lists
Standard 8-byte Gfx. Two segments are used, set by the actor draw callback (letter func_80029D48):
segment 3 = model vertex buffer, segment 4 = model texture base. Everything else uses absolute 0x80xxxxxx addresses.
Textures are always loaded with LoadBlock (gsDPLoadTextureBlock pattern: SetTextureImage 16b, SetTile tile 7, LoadBlock,
SetTile tile 0 with real fmt/siz/line, SetTileSize). Width = line*64/bits, height = loaded bytes / row bytes.
CI textures: TLUT loaded with LoadTLUT to TMEM 0x100 (16 or 256 RGBA16 entries).

## Model header (0x28 bytes, in common or level image)
```
+00 u16 flags (0x2000 animated, 0x8000 double-buffer toggle at runtime)   +02 u16 vertex count
+04 u16 triangle count  +06 u16 0
+08 Gfx* wrapper list (gsSPDisplayList(real); gsSPEndDisplayList)
+0C Vtx* vertex buffer (segment 3)    +10 Vtx* second buffer (0 in ROM, runtime)
+14 u16* animated-vertex rebuild list (animated models)   +18 0
+1C texture base (segment 4)   +20 u32 size of the animation vertex work data   +24 f32* bbox (6 floats, = header-0x18)
```
Used by func_8000092C (picks the keyframe) and func_80000450 (rebuilds vertices).
Rebuild list: repeat { u16 n: copy n packed vertices; u16 m; m x u16 indices of vertices to duplicate } until a 0.

## Vertex animation files (ROM, streamed by func_80026920 letter / func_80027D90 number via D_80106874)
```
+0 u16 header length (8+nkeys)  +2 u16 data offset (8-aligned)  +4 u8 last frame  +5 u8 nkeys  +6 u16 flags (0x8000 loop)
+8 u8 keyframe numbers[nkeys]; pad
data: for each vertex, nkeys u32: z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1). data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141)
```
Frames between keys are linearly interpolated (func_80000450). (FILES_NOTE calls these `sprite_anim`; they are 3D
vertex keyframes.) Extracted as JSON (frames[key][vertex] = [x,y,z]).

## Code-drawn graphics
- HUD / menus: letter func_80004010, func_8001A318; number func_80003E18, func_8001AC1C. Write LoadTextureBlock sequences
  directly; many read {palette, image} pointer pairs from .data tables (letter D_8006DDF0..D_8006EC20, D_80074664..D_800748C4).
- Full-screen backgrounds: letter func_8001CAA8 / number func_8001D3AC(image, palette): CI8 320x240 drawn as 32x48
  LoadTile strips, palette 256 RGBA16 just before the image, in level image 21.
- func_8001D1BC (letter): CI4 320x240 variant, no callers.
- Sprite font: descriptor {char *charset, Bitmap *glyphs} (letter 0x80071CFC, number 0x80072D7C) + SDK Sprite at +0xC;
  atlas I4 64x128 (`width_img` 64); glyph = SDK Bitmap {s16 width, width_img, s, t; u8 *buf; s16 actualHeight, LUToffset}.
  Text printing: func_8000D810 / func_8000DEBC -> spDraw.
- Debug font: 96 glyphs 1bpp 8x8 (bit 0 = leftmost pixel), at descriptor+0x164; func_8000E7A0 renders into a I4 buffer
  for the debug page overlay func_80001F60.
