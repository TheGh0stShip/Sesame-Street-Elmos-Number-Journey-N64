# Level (.bif) non-graphics structures (Letter and Number)

A level is a gzip member (FNAME `<name>.bif`) inflated by func_80013560 [Number func_800139D0] into RAM at
**0x80154000** (buffer 0x86800). It is a ready-made RAM image: every pointer is an absolute RAM address
(file offset = address - 0x80154000), so nothing is relocated. The game finds its roots through addresses hard-coded
in the code for each level (see ../LEVEL_REFS_<game>.tsv). All values are big-endian. In the JSON, pointers are
written as file offsets (`null` = 0).

## Object list and records (actor placement)
- Root: a 0-terminated array of u32 record pointers. Its address is passed by the level-start code
  (Letter func_80018A60, Number func_80019470) to the dispatcher func_8001F300 [N func_8001FBE0], which switches on
  `record.type` and calls the handler. It comes right after the last record. List addresses (all match the code):
  Letter streetab 0x801A9F5C, chute 0x801934D4, manor 0x8019411C, orchard 0x8019906C, garden 0x801AC70C,
  space 0x8018FD80, planet 0x801A87FC, bonusabc 0x801650D4; Number street 0x801AA2A4, dunk 0x8017D82C,
  bump 0x80174B50, mansion 0x80177D44, maze 0x8018F46C, ski 0x80176BAC, river 0x80197E10, bonus123 0x80167F40.
- Record, 0x34 bytes (the handlers copy the u16 at +2/+6, the s32 at +8, then the floats):

| off | type | field |
|---|---|---|
| 0x00 | u32 | type (handler is chosen by this, table below) |
| 0x04 | s32 | id. Most handlers use the low u16 (+6 -> actor+0xA). Read as s32 by type 8 (pickup slot 1..30, else 1), type 9 (float arg of func_80049ABC), type 11 (switch); type 18 writes 0x6000 here |
| 0x08 | s32 | initial actor flags (copied to actor+0xC): 0x80 = hold the last anim tick instead of wrapping (func_800272C8); bit 1 remove; bit 8 anim finished. Type 9 instead splits it (/100000000, %10000) |
| 0x0C | f32[3] | position |
| 0x18 | f32[3] | forward vector (normalised by the handlers) |
| 0x24 | f32[3] | up vector |
| 0x30 | u32 ptr | world list head (same for every record of a level) |

  The type-5 (camera) record is followed by 8 bytes that nothing reads (0 in all 16 levels); the tool keeps them as
  `unused_tail` (record size 0x3C).
- Type -> handler (= "the actor type table"; it is a switch in the dispatcher, not a data table):

| type | Letter | Number | what |
|---|---|---|---|
| 1 | func_80021110 | func_80021A20 | Elmo / player start (elmoact.c) |
| 4 | func_80024E20 | func_80026110 | world: links the level coll_list_item into the world list head (worldact.c) |
| 5 | func_8002A120 | func_8002C800 | camera (cameraact.c) |
| 7 | func_80023118 | func_80023A38 | character actor (muppetact.c); switch on id -> mesh header address in the level or common block |
| 8 | func_800327E0 | func_800347CC | pickup (letters/numbers; 30 per main level) |
| 9 | func_80022C6C | func_80023594 | effect emitter (func_80049ABC) |
| 10 | func_80034CD0 | func_80036A90 | interaction point (switch on id; id 2: player near + button -> mode 22, player state 35) |
| 11 | - | func_800408D0 | Number only; id 0x5001/0x5003/0x5005/0x6000 picks a data block |
| 12 | func_8003BB00 | func_8003D3D0 | map object (mapobjects.c); switch on id -> mesh header + a hit structure at header+0x28 |
| 13/17/16 | func_80026ABC/AF8/B34 | 13/14/15: func_80027F2C/F68/FA4 | speaker, func_80026B70 [func_80027FE0]: Letter 13->bank 0, 17->1, 16->2; Number 13->0, 14->1, 15->2 |
| 18 | - | inline in func_8001FBE0 | Number: writes s32 0x6000 into rec+4, then calls the type-11 handler func_800408D0 |
| 19 | func_80030D8C | func_80033368 | proximity trigger: player within dx²+dz² < 300000 -> player state 0x22, removes itself |

## Collision / draw world
- world list head (8 bytes): u32 first, last; 0 in the file, filled by func_800445D0 (cslist.c) at load.
- coll_list_item (12 bytes, directly after the shape): u32 next, prev (0), shape ptr. Its address is hard-coded in
  the world handler.
- coll_shape (0x14): u32 type=12 (BSP mesh; the walker func_80041230 also knows types 1 sphere, 3, 13),
  u32 0, u32 collision mask (0x80000000, ANDed with the mover's mask), u32 0, u32 BSP root.
- BSP: one tree is both the draw tree (DrawWorld func_80025E4C, "bsp stack blown") and the collision tree
  (func_80044B90 -> func_80044E3C). DrawWorld ignores the plane and culls with the box only (no culling in levels 11/19); it
  prints "bsp stack blown" at depth 128. Collision ignores the box; it spins forever at depth 129. A leaf's word0 becomes
  the hit-surface id D_80115378.
  - word0: `index << 16 | flags`, flags bit0 = leaf.
  - +0x04 s16[6] bounding box: xmin, xmax, ymin, ymax, zmin, zmax (frustum test func_80025504/func_80025134).
  - node (0x28): +0x10 f32 plane nx, ny, nz, d (the test uses (x, z, -y) of the mover, i.e. the game's y/z swap);
    +0x20 front child (n.p - d >= 0, p = mover (x, z, -y)), +0x24 back child.
  - leaf (0x20): +0x10 display list (gfx), +0x14 vertex array (F3DEX Vtx, 16 bytes; collision reads s16 x,y,z),
    +0x18 u32 triangle count, +0x1C triangle array.
- coll_tris: per triangle u8 i0, i1, i2, pad (pad is 0 everywhere and never read) indexing the leaf's vertex array.

Layout in the file: tris/vertices/DLs/leaves/nodes interleaved per leaf, then the root node, the shape, the list item,
the list head, the records, and the object list.

## Not parsed here (pointed at, owned by graphics / others)
Display lists, vertex arrays, textures (lvlB). Mesh headers used by types 7/12 (0x28 bytes: +0 u16 flags, +2 u16 unread, +4 u16 vertex count,
+0xC vertex source, +0x10/+0x18 runtime buffers, +0x20 extra size; copied by func_80018530) and the hit structure at
header+0x28 used via actor+0x1BC (func_80026B70, func_80032818, func_8003E3E0); camera data blocks referenced by
func_8002A120 [N func_8002C800].
