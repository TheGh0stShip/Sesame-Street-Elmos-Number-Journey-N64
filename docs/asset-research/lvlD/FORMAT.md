# lvlD: mesh headers and their companions, camera models, and the program's .data tables into the level images

Scope: the structures lvlC left open. Both games, all 16 `.bif` level images (RAM image at 0x80154000) plus the
`common` image (0x8024B000) where a root points there. Big-endian. Number addresses are given in brackets.
Tool: `tools/extract_level_misc.py ROM GAME OUTDIR` / `... check ROM GAME OUTDIR` (standard library only).

Level images are inflated from the loader's ROM ranges (Letter func_8001D9D0, Number func_8001E2CC).
**Number river.bif is the gzip member at ROM 0x27ADC0-0x2B8FD0**: it inflates to 0x6C07A bytes. Inflating only
up to 0x2B5120 (the range used in lvlA/lvlB/lvlC) stops early at 0x6359A bytes. River's camera model at 0x801BFF98
lies in the missing tail. The tool refuses a member that does not reach its gzip end.

## 1. Where the roots come from (all from code)

| user | Letter | Number | selector -> root |
|---|---|---|---|
| type 7 actor (muppetact) | func_80023118 | func_80023A38 | `switch (id)` -> mesh header, and for some ids a collision-quad header |
| type 12 map object | func_8003BB00 | func_8003D3D0 | `switch (id)` -> mesh header (copied by func_80018530 [func_800189A0]) and morph anim -> actor+0x1BC |
| type 11 (Number only) | - | func_800408D0 -> func_800409A0, anims switched by func_80040E34 | id 0x5001/0x5003/0x5005/0x6000 -> mesh + 2 anims (0x6000 is the id the dispatcher gives type-18 records) |
| func_8003E3E0 (Letter) | case 0 / default | - | common meshes 0x8029CA80 / 0x802990C0 with anims |
| camera (cameraact) | func_8002A120, switch D_8012EFD0 | func_8002C800, switch D_8012F154 | level id -> camera-actor mesh (copied by func_80018530); anims from a .data table indexed by actor+0x1D4 in func_8002AB50 [func_8002D0A4] |

The root addresses are hard-coded per id. The image an address belongs to is the level whose object list (lvlC) has
an object of that type and id. If no level places that id, the tool takes every level where the address holds a
valid mesh header. Common addresses (>= 0x8024B000) go to the common image. The tables are written out in the tool
(`T7`, `T12`, `T11`, `E3E0`, `CAM`) and were read from the matched C. Each address was cross-checked against
`../LEVEL_REFS_<game>.tsv`, which is computed from the ROM, so the Number addresses are Number's own.

Camera mesh per level. Letter: orchard 0x80168D60, space 0x8015B7C8, planet 0x8015F3A8, chute and manor use common
0x802E3118, and the rest use common 0x802AEAC0. Number: bump 0x801AA700, ski 0x80187430, river 0x801BFF98, and the
rest use common 0x8027B9D0.

## 2. Mesh header (class `mesh_header`, 0x28 bytes)
func_80018530(hdr, n) [Number func_800189A0] allocates 0x28 + n*16 + hdr[+0x20] bytes. It copies 10 words of the
header, sets +0x0C to a heap copy of the n vertices, and, when +0x20 != 0, sets +0x10 = buffer and
+0x18 = buffer + (+0x04)*16, then copies (+0x04)*4 words. Type 7 does the same in place. The header is 0x28 bytes,
not 0x24 as lvlB's MAP has it.

| off | type | field |
|---|---|---|
| 0x00 | u16 | flags: 0x2000 = morph-animated (has a rebuild list); 0x8000 = runtime double-buffer toggle (func_8000092C) |
| 0x02 | u16 | no reader anywhere (copied only as part of word 0). unique+1 in animated meshes (54/54); arbitrary in static meshes: tool output |
| 0x04 | u16 | nverts: number of Vtx in the vertex buffer (the copy size) |
| 0x06 | u16 | 0 |
| 0x08 | ptr | display list (graphics, lvlB) |
| 0x0C | ptr | Vtx array (segment 3) |
| 0x10 | ptr | 0 in ROM; runtime second vertex buffer |
| 0x14 | ptr | rebuild list (0 if not animated) |
| 0x18 | ptr | 0 in ROM; runtime: set to B + nverts*16 (scratch after buffer B), no known reader |
| 0x1C | ptr | texture base (segment 4); func_800243AC rewrites it for actor 0x4002 (see table 0x80075C34) |
| 0x20 | u32 | work size: bytes of runtime vertex buffers (0 if not animated). Also gates func_80018530: it returns 0 when +0x20 == 0. The type-7 in-place path (func_80023118) memsets +0x20 bytes from +0x18, i.e. nverts*16 bytes past its block |
| 0x24 | ptr | bounding box; always header - 0x18 |

func_8000092C computes the box centre ((min+max)/2) into its by-value position arguments and never reads them again: the box has no runtime effect (no culling, no centring). It equals the exact min/max of the ROM Vtx (108/108).

## 3. Bounding box (`bbox`, 24 bytes)
f32 xmin, ymin, zmin, xmax, ymax, zmax, which are model-space vertex extents. JSON keeps the raw words.

## 4. Animated-vertex rebuild list (`rebuild_list`)
This is the u16 program run by func_80000450:
`loop { n = *p++; if n == 0 stop; write n vertices taken in order from the animation key; m = *p++; if m == 0 stop;
m x u16 idx: duplicate the already-written vertex idx }`. Sum(n) = unique vertices = the vertex count of every
morph anim used with the mesh. Sum(n+m) = nverts. In all 54 lists (38 in the levels, 16 in common) the list ends with an explicit 0
(either an n or an m position), which the MAP range includes. JSON: `runs` = [[n, [idx...]], ...] and
`terminator` 'n'|'m'.

