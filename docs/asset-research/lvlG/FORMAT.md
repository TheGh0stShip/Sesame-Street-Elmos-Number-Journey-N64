# Number Journey level files (street, dunk, bump, mansion): what the bytes outside lvlB and lvlC are

Each .bif inflates to a ready-made RAM image at 0x80154000 (file offset = address - 0x80154000). All
pointers are absolute, all values big-endian. My territory is the inflated bytes not covered by lvlB's graphics
classes (everything except `level_data`) or by lvlC's object/BSP structures. `tools/extract_level_rest.py` finds the
territory from ../lvlB/MAP.tsv and ../lvlC/MAP.tsv, classifies it with the rules below, and asserts that the items
cover the territory exactly, with no gaps and no overlaps.

## 1. Vertex morph animation (`morph_anim`): 89.6% of the territory
The format is the same as the ROM animation files described in ../sprA/FORMAT.md. The level files contain more of them.
| off | type | field |
|---|---|---|
| 0 | u16 | hdr_end = 8 + nkey |
| 2 | u16 | data_off: data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141). |
| 4 | u8 | ticks (animation length; actor tick counter limit, func_8002D0A4 compares `anim[4] << 4`) |
| 5 | u8 | nkey |
| 6 | u16 | flags, 0x8000 = loop (key nkey-2 interpolates towards key 0) |
| 8 | u8[nkey] | key ticks, first 0, strictly increasing, last <= ticks |
| hdr_end | 0 bytes | pad to data_off |
| data_off | u32[nvert][nkey] | vertex-major. Word = z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1) |

- Playback: func_8000092C(mesh, out, anim, tick, ...) finds the key segment and calls func_80000450, which interpolates
  linearly into the mesh's Vtx buffer in the order given by the mesh's vertex run list (section 2).
- nvert is not stored in the animation. It equals the mesh's unique vertex count (the sum of `n` in the run list). The
  extractor takes nvert = min(ceil(non-zero length / row), floor(space to the next item / row)), where
  row = 4*nkey. The results match the run lists exactly: bump 94 and 78, mansion 52 and 72, and 318 for bump's mesh
  0x56700 (the six 9-key/24-key animations).
- How they are reached: the actor animation table D_800768A0 (ROM 0x774A0, 29 pointers, indexed by actor+0x1D4 in
  func_8002D0A4) holds street 0x565A0/0x63AA4/0x68E34 (entries 10-12) and bump 0x61D70/0x64A40/0x67710 (entries 13-15).
  Map objects (func_8003D3D0) set obj+0x1BC to the animation that directly follows a mesh header: bump 0x58898 (id
  0x6102), 0x5DB58 (0x6104), mansion 0x380E8 (0x7103). Mansion 0x34668/0x35528 come from the program data table at
  0x80077494 (itself pointed to from 0x800780A8).
- Within a level, the animations sit at the end of the file and are packed back to back. The animations that follow a
  mesh header start at header+0x28 (bump, mansion) or at the next 16-byte boundary (street 0x565A0, 8 pad bytes). The
  slack after the last animation is zero (`pad`), and then come the GCC .comment strings (lvlB class `comment`).

## 2. Vertex run list (`vertex_runs`), pointed to by mesh header +0x14
u16 stream, read by func_80000450:
`repeat { n; if n == 0 stop; (n vertices are taken in order from the animation); m; if m == 0 stop; m x u16 index of an
already-written output vertex to duplicate }`. The unique vertex count is sum(n). JSON: a list of `{n, dup:[...]}`, ending in
`{n:0}` or in a run with an empty `dup`. Odd-length lists leave a 2-byte zero `pad` before the next item.

