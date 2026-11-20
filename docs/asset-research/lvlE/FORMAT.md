# lvlE: the rest of the level images (bytes neither lvlB graphics nor lvlC structures cover)

Level images are the inflated `.bif` gzip members (RAM image at 0x80154000, absolute pointers; see
lvlA/lvlC). Offsets below are file offsets in the inflated image; RAM = 0x80154000 + offset. All big-endian.
Territory = image minus lvlB classes other than `level_data` minus every lvlC range (`tools/gaps.py` lists it).
Extractor: `tools/extract_level_rest.py ROM GAME OUTDIR [LEVEL...]` and `... check ROM GAME OUTDIR [LEVEL...]`
(GAME letter|number; works for any level of either game; it reads ../lvlB/MAP.tsv and ../lvlC/MAP.tsv for coverage).
Output: OUTDIR/<level>/<OFFSET>_<class>.json (+ .png for textures, + _k0.obj point cloud for animations) and
OUTDIR/MAP.part.tsv. `check` re-packs every JSON and compares with the image bytes (OK for all 4 Letter files).

## How the classifier finds things (in this order; first claim wins; everything left is pad (zero) or unknown)
1. Every lvlB `model_header` (0x24 bytes) is followed by one more word -> `model_header_tail`.
2. `bbox` and `vertex_dup_list` are found through the model header pointers.
3. `collider_list` = any word pair {n (1..64), ptr} in the territory whose ptr leads to n pointers to 0x50-byte records.
4. `texload_dl` = 8-aligned runs starting `E7000000 00000000` (gsDPPipeSync) made only of RDP state/load commands
   up to `B8` (gsSPEndDisplayList); its gsDPSetTextureImage targets become `tex_palette` / `tex_image`.
5. `morph_anim` headers (rule below) at 4-aligned offsets.

## model_header_tail (4 bytes)
lvlB's model header is 0x28 bytes, not 0x24: word +0x24 = RAM pointer to the model's bbox. func_8000092C
reads `*(hdr+0x24)` and adds the bbox centre ((min+max)/2) to the actor position. The bytes after it (often 8
zero bytes in streetab) are pad.

## bbox (24 bytes)
f32 min_x, min_y, min_z, max_x, max_y, max_z. JSON: words + f32.

## vertex_dup_list (u16 list; pointer at model header +0x14)
Groups `{u16 n; u16 m; u16 idx[m]}`: n new vertices are taken in order from the animation key data, then m
vertices are copied from already written vertex indices idx[] (func_80000450 writes the model's Vtx array this
way). The list has no terminator: it ends when sum(n + m) == model header u16 at +4 (the Vtx count).
Header u16 +2 = sum(n) + 1. Zero bytes after it are pad.

## morph_anim (same format as sprA/FORMAT.md, embedded in the level)
| off | type | field |
|---|---|---|
| 0 | u16 | hdr_end = 8 + nkey |
| 2 | u16 | data_off: data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141) |
| 4 | u8 | ticks (byte 4, compared by func_8002AB50 / func_8000092C) |
| 5 | u8 | nkey |
| 6 | u16 | flags (0 or 0x8000 = looping) |
| 8 | u8[nkey] | key ticks, first 0, increasing, last <= ticks |
| data_off | u32[nvert][nkey] | vertex-major; word = z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1) |
nvert is not stored (it is the number of new vertices of the model, sum(n) of its vertex_dup_list); the
extractor takes floor(space / (4*nkey)) up to the next claimed structure or the end of the uncovered range.
Use: actor anim tables in program .data, e.g. Letter `D_80075D00[actor->unk_1D4]` (func_8002AB50) holds
0x801AA260, 0x801B7764, 0x801BCAF4 = the three streetab animations (Number street has the same three at
+0x340). Per frame func_8000092C(model_header, out, anim, tick>>4, ...) finds the key pair and func_80000450
interpolates into the model's Vtx (see sprA/FORMAT.md). Animations sit directly after their model's header.
JSON: header fields, keys, verts[nvert][nkey] = [x,y,z].

## collider_list / collider_ptrs / collider_quad
`collider_list` (8 bytes): u32 n; u32 RAM pointer to `collider_ptrs` (u32 RAM pointer[n], usually right before
the list). Each pointer -> `collider_quad`, 0x50 bytes:
+0x00 u32 kind (3 in all seen); +0x04..+0x0F zero; +0x10 f32[3] normal; +0x1C f32 plane distance;
+0x20 f32[12] four corners (x,y,z each). The code (func_80023118, actor ids such as 0x1015 -> D_801AA110 in
streetab) takes `*(list+4)` as the record base, allocates n hit entries, transforms the four corners
(func_800558EC, with y/z swapped and z negated) and registers each quad with func_800445D0.
JSON: words + f32 (lossless via the words).

## texload_dl, tex_palette, tex_image
Sub-display lists, 0x80 bytes each, that only load a texture: PipeSync; SetOtherMode; SetTextureImage(palette);
LoadTLUT (count = ((w1>>14)&0x3FF)+1, 16 or 256 colours); SetTextureImage(image) ; LoadBlock; SetTile CI
(fmt 2, siz 0 = CI4 / siz 1 = CI8); SetTileSize (w = ((w1>>12&0xFFF)>>2)+1, h = ((w1&0xFFF)>>2)+1); EndDL.
No gsSPDisplayList in the level reaches them; they come in groups (alternative skins / animated textures)
selected by game code. Palettes: u16 RGBA5551. Images: CI4/CI8 rows of w pixels. Textures are emitted as RGBA
PNG plus a JSON of raw bytes (the PNG is for viewing; check uses the JSON). A list that runs into a lvlB
display_list (chute 0x26150, streetab 0x3B910) is the head of a list lvlB decoded from a later entry point; its
palette still counts as tex_palette (256 colours).

## pad
Zero bytes (or 1-3 alignment bytes) between structures; stored in JSON (length, or hex when non-zero).

## unknown
Raw bytes in JSON (hex).