## 5. Morph animation (`morph_anim`), the "hit structure at header + 0x28"
The word stored in actor+0x1BC is not a collision structure. func_8003CF50 passes it to func_8000092C as the
animation, and it is the sprA keyframe format (../sprA/FORMAT.md):
`u16 hdr_end; u16 data_off; u8 ticks; u8 nkey; u16 flags(0x8000 loop); u8 keys[nkey]; pad to data_off;
u32[nvert][nkey]` (z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1); data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141)). For type 12 it usually sits right after its header
(header+0x28). Letter 0x2101 sits at +0x50 and 0x3103 comes through a .data table. nvert is not stored. It is the
mesh's unique vertex count. The tool checks this against the next structure. Where anims are packed back to back, each one
ends exactly where the next begins, and none overlaps another structure. One exception is Number river 0x801AA088 (camera anim 19), which is one vertex row short. The tool cuts
it to 373 and notes it in the JSON.
Note: hdr_end's low byte is 8 + nkey. One anim (Number river 0x801B7430) has 0x2000 set in hdr_end. The decoder
never reads hdr_end, and the JSON keeps it raw.

Type 7 actors with animated meshes get no anim from the image. Their anims are the ROM sprA files streamed by the
script code (func_800272C8 / func_80026B70).

## 6. Actor collision quads (`coll_quad_header`, `coll_quad_ptrs`, `coll_quad`), type 7 only
`spD4` in func_80023118 [func_80023A38] for ids such as 0x1015, 0x1501, 0x3002, 0x5501 [0x1015, 0x3002, 0x5501,
0x7004, 0x7005, 0x7006, 0x7016]:
- header 8 bytes: u32 count, ptr table.
- table: count x u32 pointers to quads. The code reads only table[0] and indexes `table[0] + i*0x50`, and the quads
  are contiguous in every case.
- quad 0x50 = 20 f32. +0x00..+0x1F: the pre-baked runtime item header, never read (the loader recomputes it): u32 type 3, u32 owner 0, u32 mask 0, u32 0, f32 plane nx, ny, nz, d of the quad's own corners c0,c1,c2 (39/39). +0x20..+0x4F: four corners x,y,z. Each corner
  is transformed by the actor matrix (func_800558EC) with (x, z, -y) swizzles. The result becomes a 0x50-byte runtime
  collision item that func_80020A24 finishes and func_800445D0 links into the world list.

## 7. Program .data tables that point into the images (class `data_ptr_table`, MAP rows are ROM ranges)
These are all the `data_word@` rows of ../LEVEL_REFS_<game>.tsv (934 Letter, 316 Number words). `pair` = {TLUT,
CI texels} word pairs. Each one is loaded with a code-built LoadTLUT + LoadBlock sequence (format chosen in code).
The tool dumps each table (`data_tables.json`) with its words and the parsed structures they hit. `check` compares
the words with the ROM.

| Letter RAM (words) | Number RAM (words) | used by | entries |
|---|---|---|---|
| 0x8006DDF0 (14) | 0x8006F760 (14) | func_80004010 [func_80003E18] | 7 HUD icon pairs (common), index D_8013EB38[i].m4 [D_8013ECA8] |
| 0x8006DE28 (104) | 0x8006F798 (18) | same | glyph pairs (common): 52 letters, index D_8012FD3C [9 digits, index D_8012FEC4] |
| 0x8006DFC8 (104) | 0x8006F7E0 (18) | same | large glyph pairs (common), same index |
| 0x8006E5A0/E6B0/E7C0 (68 each) | 0x8006FC60 (18) | same | bonus level panels (Letter bonusabc, level 19; Number bonus123, level 11), indices D_800745F0/F4/F5 [D_8006FE5C/5D] |
| 0x8006E8D0/EA78/EC20 (106 each) | 0x8006FCA8 (20) | same | more bonus panels, same indices [D_8006FE5C/5E] |
| 0x80074664/714/7F4/8C4 (44/56/52/6) | 0x80075240/2F0/3D0/4A0 (same) | func_8001A318 [func_8001AC1C] | mode-22 menu image pairs (packA) |
| 0x80075C34 (10) | - | func_800243AC | 10 texture-block frames (0xA00 apart, space.bif) for actor 0x4002, written to mesh+0x1C |
| 0x80075D00 (20) | 0x800768A0 (29) | func_8002AB50 [func_8002D0A4] | camera-actor morph anims, index actor+0x1D4. Entries point into common or into the level that owns the matching camera mesh |
| 0x80076360 (1) | 0x80076CE0 (1) | no instruction addresses it | anim pointer of map object 0x6601 (common) |
| 0x80076B18 (2) + 0x80076B20 (1) | 0x80077494 (4) + 0x800774A4 (2) | func_8003BB00, func_8003CF50 [func_8003D3D0, func_8003E078] | D_..B20/D_..4A4 = table of anim lists indexed by actor+0x1DB. Each list is indexed by actor+0x1DA. Letter: one list for 0x3103 (common). Number: list 0 -> ski 0x7601, list 1 -> mansion 0x7101 |
| - | 0x80076A00 (27) | func_80035260 | common model headers; index = func_800217C4(9) (random) |
| - | 0x800774B0 (9) | func_8003FC40 | common model headers |

## 8. JSON
`OUTDIR/<level>.json` and `common.json`: `structures` sorted by offset. Each one has kind, ram, off, size,
referenced_by, the fields above, and raw words wherever floats are shown. `data_tables.json`: tables, unplaced ids,
camera mesh per level. `MAP.part.tsv` holds the level rows plus the .data ROM rows (common structures are listed in
JSON only: packC owns those bytes).
