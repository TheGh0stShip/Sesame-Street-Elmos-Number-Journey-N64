# Number Journey level images: the bytes lvlB and lvlC left (maze, ski, river, bonus123)

Each `.bif` is a gzip member that inflates to a ready-made RAM image at 0x80154000 (file offset = address -
0x80154000; all pointers absolute, big-endian). This document covers the structures found in the bytes that neither
lvlB's graphics classes (other than `level_data`) nor lvlC's object/BSP structures cover. Function names are Number
addresses, checked against the disassembly (`s.sh dis`), not against twin source names.

## 1. Model header: the parts lvlB does not cover
lvlB records the header as 0x24 bytes. It is 0x28 bytes: func_800189A0 (model instancer) copies 40 bytes
(`func_80000EDC(dst, hdr, 10)` words). Fields used here:

| off | type | use |
|---|---|---|
| +0x04 | u16 | Vtx count in the vertex block |
| +0x08 | ptr | display list (segment 3 = vertex block, segment 4 = texture block) |
| +0x0C | ptr | vertex block (segment 3 base); func_800189A0 copies count*16 bytes of it to the instance |
| +0x14 | ptr | **vertex order list** (section 2) |
| +0x1C | ptr | texture block (segment 4 base), +0x20 u32 its size (func_800189A0 allocates count*16+40+size) |
| +0x24 | ptr | **bounds**: 6 x f32 (`bounds`, usually just before the header) |

Class `model_header_tail` = the +0x24 word. Class `bounds` = the 0x18-byte block: f32 xmin, ymin, zmin, xmax, ymax, zmax
(exact vertex extents, proven; read only by dead code in func_8000092C, no runtime effect).

What follows a header: for actor and map-object models the code takes `header + 0x28` as the first animation
(func_8003D3D0: `obj->unk_1BC = D_801B55C8` for header D_801B55A0, etc.).

## 2. Vertex order list (`vertex_order`)
Read by func_80000450 (morph decoder) through model +0x14. u16 big-endian:
```
loop: n = next u16; if n == 0: end
      n animated vertices are written sequentially from the animation
      m = next u16; if m == 0: end
      m u16 indices: copy x,y,z of already written output vertex [index]
```
The number of animated vertices of a model (sum of n) equals the vertex count of every animation played on it.

## 3. Morph animations (`anim_header`, `anim_vertices`, `anim_tail`)
Same format as the ROM files of agent sprA (see ../sprA/FORMAT.md), here embedded in the level image.
Header: u16 hdr_end (= 8 + nkey; high byte 0 except one river animation where it is 0x20: never read), u16 data_off,
u8 ticks, u8 nkey, u16 flags (0x8000 = loop), u8 key ticks[nkey] (first 0, strictly increasing, last <= ticks),
zero bytes up to data_off. **data_off = (hdr_end & ~7) + 8** (so hdr_end 0x10 gives 0x18, 0x18 gives 0x20; the rule
"round up to 8" in sprA's text only holds when hdr_end is not a multiple of 8).
Data: u32[nvert][nkey], vertex-major, word = z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1). func_80000450
addresses vertex v, key k at `anim + ((data_off + 4k) & ~3) + v*nkey*4` and interpolates k -> k+1 (loop: last
segment wraps to key 0).
nvert is not stored. Animations are packed back to back (start alignment only 4); the extractor takes the run up to
the next valid header or the next foreign structure and divides by 4*nkey. Remainders (< one row) are `anim_tail`.
Each animation's nvert was compared with the models' animated-vertex counts (REPORT).
Who finds them: header+0x28 (func_8003D3D0 map objects, func_800409A0 type-11 objects); func_80040E34 switches the
type-11 objects between pairs of animations (bonus123 0x8016DA50/0x80170E34 etc.); table D_800774A4 -> D_80077494
(ski: 0x80187460/0x80188BA8 for model 0x8018C8C8 and 0x80188668/0x80189528 for 0x80188640); table D_800768A0
(speech-scene animations, func_80027FE0: `anim = D_800768A0[obj->unk_1D4]`, entries for the common block and for
several levels).

