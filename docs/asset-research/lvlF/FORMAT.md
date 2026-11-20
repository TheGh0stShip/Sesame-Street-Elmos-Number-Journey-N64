# Level image "rest" formats (agent lvlF)

Territory: bytes of the inflated `.bif` level images (ready-made RAM images at 0x80154000, file offset =
address - 0x80154000) that neither lvlB (graphics, its non-`level_data` classes) nor lvlC (objects, BSP) cover.
Done for Letter garden, space, planet, bonusabc. The formats are engine formats and apply to Number unchanged
(Number twins: find them by disassembly). All values big-endian.

How the ranges were found (`tools/classify.py GAME file,file,...`, analysis only, reads other agents' MAPs):
roots = ../LEVEL_REFS_<game>.tsv entries plus every word inside the image that points into an uncovered byte.
LEVEL_REFS is NOT per level: a program .data table or a code literal may belong to another level or to a menu
file that is also loaded at 0x80154000 (e.g. the 0x820-stride tables at Letter 0x80074660-0x800747F0 used by
func_8000B470/func_8001A318 belong to menu screens). Every rule below therefore checks either the owning table
or the content (header validation).

## morph_anim: keyframed vertex morph animation (same format as sprA/FORMAT.md)
`u16 hdr_end (=8+nkey); u16 data_off; u8 ticks; u8 nkey; u16 flags (0x8000 loop); u8 key_tick[nkey]; zero pad
to data_off; u32 vertex words [nvert][nkey]` (word = z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1)).
data_off is the next multiple of 8 strictly above hdr_end in the in-level copies (0x20 -> 0x28) and in sprA's
files (0x31 -> 0x38): rule `(hdr_end + 8) & ~7`.
nvert is not stored: it equals the number of unique vertices of the model's vertex-order list (below); the
extractor uses floor((size - data_off) / (4 nkey)) with size = distance to the next item, and keeps the
remainder as `tail` (stale bytes, never read).
Reached by: func_8003BB00 map objects (literal address per object id), and the camera/actor animation table
D_80075D00[] (Letter .data 0x80075D00, indexed by actor->unk_1D4; func_8002AB50 -> func_8000092C ->
func_80000450).

## morph_vertex_order: vertex expansion list
Pointed to by model header +0x14 (lvlB `model_header`). u16 groups: `n` (0 = end), `m` (0 = end), `m` u16
indices. func_80000450 writes `n` vertices taken sequentially from the animation (s16 x,y,z into 16-byte Vtx),
then duplicates `m` already-written vertices by index. JSON: `groups: [{n, dup:[...]}, ..., {n:0}]` (or
`{n, m:0}` when the list ends on m = 0).

## model_header_ext and model_bounds
The model header is 0x28 bytes (func_80018530 copies 10 words); lvlB mapped 0x24. Word +0x24 is a pointer to
a 24-byte box `f32 min x,y,z, max x,y,z` (class model_bounds), stored just before the header.

## hud_palette / hud_texture (Letter bonusabc, func_80004010)
The bonus game draws its letter/picture cards with display lists built in code (gsDPLoadTLUT 16 colours from
`pal`, LoadBlock of a 4-bit image, texrect). Tables of `{u32 pal, u32 tex}` in .data, entry = 8 bytes,
tex = pal + 0x20:

| table (Letter .data) | entries | tile | bytes |
|---|---|---|---|
| D_8006E5A0, D_8006E8D0 | 34, 53 | 32x32 | 0x200 |
| D_8006E6B0, D_8006E7C0, D_8006EA78, D_8006EC20 | 34, 53, 53, 53 | 64x32 | 0x400 |
| literal 0x80165130/0x80165150 | 1 | 32x32 | 0x200 |

Format: palette = 16 RGBA5551; texture = CI4 (4 bits per texel, high nibble first; set as fmt I with TLUT
RGBA16 enabled, i.e. a CI4 image). Unreferenced items between table entries (4 pal+32x32 cards, 2 extra
64x32 images without own palette) have the same layout and are marked "unreferenced".

## texframe_palette / texframe_texture (Letter space, actor 0x4002)
func_800243AC: for actor id 0x4002 it sets the model copy's segment-4 base (+0x1C) to
D_80075C34[actor->unk_1D4] each frame, swapping the whole texture block. Each block = 256-colour RGBA5551 TLUT
(0x200) + CI8 32x64 texels (0x800) = 0xA00, laid out like the model's own block (model 0x80182DAC, seg4
0x8017DCD0: lvlB palette + CI8 32x64). 10 frames.

## unused_palette / unused_texture (Letter space)
Three palette+CI8 blocks inside the texture bank that no display list, model or code table reaches: 256-colour
TLUT + CI8 (dimensions from the gap and the neighbours: 32x32, 64x32, 32x32).

## coll_quad, coll_quad_ptrs, coll_quad_set (Letter bonusabc, actor 0x1501)
func_80023118 case 0x1501 sets a quad set 0x801A9C8C: `u32 count; u32 ptr -> u32[count]` pointers to quads;
the actor copies count*0x50 bytes. Quad (0x50), collision shape type 3 tested by func_80040C00 (via the shape
walker func_80041230): `u32 type=3, u32 0, u32 mask, u32 0; f32 normal[3]; f32 dist; f32 corner[4][3]`; the
ray test runs two triangles (c0,c1,c2) and (c2,c3,c0).

## display_list (bonusabc 0x0-0x30)
Two 3-command F3DEX lists (PipeSync, SetCombineMode, EndDL) at the image start, as in streetab, not mapped by
lvlB. JSON: raw words.

## pad
Zero bytes (alignment between items, trailing zero words). Checked to be zero.

## Extractor
`tools/extract_level_rest.py ROM GAME OUTDIR [check]`: inflates the level files (ROM offsets from
../files/FILES_<GAME>.tsv), reads ../MAP.tsv, writes OUTDIR/GAME/<file>_<offset>_<class>.(png|json); textures
as 8-bit paletted PNG (palette expanded 5->8 bit by bit replication, tRNS = alpha bit; exactly invertible).
`check` re-serialises every range from the output files and compares with the inflated bytes.
