# `common` graphics pack (packC)

Letter ROM 0x10D0F0-0x1BF6F0 (730,624 bytes); Number ROM 0x10D560-0x177680 (434,464 bytes).

## How the game loads it
- The level loader (Letter `func_8001D9D0`, Number `func_8001E2CC`) runs a level case first (it inflates a `.bif` gzip
  member to 0x80154000 or copies a menu screen), and then for **every** case it PI-copies this file **raw**, with no
  header and no compression, to **RAM 0x8024B000**. The copy goes through Letter `func_80017318` and Number `func_80017788`
  in 0x4000-byte PI DMA chunks. The code also refers to the end address (Letter 0x802FD600 in `func_80018840`/`func_80018A60`,
  Number 0x802B5120).
- The file is a **pre-linked RAM image**: every pointer inside it is an absolute KSEG0 address in
  0x8024B000..0x8024B000+size, and nothing relocates it. To turn a pointer into a file offset, subtract 0x8024B000.
- It has no directory. Objects are reached from three places:
  1. **Code and .data**: these are the addresses listed in `../LEVEL_REFS_<game>.tsv`, region `common`. Letter has 280 and
     Number has 137. Examples: `func_80018278` uses 0x8024B008 as an RGBA32 32x32 tile for the screen fade; `func_80004010`
     [Number `func_80003E18`] builds HUD display lists from TLUT+CI texel pairs; `func_80023118` [Number `func_80023A38`],
     `func_8003BB00`/`func_8003E3E0` [`func_8003D3D0`] and `func_8002A120` [`func_8002C800`] pick model headers by actor type.
  2. **Level display lists**: the inflated `.bif` level images at 0x80154000 hold `gsDPSetTextureImage`,
     `gsSPDisplayList` and `gsSPVertex` commands with absolute 0x8024xxxx-0x802Fxxxx addresses, so shared textures
     live here.
  3. **Pointers inside the pack**, starting from the model headers.

## Model header (0x28 bytes; class `model_hdr`)
`func_80018530(hdr, nverts)` [also `func_80023118` inline] copies the first 10 words into a heap object and
fills in the runtime fields.
| off | type | field |
|---|---|---|
| 0x00 | u16 | flags: 0x2000 = morph-animated, 0x8000 = runtime buffer toggle |
| 0x02 | u16 | never read (unique+1 for animated meshes, arbitrary otherwise) |
| 0x04 | u16 | nverts: number of 16-byte Vtx in the vertex array |
| 0x06 | u16 | 0 |
| 0x08 | ptr | display list (often a 2-command wrapper `gsSPDisplayList(real); gsSPEndDisplayList`) |
| 0x0C | ptr | vertex array (Vtx[nverts]); bound to **segment 3** (`gsMoveWd(6, 0x0C, ...)`, `func_80024C1C` etc.) |
| 0x10 | ptr | 0 in ROM; runtime: heap copy of the vertices |
| 0x14 | ptr | 0 or the vertex expansion list (see below) |
| 0x18 | ptr | 0 in ROM; runtime: buffer + nverts*16 |
| 0x1C | ptr | 0 or texture block base; bound to **segment 4** (`gsMoveWd(6, 0x10, ...)`) |
| 0x20 | u32 | runtime buffer size (bytes allocated after the vertex copy) |
| 0x24 | ptr | 0 or bounding box: f32[6] min xyz, max xyz = exact vertex extents (class `bbox`, usually stored right before the header); read only by dead code in func_8000092C, no runtime effect |

The display lists of a model load vertices as `0x03xxxxxx` (segment 3 = header+0x0C) and textures as `0x04xxxxxx`
(segment 4 = header+0x1C) or as absolute addresses. The extractor finds headers by structure: nverts in 1..4095,
+0x08/+0x0C in range, +0x10/+0x18 zero, the optional pointers in range, and a display list at +0x08 that decodes.
Every header the code refers to is found this way.

## Vertex expansion list (class `table`, header +0x14)
A u16 list of entries `{n, m, idx[m]}`: n vertices are taken in order from the animation source, then m existing
vertices (u16 indices) are duplicated. The entries run until the sum of n+m equals nverts, followed by an optional
u16 0 to reach 4-byte alignment. The sum of the n values is the number of **unique** vertices. This is the vertex
count of the morph animations; see `../sprA/FORMAT.md`, where `func_80000450` uses it.