## 3. Mesh header tail and bounding box
- `mesh_header_tail`: the u32 at mesh header +0x24 (lvlB's `model_header` covers only +0x00..+0x24). It points to
  the bbox. +0x28 is the "anim" field that func_8003D3D0 copies to obj+0x1BC. For animated meshes the
  animation starts there; otherwise zero bytes follow (`pad`).
- `mesh_bbox`: f32[6] = min x, y, z, max x, y, z. func_8000092C adds (min+max)/2 to the draw position.

## 4. Collision quads for character actors (`coll_quad`, `coll_quad_ptrs`, `coll_quad_set`)
func_80023A38 (the type 7 "muppet actor" handler) picks a `model` for some ids (0x7004 D_8017C62C, 0x7005 D_8017E3AC,
0x7006 D_8018012C, 0x7016 D_801867CC, and street's D_801AA450). The model is a `coll_quad_set`:
| off | type | field |
|---|---|---|
| 0 | u32 | count |
| 4 | u32 | ptr to `coll_quad_ptrs` (u32[count] of quad pointers; the code reads only the first and indexes the quads as a contiguous 0x50-byte array, which holds in every level) |

`coll_quad`, 0x50 bytes, laid out like the runtime type-3 collision shape (walker func_80042EB0 -> func_80040C00):
| off | type | field |
|---|---|---|
| 0x00 | u32 | type = 3 |
| 0x04 | u32 | owner (0 in the file; set at runtime) |
| 0x08 | u32 | flags (0 in the file; runtime 0x02000000) |
| 0x0C | u32 | 0 |
| 0x10 | f32[4] | plane (normal + distance, precomputed; the handler recomputes it with func_80021334) |
| 0x20 | f32[3] x4 | corners a, b, c, d (object space; the handler swaps y/z and transforms by the actor matrix) |
For each quad the handler allocates a coll item and a shape, then links them into the actor's group with func_80046250.

## 5. Texture-load display lists and their textures (`texload_dl`, `palette`, `texture`)
These are 0x80-byte F3DEX 1.23 DLs, packed back to back: pipesync; SetOtherMode TLUT RGBA16; SetTImg palette; tlutsync;
SetTile 7; loadsync; LoadTLUT (16 or 256 colours); pipesync; SetTImg image (16-bit load); SetTile 7; loadsync; LoadBlock;
pipesync; SetTile 0 (CI4 or CI8); SetTileSize; EndDL. Each one loads one palette (RGBA5551) and one CI image of 16x16
to 64x32 pixels. No code address, program data word, or level pointer reaches these DLs. lvlB's walker did not reach them,
so their textures were left as `level_data`. They are probably reached through an index or runtime arithmetic I did not
find, or they are unused material DLs. The extractor writes the textures as RGBA PNG plus a grey index PNG (used for
the lossless check), the palettes as JSON u16 lists, and the DLs as text.

## 6. Prologue display lists (`display_list`), bump 0x00-0x30
Two 0x18-byte DLs: pipesync; SetCombine; EndDL. lvlB maps the same pair (entries 0x80154000/0x80154018) in the other
levels. In bump, lvlB did not reach them.

## 7. `pad` / `unknown`
- `pad`: zero bytes used for alignment, mostly 4-byte holes between BSP leaves and slack at the end of files.
- `unknown`: bump has four zero blocks of 0x600 bytes (0x262C0, 0x30250, 0x3A1E0, 0x44170). Their size matches a
  CI8 palette plus a 32x32 CI8 texture, but nothing points at them. bump 0x4FB80 (0x220 bytes) is laid out like a
  16-colour TLUT plus a CI4 32x32 image, but no DL or pointer reaches it. Both are dumped raw.

## Extractor
`tools/extract_level_rest.py ROM number OUTDIR [extract|check|map]` inflates the four gzip members from the ROM
(ranges in the script, from ../files/FILES_number.tsv). It then classifies the territory, writes OUTDIR/<level>/<class>_<offset>.*
(JSON; animations also get one OBJ point cloud per keyframe; PNG; TXT), and writes OUTDIR/MAP.tsv. `check` rebuilds every
item from the extracted files and compares it with the inflated bytes. Result: 540 items, 0 mismatches.
