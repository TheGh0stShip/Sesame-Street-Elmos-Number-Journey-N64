# sprA report: the `sprite_anim` territory

Settled from the code: these are **keyframed vertex animations** for the 3D character meshes, not
sprites and not speech audio. func_80026920 only DMA-copies them into a heap buffer; the bytes are then
decoded by func_8000092C -> func_80000450 into mesh vertex positions (packed z:s10/y:s11/x:s11 from the MSB, linear
interpolation between keyframes). See FORMAT.md.

## Counts
| game | range | bytes | anims | keyframes | vertex rows | data bytes | pad bytes |
|---|---|---|---|---|---|---|---|
| letter | 0x61AFB0-0x741920 | 1,206,640 | 24 | 898 | 8,047 | 1,206,378 | 262 |
| number | 0x61F510-0x7CE410 | 1,765,120 | 35 | 1,324 | 11,706 | 1,764,740 | 380 |

Pad = zero bytes between key table and vertex data plus the 0-12 byte stale tail per file.
Vertex counts per anim (= mesh unique-vertex counts): Letter 380 (7 anims), 346 (8), 291 (9);
Number 380 (11), 339 (11), 309 (11), 214 (2). Looping anims (flag 0x8000): Letter 0x61AFB0, 0x6A0760,
0x73F930; Number 0x61F510, 0x6A44D0. Longest: Number 0x6E3450 (65 keys, 255 ticks).
check mode: all 59 files repack byte-exact.

## Unreferenced / odd
- Number 0x7470D0 (58 keys, 309 verts) and 0x758920 (22 keys, 309 verts) are packed after the
  referenced 0x739C20 and 0x7A7F10 (17 keys, 214 verts) after 0x79CD90; no code symbol points at them
  and the loader copies only symbol start..end, so if 0x739C20's load uses end = D_0075F380 (not checked) it pulls them
  into RAM but the decoder never reaches them: unused (cut) animations. Letter has none.
- All Letter start symbols are referenced (func_800272C8, func_80026B70). Number references were
  taken from the coordinator's note (func_800287AC / func_80027FE0) and the D_ list in ASSET_FACTS;
  I did not disassemble Number to confirm each one individually.
- Tail words of each file are leftover packer buffer contents (vertex-like, non-zero), not zero fill.

## Not done
- Mapping each anim to its character/mesh (by vertex count it is decidable: match against the
  mesh vertex lists at mesh+0x14 in the gzip level data) and rendering with faces.
