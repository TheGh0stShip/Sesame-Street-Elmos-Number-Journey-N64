# Graphics inside the level files (.bif), both games

## Container (summary; agent files / lvlA own it)
Each `.bif` is one gzip member (inflate with `zlib.decompressobj(31)`); the result is a prelinked RAM image
loaded at **0x80154000** (Letter func_8001D9D0 -> gzip reader func_80013560; Number func_8001E2CC -> func_800139D0).
Every pointer is absolute (no relocation). After the level, the raw `common` block is copied to 0x8024B000;
level display lists also point into it (textures there are counted as `common_textures` but not owned here).
Offsets in MAP.tsv are offsets inside the inflated file (RAM = 0x80154000 + offset).

## How the game reaches the graphics
1. **Model records** (class `model_header`, 0x24 bytes): `+0x00 u32 flags; +0x04 u16 vertex count; +0x06 u16;
   +0x08 u32 display list; +0x0C u32 segment-3 base (vertex block); +0x10,+0x14,+0x18 u32 (not graphics; +0x14
   points past the vertex block); +0x1C u32 segment-4 base (texture block, 0 if none); +0x20 u32 texture block size`.
   The draw functions (Letter func_80024C1C, func_8002BADC, func_80029D48, func_80034B64, func_8003E220,
   func_8003EF84, func_8003FC04) emit `gsDPPipeSync; gsDPSetCombine; gsSPSegment(3, K0_TO_PHYS(+0x0C))`
   (0xBC000C06), `gsSPSegment(4, +0x1C)` (0xBC001006), `gsSPMatrix`, `gsSPDisplayList(+0x08)`.
   The record's DL is usually a 16-byte wrapper `gsSPDisplayList(real); gsSPEndDisplayList`; the real DL addresses
   vertices as `0x03xxxxxx` and textures/TLUTs as `0x04xxxxxx`.
2. **Absolute display lists**: other DLs use 0x8015xxxx-0x801Dxxxx (and 0x8024B000+ common) addresses directly.
   They are reached from code (LEVEL_REFS_<game>.tsv: addresses built by code / .data pointers, e.g. Letter
   func_80023118 object table, func_8003BB00, func_80024E20), from pointer words inside the image, and from
   `gsSPDisplayList`.

## Display lists (class `display_list`)
F3DEX 1.23 (GBI 1, F3D opcode numbers), 8-byte commands, ending with `gsSPEndDisplayList` (B8) or a branch.
A DL candidate is accepted when every command is plausible up to the end and it contains geometry or texture
commands. Overlapping/adjacent entry points are merged into one block. Text form (`dl/dl_<ADDR>.txt`): one
macro per line, entry labels `dl_<ADDR>:`; **every argument is the raw bitfield** (divided by 2 for vertex
indices and v0), in the field order of the table `OPS` in tools/extract_level_gfx.py (e.g.
`gsSPVertex(addr, n, v0, len_m1)`, `gsDPSetTile(fmt, siz, line, tmem, tile, pal, cmt, maskt, shiftt, cms, masks, shifts)`,
`gsDPLoadTLUTCmd(tile, count_x4)`). Any command whose set bits are not fully described would be written
`gsDPWord(w0, w1)`; none occur. `asm()` in the tool is the exact inverse.

## Textures and palettes (`texture`, `palette`)
Format, size and palette come from tracing the DL with segments 3/4 set from the model record:
`SetTextureImage` + `SetTile` + `LoadBlock`/`LoadTile` + `SetTileSize` (width from tile line for LoadBlock,
height = loaded bytes / row bytes) and `LoadTLUTCmd` (count). Seen: CI8 and CI4 with RGBA5551 TLUTs (256 or 16
entries, typically 0x200 bytes before the texel block), RGBA16, IA/I. PNG: CI as paletted PNG with tRNS alpha (lossless), RGBA16 as
RGBA (5-bit replicated), IA as LA, I as L. Texture name `tex_<ADDR>_<FMT><bits>_<W>x<H>.png`.

## Vertices (`vertices`) and OBJ
Vtx 16 bytes: s16 x,y,z, u16 flag, s16 s,t (10.5), u8 r,g,b,a. Ranges from `gsSPVertex` (n*16).
`level.obj`/`level.mtl`: one group per top-level DL, triangles from the vertex cache simulation, model
space (matrices not applied), vertex colours as extra v components, UV = s/32/width.

## Other classes
`level_data`: everything not reached as graphics (object tables, collision/terrain, animation...: lvlA's
territory). `comment`: trailing ELF `.comment` "GCC: (GNU) 2.7.2" strings (linker leftover). `pad`: zero runs.
