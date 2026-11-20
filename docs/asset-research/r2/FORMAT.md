# r2: conflict resolution of the merged ledger (method and file formats)

`tools/run.sh` (through job.sh) runs `tools/resolve.py .`. It reads every source `*/MAP.tsv` (the same set and
the same parser as `../ledger/tools/merge_ledger.py`, imported unchanged), the code ledger, the two ROMs, the inflated
level images `../lvlA/out/<game>/*.bif.bin`, `../LEVEL_REFS_<game>.tsv`, and the conflict list
`../ledger/CONFLICTS_<game>.tsv`. It writes only offsets, counts and decoded field values: no ROM bytes.

## Outputs
| file | content |
|---|---|
| `LEDGER_<game>.tsv` | ROM 0x0-0x800000 tiling, same 9 columns as `../ledger/LEDGER_<game>.tsv`; asserted gap- and overlap-free |
| `LEVELS_<game>.tsv` | the 8 level-image tilings, same 10 columns as `../ledger/LEVELS_<game>.tsv`; asserted the same way |
| `RESOLVED_<game>.tsv` | one line per conflict pair (see columns below) |
| `CORRECTIONS_<game>.tsv` | every source-map row judged `merged` or `wrong`, with the evidence: what that agent's map should change |
| `STATS.json` | counts per evidence class and verdict, undecided list (empty) |

In LEDGER/LEVELS a row taken over from a source map keeps its text. Rows that were in a conflict get
` {r2: <status>, <evidence>}` appended to the description. Owner `r2` marks an **evidence row**: a proven object
that no source claim matched exactly. Its description says which agents claimed the bytes.

RESOLVED columns: `space, overlap_start, overlap_end, type` (as in CONFLICTS), `evidence_class`, then for each
claim `agent, start, end, kind, status, evidence`, then `verdict` (`A:<agent>`, `B:<agent>`, `evidence`,
`third claim <agent> <range>`, `naming only: both are pieces of one object`, `both zero padding: one pad`), the
`deciding_evidence`, `ledger_owner_now` (who owns the overlap in the new ledger), and `loser_still_owns`
(always `no`). A pair whose claim is no longer in its MAP gets ` [claim of X no longer in its MAP.tsv]`. A pair
that only the current maps produce gets ` [pair new in the current maps]`.

## Statuses and painting
Every claim that appears in a pair is checked on its own:
`ok` (its bounds are exactly one object, proven) > `evidence` (r2 row) > `part` (a correctly bounded piece of a
proven object: header only, a single G_VTX load, a secondary display-list entry, a 0x24-byte prefix of a model
record) > `filler` (all-zero pad) > `unverified` > `merged` (the union of several proven objects) / `wrong`
(contradicted by the bytes or the code).
The verdict of a pair is the stronger claim. Painting uses the ledger's own order inside one strength:
`wrong` -0.5 < containers -1..unknown 0 < `merged` 0.5 < `unverified` 1 < `filler` 1.5 < `part` 3 <
`evidence` 4 < `ok` 5. The script checks that no byte of a pair's overlap is still owned by a claim judged
merged/wrong when the other claim is better.

## Verifiers (one per kind; tools/resolve.py `v_<kind>`)
- **display_list** (F3DEX 1.23, `../gfx/tools/f3dex.py` opcode set): scan from the claim start. Every word pair must
  be an F3DEX command up to the first `G_ENDDL` (B8000000 00000000) or branch. One list = start up to that ENDDL.
  A claim that ends at a later ENDDL is `merged` (n lists). An entry that falls inside an earlier list with the
  same ENDDL is a secondary entry point, so it is a `part` (shared tail, e.g. after `gsSPDisplayList(0x0F000000)`
  hooks). Roots are every display-list claim of any agent plus the model records.
- **vertex**: all display lists are traced with segments. Model records set segment 3 = +0x0C and segment 4 = +0x1C.
  `gsSPSegment` (G_MOVEWORD idx 6) is also followed. The G_VTX loads of one list that touch or overlap form one
  array. Arrays of different lists join only where they overlap. Claim = array is `ok`. A single load or piece of
  an array is `part`.
