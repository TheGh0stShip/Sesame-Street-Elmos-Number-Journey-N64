# r4: end-to-end rebuild proof (formats of the outputs)

## Tools
- `tools/codebuild.py GAME OUTDIR`: compiles every function source of `sesverify/v17/<game>/src` with the lane
  and flags in VERIFY.tsv column 5. It writes the relocatable objects to `out/<game>/code/obj/func_ADDR.o` and a
  `.json` with the lane, flags and source. Resumable. All 964 + 973 build (about 6 s per game).
- `tools/rebuild_rom.py GAME OUTDIR [--redo assets,code,<owner>]`: builds the complete ROM image and compares it.
  The stages are cached in `out/<game>/cache/*.pkl`.
- `tools/probe.py`: runs another agent's extractor in its `check` direction with a ROM object (`ProbeBytes`) that
  records each equality test of a ROM slice against the bytes the extractor rebuilt. The rebuilt bytes are
  captured; ROM-vs-ROM comparisons are ignored. Every file the extractor opens is logged. Writes outside `r4/`
  go to /dev/null, so the probe never touches another agent's folder.
- `tools/dbg_link.py GAME ADDR [RAMADDR]`: diagnostics for one function's link (sections, relocations, differing words).
- Run everything through job.sh with `PYTHONPATH=<lab>/sydkit:/usr/lib/python3/dist-packages` (numpy) and the lab venv.

## How each part is rebuilt
- **Assets**: these owners are run through their check modes: audio, songs, packA, packB, packC, hud, lvlD, gfx,
  data1, data2 and ucode. sprA and speech re-encode from the ROM parse in their own check modes, so for them
  r4 calls their encoder on the extracted files instead: sprA `encode(out/<game>/*.json)` and speech
  `repack(params.json)`.
  - audio: the ADPCM re-encoder `adpcm_encode_check(pcm, book, orig)` tries the ORIGINAL frame header first. r4
    replaces `orig` with zeros, so the header comes from the WAV + JSON alone. A frame that then differs is an
    exception (its header is not determined by the extracted form).
  - packC: objects whose extracted file is a `.bin` dump (its textures/TLUTs and raw pieces) are tagged
    `extracted-verbatim`. So are ucode's microcode `.bin` blobs.
- **gzip level members** (lvlA): the header is generated (`1f8b 08 08`, mtime 0, xfl 0, os 0, FNAME = image name).
  The CRC32 + ISIZE trailer is generated from the inflated image `lvlA/out/<game>/<name>.bif.bin`. For the
  deflate stream, r4 tried zlib at levels 1-9 × memLevel 1-9 × strategies 0-4 × window 9-15, with a
  64 KB-prefix quick reject. When none reproduces the stream, the member is `container-not-reproduced` and the
  stream is an exception, while the ROM member's inflated content is proven equal to the extracted image
  (`inflated_equal` in SUMMARY).
- **Code**: the objects are linked by r4 (n64dk ELF relocation, no compiler):
  - `.text` goes at the function's real address. `func_X`/`D_X` resolve to the address in their name. Real names
    resolve from sesverify v17 NAMES.tsv, then from the kit's `sdk_symbols.txt` (a symbol table, no bytes).
  - Each own data section (.rodata/.data/...) is cut into ITEMS at the offsets .text refers to (jump table,
    literal, string, static). Each item is placed independently. The trailing zeros of a literal are kept up to
    4/8 bytes.
  - The link map tries placement rules in this order:
    1. `typed-evidence`: inside the data1/data2 typed objects whose evidence names the function, matched
       against the REBUILT data image (data1/data2/ucode/lvlD captures, never the ROM), first at/after the
       previous item.
    2. `contiguous`: an all-zero item follows the previous item of its section.
    3. `typed-evidence(content-differs)`: the function's own typed .rodata object, when the compiled literal
       differs from its bytes.
    4. `data-image-search`: a 4-aligned match anywhere in the rebuilt data image.
  - If an item's relocated words still differ from the ROM, the other candidates are tried and the function is
    flagged `resolved-by-compare(n items)`.
  - `LINKMAP_<game>.tsv`: func, method, `section+itemoffset=RAM/size` per item.
- **Padding**: ledger `pad` rows are generated as zeros. Stale non-zero bytes in them are exceptions.
- **Header**: CRC1/CRC2 are recomputed over the rebuilt image with ucode's `check_header.crc`. The other header
  fields (56 bytes) and the IPL3 (4032 bytes) have no extracted form and are exceptions.
- Each ledger row is filled from its owner's captures first, then from any other owner's (`(other-owner)`).
  The rest is an exception: the original bytes are placed, with the reason recorded.

## REBUILD_<game>.tsv
Columns: `start end bytes owner kind class source equal note`. A ledger row is split wherever the source or
the equality changes.
- `source` is one of: `rebuilt-from-extracted`, `rebuilt-from-extracted(other-owner)`, `compiled`,
  `generated-pad`, `generated` (gzip header/trailer, header CRC), `extracted-verbatim` (a `.bin` dump read
  back), `exception`.
- `note` gives the function for compiled rows and the reason for exceptions.

## SUMMARY_<game>.json
SHA-1 of the original and of the rebuilt image, bytes per source, unequal bytes per source, the exception list,
per-owner probe notes (exit code, captures, `.bin` inputs) and per-member gzip results.

The image itself is `out/<game>/rebuilt.z64` (lab only).
