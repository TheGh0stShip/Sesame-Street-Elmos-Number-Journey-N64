# "sprite_anim" files = character vertex (morph) animations

Not sprites, not images, not audio. Each file is a keyframed vertex-position animation for one
character mesh. The "u16 width, u16 height" guess was the header's two offsets.

## How the game uses them (Letter addresses; Number has twins)
- A script routine (Letter func_800272C8 / func_80026B70; Number func_800287AC / func_80027FE0)
  sets D_80106874 = file ROM start and D_800A75E0 = end - start.
- The loader thread func_80026920 allocates a buffer (func_80040400), DMAs the file in 0x4000-byte
  chunks (func_8005EE10, PI DMA), stores the buffer in D_801180A4 and clears D_80106874 when done.
  It only copies bytes; nothing is decoded there (so it is not a speech/audio stream, even though the
  same thread and the speech system run together: these clips play alongside speech scenes).
- The script copies D_801180A4 to actor->unk_1BC (anim pointer); actor->unk_1CC is the current tick,
  compared against byte 4 (tick count) to stop/loop; the old buffer is freed with func_80040680.
- Per frame func_8000092C(mesh, out, anim, tick, ...) finds the keyframe pair around `tick` in the key
  table and calls func_80000450(mesh, anim, keyIndex, dstVerts, tickInSegment), which writes s16
  x,y,z into the mesh's 16-byte vertices (Vtx), linearly interpolating between key k and k+1:
  v = a + (b - a) * t / (key[k+1] - key[k]) (s16 multiply, integer divide).
- The vertex order is driven by the mesh's u16 list (mesh+0x14): repeated {n, then n vertices taken
  sequentially from the animation; m, then m u16 indices of already-written vertices to duplicate},
  ending with 0. So the vertex count of an animation equals the mesh's count of unique vertices.
  There is no palette and no pixel data.

## Layout (big-endian)
| off | type | field |
|---|---|---|
| 0 | u16 | hdr_end = 8 + nkey (end of key table; not read by the decoder) |
| 2 | u16 | data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141); "round hdr_end up to 8" is wrong for 20/141 |
| 4 | u8  | ticks: animation length in ticks (actor tick counter limit) |
| 5 | u8  | nkey: number of keyframes |
| 6 | u16 | flags: 0x8000 = looping (when the segment is the second-to-last, k = nkey-2, the "next" key is key 0 instead of key nkey-1); other bits 0 in all files |
| 8 | u8[nkey] | key tick numbers, strictly increasing, first 0, last <= ticks (usually == ticks) |
| hdr_end | zero bytes | pad to data_off |
| data_off | u32[nvert][nkey] | vertex-major: word for vertex i, key k at data_off + (i*nkey + k)*4 |
| ... | 0-3 words | file padding to a 16-byte boundary; contents are stale (non-zero, vertex-like) packer output, never read |

Vertex word: z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1).
(GCC MIPS big-endian bitfield `struct { s32 z:10; s32 y:11; s32 x:11; }` declared in func_80000450).

nvert is not stored; it comes from the mesh. The extractor uses floor((size - data_off) / (4*nkey)),
which gives the mesh counts exactly (Letter 380/346/291, Number 380/339/309/214) since the slack is
always < one vertex row.

## Files packed together
Number's file table (code symbols) groups some ranges that hold two or three animations back to back:
0x739C20-0x75F380 holds anims at 0x739C20, 0x7470D0, 0x758920; 0x79CD90-0x7AB810 holds 0x79CD90,
0x7A7F10. Each sub-file starts at a 16-byte boundary and has the same header. The extractor splits a
range at every 16-aligned offset that carries a valid header (hdr_end == 8+nkey, data_off rule,
first key 0, keys increasing, last key <= ticks).

## Extractor
`tools/extract_sprites.py ROM GAME OUTDIR [check|map]` (GAME letter|number; reads ../../FILES_<game>.tsv).
- extract: OUTDIR/GAME/XXXXXX.json (all header fields, key ticks, header pad and tail bytes, vertices
  per key) and one OBJ point cloud per keyframe (XXXXXX_kNN_tTTT.obj; meshes' faces live elsewhere).
- check: repacks the JSON form and compares with the ROM bytes: byte-exact for every file of both
  games. Encoder choices that are not derivable from the vertices (stale tail words) are stored verbatim.

## Corrections from r3 (r3/FORMATS_PROVEN.md §1)
- The streaming thread in Number is func_80027D90 (size D_800A7A50, ROM address D_80106C74, buffer D_80118244).
- When keys[-1] == ticks (139/141 anims) the last key row is never read; in loop anims the last row is dead data (it is not key 0).
- tick >= ticks leaves the mesh unrebuilt. Interpolation truncates the difference and the product to s16, divides toward zero.
- Number 0x6482F0 and 0x758920 have keys[-1] < ticks and take the "Frame passed last key!" path.
- Number 0x79CD90-0x7AB810 (2 anims, 214 vertices) is unreferenced: D_0079CD90 is only the end of the 0x78E0B0 clip.