## Vertex morph animation (class `morph_anim`, placed right after a model header)
This is the sprA format: u16 hdr_end (8+nkey), u16 data_off (read by the game; packer value ((8+nkey) & ~7) + 8, pad always 1..8 bytes), u8 ticks, u8 nkey,
u16 flags, u8 key ticks[nkey], zero pad, then u32[nunique][nkey] packed z:s10 (bits 31..22), y:s11 (21..11), x:s11 (10..0).
Size = data_off + 4*nkey*nunique. The code stores its address in actor+0x1BC (`func_8003BB00`, `func_8003E3E0` /
Number `func_8003D3D0`).

## Vertex array (`vtx`)
Standard F3DEX `Vtx`, 16 bytes each: s16 x, y, z; u16 flag (0); s16 s, t (10.5 fixed point); u8 r, g, b, a (colour or normal).

## Display list (`dl`)
F3DEX 1.23 (GBI1 opcode numbering), 8-byte commands. Opcodes used: 0x04 vertex, 0x06 DL/branch, 0xB1 2 triangles,
0xBF 1 triangle, 0xB6/B7 geometry mode, 0xB9/BA other mode, 0xBB texture, 0xE6/E7/E8 syncs, 0xF0 LoadTLUT,
0xF2 SetTileSize, 0xF3 LoadBlock, 0xF5 SetTile, 0xFA/FB colours, 0xFC combine, 0xFD SetTextureImage, 0xB8 end.
A list ends at 0xB8 or at a branch. Vertex indices are stored times 2.

## Textures (`texture`, `tlut`)
- Texels reached from a display list (the pack's own lists or a level's): the format and size come from the load
  sequence. LoadBlock texel count times the load size gives the byte count. Tile 0's SetTile gives the render format.
  Tile 0's SetTileSize gives the width and height. A TLUT is 16 or 256 RGBA16 colours, `LoadTLUT` count+1 entries.
- Texels referenced only by **code** (`format` column `code-set format`): the HUD/menu code builds the display list
  itself. `func_80004010` uses `gsDPSetTextureImage(RGBA16) + LoadTLUT` on a palette, then `SetTextureImage(CI, 16b)`
  on the texels. Code addresses usually come in pairs, a palette (0x20 or 0x200 bytes) and its texels, and so does
  the .data list at Letter 0x8006DDF0.. / Number 0x8006F760... The extractor classes a code-rooted range of 0x20 or
  0x200 bytes (or one whose next root is that far away) as `tlut`, and the rest as `texture`. A code-rooted
  texture object runs up to the next known object or root, because its size is set in code; any zero tail stays
  inside it.

## Collision quads
`coll_hdr` holds u32 count and ptr. It points to `ptr_table`, an array of count pointers. Each pointer points to a
`coll_quad`, a 0x50-byte record of 20 f32. Bytes +0x00..+0x1F are the pre-baked runtime item header, never read: u32 type 3, u32 0 (owner), u32 0 (mask), u32 0, f32 plane nx, ny, nz, d of the quad's own corners (39/39). Bytes +0x20..+0x4F hold four xyz corners, which `func_80023118` reads, swapping y and z and negating one of them. The quads are contiguous. The header is referenced by code (Letter
D_80271460 for actor type 0x5501).

## Padding / unknown
`pad` is zero runs of at least 32 bytes between objects. `unknown` is small leftover ranges that nothing references,
mostly 4-12 byte zero/odd tails between objects. A few look like unreferenced Vtx arrays or palettes; the
description column says which.

## Extractor
`tools/extract_packC.py ROM GAME OUTDIR` (GAME = letter|number). It reads only the ROM, plus `../LEVEL_REFS_<game>.tsv`
for the code roots, and inflates the level gzip members itself. Output:
- `dl/OFFSET_dl.gbi.txt`: one GBI macro per command. When a macro cannot reproduce the exact words, the command is
  written as `gsDPWord(w0, w1)`.
- `vtx/*.json`: [[x,y,z,flag,s,t,r,g,b,a], ...].
- `tables/*.json`: headers with named fields, the u16 lists, f32 boxes and quads (with raw words), and morph anims as words.
- `tex/*.bin` for all texels and palettes, plus `*.png` where the format and size are known from a display list.
- `raw/*.bin` for unknown data.
- `obj/model_XXXXX.obj`: the triangles of each model's display list tree, with vertices resolved through segment 3
  and UVs at s,t/1024.
- `index.json` and `MAP_<game>.tsv`.

`extract_packC.py check ROM GAME OUTDIR` re-assembles every `.gbi.txt` with its own assembler, re-packs the vertex,
table and morph JSON, reads the binaries back, zero-fills the pads, and compares the whole file with the ROM. It is
byte-exact for both games. `tools/run.sh` runs extract and check for both games through the lab Python and builds `MAP.tsv`.