## 4. Code-built animated textures (`texanim_table`, `palette`, `texture`)
func_80025DF0 (maze, level 7) and func_80025AD0 (river, level 5) are called by the world handler func_80026110 and by
func_80027838 every frame; they build a display list into a heap buffer:
`SetTextureImage(RGBA16, table[i]) ; LoadTLUT 16 colours ; SetTextureImage(CI16-load, table[i] + 0x20) ;
LoadBlock 0x200 texels... ; SetTile CI4 ; SetTileSize 32x32`, with i = counter/2 and the counter wrapping at 16 (maze,
8 frames, table D_8019F170) or 26 (river, 13 frames, table D_801B3C90). Each frame is 0x220 bytes: 16-colour
RGBA5551 TLUT (0x20) + CI4 32x32 texels (0x200). The table is u32[n] frame pointers.

## 5. Display lists, vertices, textures that lvlB did not reach
The tool walks display lists from: lvlC BSP leaves (+0x10 DL, the world's draw lists); every model header (+0x08 with
segment 3 = +0x0C and segment 4 = +0x1C); and any 8-aligned run of valid F3DEX commands in free space that ends with
gsSPEndDisplayList or runs into a list lvlB starts later (head fragments). Followed commands: G_DL (0x06),
G_VTX (0x04: n = (w0 >> 10) & 0x3F, 16 bytes each), SetTextureImage (0xFD) + LoadTLUT (0xF0: count =
((w1 >> 14) & 0x3FF) + 1) / LoadBlock (0xF3: texels = ((w1 >> 12) & 0xFFF) + 1 in the load size), format from the next
SetTile of tile 0 (0xF5) and size from SetTileSize (0xF2). Only bytes no other agent owns are classified (a texture that
lvlB covered in part appears with a `[bytes a-b of n]` note).
Texture-load-only lists (river 0x14040.., maze 0x12FD0..) load CI8 64x32 textures with 256-colour TLUTs; nothing
static points at them (the 0x820-stride pointer table in program .data at 0x80075240 is not level specific, see REPORT).

## 6. Type-3 collision quads (`coll_quad`, `coll_quad_set`)
0x50 bytes: u32 type = 3, u32 0, u32 collision mask, u32 0, f32 nx, ny, nz, d (plane), f32[4][3] corners.
The ray test is func_80042880 (twin source named func_80040C00): plane test, then two triangles (c0 c1 c2) and
(c3 c2 c0 order as in the source). Called from the coll-list walkers func_80042EB0 / func_80043020 for `type == 3`
when `mover mask & shape mask`. In these levels every quad has mask 0 and the quads are grouped as
`u32 ptr[n]; u32 n; u32 &ptr[0]` with no reference found anywhere (image, program data, code constants): leftover
invisible walls that are not linked.

## 7. Other classes
- `texture_block_unused`: bytes inside a model's texture block (segment 4, +0x1C/+0x20) that its display list never
  loads (bonus123 model 0x80177360: 0xA00 bytes between an 8-byte CI4 16x1 strip and the next TLUT).
- unreferenced 0x220 frames inside texture blocks (ski 0x370E8, river 0x655F8) and a 0x28 TLUT16 + CI4 16x1 strip
  (river 0x62BC0) are decoded as `palette` + `texture` with a "not loaded" note.
- `pad`: zero runs (alignment between structures).

## Extractor
`tools/extract_level_rest.py ROM number OUTDIR [extract|check|map]` inflates the four .bif members from the ROM
(offsets from ../files/FILES_number.tsv), subtracts lvlB (non-`level_data`) and lvlC ranges (their MAP.tsv), classifies
the rest as above and writes `OUTDIR/number/<level>/<offset>_<class>.json` (decoded fields: words, floats, u16 lists,
header fields, vertices as [x,y,z] per key, GBI word pairs, palette entries, texels) plus a PNG for complete textures.
`check` re-encodes every item from its JSON form and compares with the image bytes, and proves that lvlB + lvlC + this
map tile each file with no gap. `map` prints MAP.tsv rows. (`letter` is accepted but was not validated here: lvlF owns it.)
