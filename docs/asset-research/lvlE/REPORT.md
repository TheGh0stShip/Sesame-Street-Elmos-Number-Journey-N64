# lvlE report: uncovered bytes of the Letter level images streetab, chute, manor, orchard

Scope changed mid-task: Number street/dunk/bump/mansion moved to agent lvlG; the extractor is general
(GAME argument, any level) and FORMAT.md covers the formats. A first probe showed the same classes in Number
(street holds the same three animations at +0x340; bump and mansion have many morph animations, bump 0x5DB60..
holds 9-key animations).

Territory (from `tools/gaps.py`: lvlB non-`level_data` and all lvlC ranges subtracted): 236,080 bytes.
All of it is in MAP.tsv (463 ranges, tiling every gap exactly). `extract_level_rest.py check`: **OK** (all 463
re-packed byte-exact).

| class | bytes | streetab | chute | manor | orchard |
|---|---|---|---|---|---|
| morph_anim | 206,884 | 156,128 | 19,560 | 13,276 | 17,920 |
| tex_image | 12,288 | 8,704 | 3,072 | 0 | 512 |
| tex_palette | 5,728 | 4,160 | 1,536 | 0 | 32 |
| vertex_dup_list | 3,088 | 0 | 496 | 620 | 1,972 |
| unknown | 2,560 | 0 | 0 | 2,560 | 0 |
| texload_dl | 1,712 | 1,184 | 400 | 0 | 128 |
| collider_quad | 1,600 | 320 | 0 | 0 | 1,280 |
| pad | 1,512 | 345 | 409 | 393 | 365 |
| bbox | 504 | 24 | 120 | 216 | 144 |
| model_header_tail | 84 | 4 | 20 | 36 | 24 |
| collider_ptrs | 80 | 16 | 0 | 0 | 64 |
| collider_list | 40 | 8 | 0 | 0 | 32 |

## Findings
- The "16-bit records" at streetab 0x56254.. are three keyframed vertex morph animations (sprA format) for
  the model whose header is at 0x56230: 0x56260 (51 keys, 198 ticks), 0x63764 (20 keys, 76 ticks), 0x68AF4
  (43 keys, 166 ticks). Referenced from program .data D_80075D28..30 (part of the actor anim table D_80075D00,
  func_8002AB50). The first two have 267 vertices; the third fills to 0x7C444 (466 rows by floor; its last word
  is 0, possibly one stale/pad word). Then pad and lvlB's ELF .comment.
- lvlB's model header is really 0x28 bytes: +0x24 is a bbox pointer (func_8000092C).
- Every animated model in these levels is laid out: vertex_dup_list, bbox, model header, animation(s).
- lvlB/lvlC overlap: none found in my ranges; LEVEL_REFS code addresses are shared by all levels of a game (one
  switch per actor id), so many refs that land in my ranges belong to other levels (e.g. the HUD letter tables
  D_8006E5A0..D_8006EC20 of func_80004010 are used only in mode 19); identification here is by in-image pointers.
- orchard 0x14D90 animation uses data_off 0x18 for hdr_end 0x10 (8 extra zero bytes): the header rule allows it.

## Not done / open
- manor 0x60B0..0x6AB0 (2,560 bytes) `unknown`: nothing points at it (in-image pointer scan, LEVEL_REFS). Looks
  like a 0x200-byte RGBA5551 palette (leading zero entries) + 0x800 bytes of 8-bit indices (CI8 64x32?):
  probably an unused texture left by the linker.
- Which model each streetab animation drives (and so the exact vertex count of the third) is not proved from
  the code; nvert comes from the space available.
- texload_dl groups are not traced to the code that splices them (likely texture-swap tables in .data).
