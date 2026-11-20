# lvlC report: level non-graphics data

Done (both games, all 16 levels): object list, object records, world list head, collision list item, collision shape,
BSP nodes and leaves, collision triangle arrays. `tools/extract_level_data.py ROM GAME OUTDIR` writes
`OUTDIR/<level>.json` and `MAP.part.tsv`; `tools/extract_level_data.py check ROM GAME OUTDIR` re-serialises every
structure and compares with the inflated bytes: **OK for both games** (0 mismatches). The roots are found by scanning
(object list, type-12 shapes with a valid tree), and they equal the addresses in the code for all 16 levels.
MAP.tsv: 10350 structures (`game, file:start, file:end, class, ...`).

### letter

| level | size | objects | bsp nodes | leaves | tris | object types | bytes mapped |
|---|---|---|---|---|---|---|---|
| bonusabc.bif | 354072 | 21 | 82 | 83 | 900 | {1: 1, 4: 1, 5: 1, 7: 7, 8: 7, 10: 1, 13: 1, 16: 1, 17: 1} | 10764 |
| chute.bif | 259860 | 107 | 188 | 189 | 1918 | {1: 1, 4: 1, 5: 1, 7: 21, 8: 30, 9: 1, 12: 51, 13: 1} | 27284 |
| garden.bif | 362576 | 53 | 337 | 338 | 3661 | {1: 1, 4: 1, 5: 1, 7: 8, 8: 30, 9: 1, 12: 9, 16: 1, 19: 1} | 41960 |
| manor.bif | 263356 | 177 | 152 | 153 | 1695 | {1: 1, 4: 1, 5: 1, 7: 94, 8: 30, 9: 1, 12: 48, 13: 1} | 27720 |
| orchard.bif | 283384 | 119 | 266 | 267 | 2985 | {1: 1, 4: 1, 5: 1, 7: 75, 8: 30, 9: 1, 12: 8, 16: 1, 19: 1} | 37840 |
| planet.bif | 346524 | 51 | 123 | 124 | 1392 | {1: 1, 4: 1, 5: 1, 7: 1, 8: 30, 9: 1, 12: 15, 17: 1} | 17364 |
| space.bif | 245496 | 48 | 0 | 1 | 10 | {1: 1, 4: 1, 5: 1, 7: 12, 8: 30, 9: 1, 12: 1, 17: 1} | 2812 |
| streetab.bif | 509070 | 21 | 194 | 195 | 2229 | {1: 1, 4: 1, 5: 1, 7: 3, 10: 2, 12: 10, 13: 1, 16: 1, 17: 1} | 24144 |

bytes per class: bsp_leaf 43200, bsp_node 53680, coll_list_item 96, coll_shape 160, coll_tris 59160, object 31108, object_list 2420, world_list_head 64

### number

| level | size | objects | bsp nodes | leaves | tris | object types | bytes mapped |
|---|---|---|---|---|---|---|---|
| bonus123.bif | 328682 | 14 | 108 | 109 | 1270 | {1: 1, 4: 1, 5: 1, 7: 4, 11: 3, 13: 1, 14: 1, 15: 1, 18: 1} | 13724 |
| bump.bif | 488874 | 80 | 127 | 128 | 1430 | {1: 1, 4: 1, 5: 1, 7: 30, 8: 30, 9: 1, 12: 15, 13: 1} | 19428 |
| dunk.bif | 237750 | 85 | 215 | 216 | 2359 | {1: 1, 4: 1, 5: 1, 7: 41, 8: 30, 9: 1, 12: 8, 13: 1, 19: 1} | 29760 |
| mansion.bif | 305762 | 85 | 203 | 204 | 2287 | {1: 1, 4: 1, 5: 1, 7: 42, 8: 30, 9: 1, 12: 7, 14: 1, 19: 1} | 28608 |
| maze.bif | 376982 | 75 | 283 | 284 | 3106 | {1: 1, 4: 1, 5: 1, 7: 32, 8: 30, 9: 1, 12: 7, 14: 1, 19: 1} | 37084 |
| river.bif | 406938 | 48 | 358 | 359 | 3930 | {1: 1, 4: 1, 5: 1, 7: 5, 8: 30, 9: 1, 12: 7, 15: 1, 19: 1} | 44268 |
| ski.bif | 275326 | 99 | 213 | 214 | 2427 | {1: 1, 4: 1, 5: 1, 7: 60, 8: 30, 9: 1, 12: 4, 15: 1} | 30672 |
| street.bif | 509902 | 21 | 201 | 202 | 2266 | {1: 1, 4: 1, 5: 1, 7: 3, 10: 2, 12: 10, 13: 1, 14: 1, 15: 1} | 24796 |

bytes per class: bsp_leaf 54912, bsp_node 68320, coll_list_item 96, coll_shape 160, coll_tris 76300, object 26428, object_list 2060, world_list_head 64

## Findings
- Draw and collision share one BSP per level, root right before the shape. Exactly one shape per level.
- space.bif (Letter) has a single leaf of 10 triangles as its whole collision (flying level).
- The 4th byte of every collision triangle is 0 (unused); the 8 bytes after each camera record are 0 and never read.
- Types 13/16/17 (Letter) are 13/14/15 in Number; Number adds types 11 and 18.
- Root list address of every level matches the code exactly (Letter func_80018A60, Number func_80019470).

## Not done / open
- Mesh headers for type 7/12 actors (id -> address switch in func_80023118/func_8003BB00 [N func_80023A38/func_8003D3D0]),
  the hit structure at header+0x28, camera data blocks (func_8002A120/func_8002C800), and the per-level
  .data pointer tables (534 Letter / 111 Number data-word refs in ../LEVEL_REFS_*.tsv) are not parsed. These
  are what is left in LEVEL_REFS that my structures don't cover. The meshes are the targets of the sprA morph animations.
- MAP covers only the structures above; the rest of each .bif (textures, vertices, DLs, meshes) is lvlA/lvlB territory.