- **model_header**: record valid if +0x06, +0x10, +0x18 are 0, +0x24 is 0 or header-0x18 (the bbox), and +0x08 starts a
  valid display list. The size is 0x28: Letter `func_80018530` (sesverify v16 matched C) calls
  `func_80000EDC(new, src, 10)`, a u32 copy loop, so it copies 10 words. It allocates `16*nverts + 40 + [+0x20]`
  and puts the vertex copy at new+40. A 0x24 claim or a lone +0x24 word is `part`. A "record" that fails the test is `wrong`.
- **vertex_list** (rebuild list): run the u16 program of `func_80000450` (`loop {n; if !n stop; m; if !m stop; m idx}`)
  from the claim start. The list ends after the 0 that the loop reads. Stopping at sum(n+m) is `wrong`. Taking in
  the following zero alignment is `merged`.
- **morph_anim** (sprA format): a header is valid if `hdr_end&0xFF == 8+nkey`, data_off is 4-aligned and within 16 of
  hdr_end, the pad is zero, the keys start at 0 and increase, and the last key is <= ticks. The end is taken from the
  candidates (claimed ends, ends of split pieces, the next header, the end of the ROM file without its zero
  padding). A candidate is accepted when `(end-start-data_off) = 4*nkey*nunique` with nunique a unique-vertex count
  of an animated mesh (sum of n over every rebuild list, header +0x02 - 1), or the end is the next header, or it is
  whole vertex rows up to the zero padding at the end of its ROM file. A candidate that runs over a proven display
  list or a loaded texture is rejected. ROM sub-files start 16-aligned (sprA FORMAT), so a ROM animation keeps its
  stale tail up to the boundary.
- **texture/palette**: a claim is `wrong` if it overlaps a verified pointer table (all words RAM pointers or 0) in
  program .data. Otherwise its start must be a texture reference: a traced G_SETTIMG (size from the LoadBlock /
  LoadTile / LoadTLUT that follows), a HUD TexPair texel or TLUT word (tables read from .data: Letter 0x8006DFC8 /
  DE28 / DDF0, Number 0x8006F7E0 / F798 / F760), a HUD code-built load (hud/FORMAT.md, every address checked in
  LEVEL_REFS), a code-origin load from the gfx manifest (sizes from the loading code), or a menu-file address held
  by one of lvlD's typed `-> menu22/menu21` pointer tables. A claim smaller than the load is `wrong`. A claim with
  another reference inside is `merged`. A TexPair TLUT word typed as texels, or the reverse, is `wrong`.
- **font pieces** (gfx `font` vs data1 `text/table/data`): string = printable ASCII + NUL + zero pad to 4. Bitmap[] =
  16-byte records whose +8 buf resolves. Descriptor = pointer words (12 bytes when the third word is a pointer).
  If both descriptions verify, the typed gfx row is kept (gfx extractor check: 0 mismatches, 4,237 / 4,666 items).
- **program_data**: lvlD `data_ptr_table` is `ok` when every word is a RAM pointer or 0. data2 `data` rows are
  symbol-granular, untyped `part`s.
- **collision**: BSP leaf = word0 bit0 set, +0x10 a valid display list, +0x14 / +0x1C resolve (lvlC FORMAT). A
  coll_quad is 0x50 bytes, the target of the quad pointer table. A claim containing more than one target is `merged`.
- **container**: lvlA gzip member: magic 1F 8B 08, ISIZE (last 4 bytes, LE) = inflated size. gfx's container rows add
  the slack up to the next file (`merged`). An evidence `pad` row is made for the slack.
- **code** (func_80000400 only): find `jr` + delay slot. The routine ends at 0x103C and the 20 bytes after it are
  zero words that are never run.
- **rsp**: the ucode block is whole. data1's piece is cut at the .text/.data section line (`part`).

## Evidence rows
Where every claim on a proven object is merged/wrong, an r2 row is made: display list (start to ENDDL), vertex
array, rebuild list (and its zero pad), whole animation, 0x28 model record, gzip slack pad.

## Tools
- `tools/evidence.py`: RAM -> space mapping (level image 0x80154000, common block -> ROM, main program
  RAM = ROM + 0x7FFFF400). Also the display-list scan and closure (segments, vertex/texture loads) and the
  pointer-word test.
- `tools/resolve.py`: the verifiers, verdicts, painting and outputs. `tools/run.sh`: driver for job.sh.
- `tools/probe_dl.py`, `tools/dis.py` (console disassembly of a range), `tools/peek.py` (gzip slack: counts only):
  lab survey tools used while building the rules.
