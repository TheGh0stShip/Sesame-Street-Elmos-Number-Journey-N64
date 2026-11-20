# r3 report: formats proven from code
Six structures proven from the matched C/disassembly and re-checked on every instance in both games (FORMATS_PROVEN.md has the field tables with the proving expressions; tools/check_proven.py re-validates; check_output.txt is the last run).
- Morph animations: data_off is read from the file (packer rule `((8+nkey) & ~7) + 8`, 141/141); bit packing from the top bit: z (signed 10), y (signed 11), x (signed 11): sprA, lvlD and packC decode x at the top, so their decoded coordinates and OBJ files are wrong although their repack is byte-exact; word for vertex i, key k at data_off + 4*(i*nkey + k); loop flag 0x8000; vertex count = sum of n in the mesh rebuild list (81/82 level/common, 57/59 ROM files); last key row never read in 139/141; 43 ROM animations have unread 4-12 byte tails.
- Mesh header: +0x02 read by nothing; bounding box only read by func_8000092C into a discarded copy (no effect), equals min/max of the vertices (108/108); the type-7 setup zeroes nverts*16 bytes past its heap block (9 meshes).
- Rebuild list: executed loop documented; sums to the vertex count (54/54).
- Collision quads +0x00..+0x1F: pre-built runtime header (type 3, owner, mask, plane normal and distance), read by nothing (39/39 consistent).
- BSP: draw culls on the s16 box only (skipped in levels 11 and 19); collision tests the plane only; leaf boxes exact (3066/3066); triangle byte 3 never read; draw walker reports "bsp stack blown" at depth 128, collision walker hangs at 129.
- Object record: +0x08 initial actor flags; +0x04 a full s32 for types 8, 9, 11, 18; every type id named.
Odd: Number ROM 0x79CD90-0x7AB810 (two animations) never loaded; two Number animations end before their last tick.
Caveats: Number's collision and DrawWorld twins were not read separately (data checks pass for both games); about 3% of BSP nodes have triangle centres on the unexpected side of the plane; meaning of mesh +0x02 unknown (unused).
