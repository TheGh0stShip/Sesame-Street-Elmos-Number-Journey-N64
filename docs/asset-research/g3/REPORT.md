# g3 report: level images rebuilt from their extracted parts

All 16 inflated level images (8 per game) rebuild byte for byte from the level owners' extracted files, and the
SHA-1 of each equals the inflated original. **No exceptions remain** (0 original bytes placed).

| game | image bytes | rebuilt-from-extracted | extracted-verbatim | generated | exceptions | images equal |
|---|---|---|---|---|---|---|
| Letter | 2,624,338 | 2,606,448 (99.32%) | 14,048 | 3,842 | 0 | 8/8 |
| Number | 2,965,768 | 2,902,848 (97.88%) | 57,954 | 4,966 | 0 | 8/8 |

Generated = ledger `pad` rows (zeros) + the ELF `.comment` rows (GCC strings). No row is unequal.

## Verbatim (owner's extracted form is a raw dump, not a decoded form)
- Letter:
  - lvlE `tex_image` 12,288 and `texload_dl` 1,712 (JSON `hex`). lvlE writes a PNG for viewing, but it has no
    PNG->texel encoder.
  - lvlF `display_list` 48 (u32 word list).
- Number:
  - lvlH `vertices` 49,440 (all in river), `coll_quad` 3,600, `texture_block_unused` 2,560, `coll_quad_set` 260
    and `anim_tail` 126 (stale bytes): JSON `hex`.
  - lvlG `texload_dl` 1,920 and `display_list` 48 (hex word text).
  - lvlG bump `unknown` 0x00000 / 0x00018 (48 B, `.bin`).

Turning these into decoded forms means changing lvlE/lvlH's decoders and re-running their extracts into their own
folders. That is left to those owners. The lvlB vertex text writer is the model to follow.

## What was done to make it work
- lvlB had no lossless vertex form (805 KB Letter / 732 KB Number, only a lossy OBJ) and its model records were
  MAP-only. lvlB's extractor now writes `vtx/*.txt` and `mh/*.json` too.
- lvlB's check reassembled DLs from in-memory text and overwrote its PNG before reading it back. It now reads the
  files. It was run into g3/out/lvlB/<game>. Original: `lvlB/tools/extract_level_gfx.py.orig`.
- lvlH's check re-encoded its in-memory decode. It now reads its JSON files. Pre-edit copy:
  `extract_level_rest.py.pre-g3`.
- lvlG: the tool's classifier disagrees with lvlG/MAP.tsv on bump 0x3EB08 (MAP: mesh_bbox, tool: unknown .bin) and
  0x3EB44/0x3EB48 (mesh_header_tail). Its own `read_item()` is called on the MAP row's extracted file for the
  uncaptured or .bin-only rows (19 rows, mostly re-reads of the same hex DL text).
- The `.comment` generator: each entry is `\0GCC: (GNU) 2.7.2\0`.
- Known limits from the brief that did not matter: lvlE's crash on Number, lvlG's missing Letter table and lvlF being
  checked on Letter only. The ledger gives lvlE/lvlF only Letter rows and lvlG/lvlH only Number rows, so each tool
  ran on its own game, and every run exited 0.

## Odd things seen
- lvlB's own MAP.tsv was edited after its tool ran: a rerun gives a slightly different split at Letter streetab
  0x56230 / Number street 0x56570. The ledger is unaffected.
- r1's check also compares palette pairs with constants. lprobe orders captures so that such side tests cannot win.
- lvlC's LEVELS table gives Number river's member end as 0x2B5120, while files/ledger give 0x2B8FD0. Inflation
  stops at the member end, so the image is the same.

## Files
- `tools/rebuild_levels.py`, `tools/lprobe.py`
- `out/LEVELREBUILD_letter.tsv`, `out/LEVELREBUILD_number.tsv`, `out/LEVELSUMMARY_<game>.json`
- `out/<game>/*.bif.bin` (rebuilt images, lab only)
- `out/lvlB/<game>/` (lvlB extract output incl. vtx/ and mh/, lab only)

Run: `cd tools && ./job.sh ses_g3 3G 3000 bash -c 'cd sesassets/g3 && venv/bin/python tools/rebuild_levels.py GAME out'`
(about 2-3 min per game, after the lvlB extract).
