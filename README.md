# Sesame Street: Elmo's Number Journey (N64) — 100% source-reproduced

[![Byte-identical source](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=matched_code_percent&label=Byte-identical%20source)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)
[![Ordinary matching C](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=matching-c&measure=matched_code_percent&label=Ordinary%20matching%20C)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)
[![Fully linked](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=complete_code_percent&label=Fully%20linked)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)

All 973 catalogued functions and all 444,672 executable bytes are reproduced from byte-identical source. Of that code, 434,956 bytes (97.82%) are ordinary matching C; the remainder is verified original assembly or compiler/runtime source and is reported separately rather than mislabeled as C.

`make` rebuilds the whole retail ROM from this tree and checks it: code segment, main segment and the full 8 MiB ROM
are byte-identical (`number.code.sha1`, `number.main.sha1`, `number.sha1`). The base ROM is read once, by `make extract`.

| output | bytes | SHA-1 |
|---|---|---|
| build/number.code.bin (code, 0x80000400-0x8006CF00) | 444,672 | 8ecd3b155d0264f2ed08fa4cd6e57b1f097f6a50 |
| build/number.main.bin (code + RSP microcode + .data + .rodata) | 506,656 (ROM 0x1000-0x7CB20) | 5415553c7d6927cc003b5d18d36bcffa777a779e |
| build/number.z64 (the ROM) | 8,388,608 | 7195ea96d9fe5de065af61f70d55c92c8ee905e6 |

## Prerequisites (nothing here is shipped: compilers, ROM, Python packages)
- Under `TOOLROOT` (default `${N64DK_LAB}/toolroot`; override each with KMC=, IDO53=, IDO71=, BINUTILS=):
  `kmc-gcc-2.7.2/` (KMC GCC 2.7.2: gcc, cc1, KMC GNU as), `ido5.3/`, `ido7.1/` (IDO cc, static recompilation),
  `mips-binutils/usr/bin/mips-linux-gnu-{as,ld,objcopy}` (GNU binutils).
- Python 3 with `rabbitizer` (the boot/IPL3 extractor uses it), given as `PYTHON=`; GNU make >= 4.3; a host `cc`.
- GNU gzip 1.2.4 source tarball (public; SHA-1 in tools/assets/gzip124.sha1): fetched from ftp.gnu.org into
  build/tools/ by default, or given with `GZIP124_TAR=path`. It is built -DSMALL_MEM: the packer of the level files.
- Your own copy of the retail ROM (its SHA-1 must be the one in `number.sha1`).

## The two commands
    make extract BASEROM=path/to/baserom.z64     # once: ROM -> assets/ (decoded files) and extracted/data/ (PNGs)
    make -j2 PYTHON=/path/to/python3             # everything else; ends with sha1sum -c of code, main and ROM
Every directory is a variable, so a build can be made in a new directory without touching the old one:
`make BUILD=dir ASSETS=dir/extracted/assets DATA_EXTRACTED=dir/extracted/data extract BASEROM=...`, then the same
variables with `make -j2`. Nothing after `make extract` can read the ROM: the encoders and the ROM layout run under an
audit hook (tools/assets/assets.py `seal`) that refuses to open any file outside the tree, and the proof run below
also made the ROM file unreadable (chmod a-r) for the whole `make`.

Proof run (2026-11-20, fresh directory, ROM unreadable after extract):
    make PYTHON=<venv python> BUILD=build_f2 ASSETS=build_f2/extracted/assets DATA_EXTRACTED=build_f2/extracted/data \
         GZIP124_TAR=<gzip-1.2.4.tar.gz> extract BASEROM=<ROM>          72 s
    make -j2 (same variables)                                            61 s  -> code OK, main OK, number.z64 OK
(lab driver: work/pi/fresh.sh number build_f2; whole job 2:13 wall clock with extraction).

## What is regenerated from what
| ROM part | source in this tree | tool |
|---|---|---|
| code segment | src/ (921 C units, one function per file), asm/ (22 original assembly files, 52 routines) | KMC GCC 2.7.2 / IDO 5.3 / IDO 7.1 per tools/flags.tsv, KMC as / GNU as, GNU ld with number.ld |
| units' own .data/.rodata (120 units, 4,896 bytes) | the same C units (literals, jump tables, statics) | linked at their addresses |
| program .data/.rodata | data/d_*.c: typed C (33539 bytes of typed objects, 565 bytes of zero pads), display lists as gs* macros, pointers as symbols | KMC GCC; data/data.mk |
| 4 textures (11,008 bytes) | extracted/data/*.png | tools/data_tex.py -> generated C arrays |
| RSP microcode (rspboot, gspF3DEX, aspMain; 11,268 bytes) | assets/rsp/number/*.s (RSP assembly) | assets.py rspbin (g2's RSP assembler) -> build/rsp/*.bin -> data/rsp.s |
| ROM header + IPL3 | assets/boot/number/header.json, ipl3.s | g2's boot assembler (GNU as) |
| audio, songs, speech, sprites, packA-C, hud, textures, level files | assets/<owner>/ (WAV/JSON/PNG/... decoded forms) | tools/assets/owners/<owner>/tools (each owner's encoder), layout tools/assets/tables/layout_number.tsv |
| level files (gzip members) | the re-encoded level images | GNU gzip 1.2.4 -6 (SMALL_MEM) |
| header CRCs, stale padding tails | computed over the finished image | assets.py rom |

## Honest remainders (bytes that still enter the ROM as stored or literal values)
- ADPCM frame header bytes stored in assets/adpcm headers.json: 2467 bytes (the encoder does not choose them).
- ADPCM clamped frames stored as hex exceptions in assets/audio wNNN.json: 612 bytes.
- IPL3 data table as `.word` lines in ipl3.s: 1,168 bytes. RSP microcode DMEM tables as data directives
  (.word/.ascii) in gspF3DEX.data.s (2,048) and aspMain.data.s (688); the RSP text is real instructions.
- ROM header fields from header.json: 41 bytes.
- Untyped u8 arrays in data/: 708 bytes (objects whose type is not known).
- `data/rsp.s` uses `.incbin`, but only of build output (the assembled microcode); no `.incbin` of ROM bytes remains.

## Post-compile steps (stated exactly)
1. tools/trimtext.py, every object: KMC as rounds .text to 16 bytes; the original units held several functions, so
   only a unit's last function was followed by padding. With one function per file the zero padding is cut to the
   function's catalogued size (no symbol/relocation in it), .text alignment set to 4, data sections to 4. IDO objects
   built without -non_shared get their PIC/CPIC flags cleared (refused if a GOT/CALL relocation exists).
2. tools/rodata_split.py, 3 objects / 21 parts (tools/rodata_split.tsv): parts of a unit's .rodata that the
   retail .rodata holds apart (the literal order of the original multi-function units) get their own symbol, and the
   HI16/LO16 references are retargeted; symbols.ld places them. Those units' .rodata is then NOLOAD.
3. tools/datatrim.py (tools/datatrim.tsv, data/datatrim.tsv): cuts KMC as's 16-byte end padding of data sections so
   the next input lands at its address (only zero bytes with no symbol/relocation).
Not loaded from their own units (bytes come from data/): the 3 split units and 2 duplicated libultra gu statics.

## Source quality limits
- Ordinary C: 921 files. Original assembly: 22 .s files (libultra exceptasm/interrupt/rmonrcp/bzero/bcopy/sqrtf/
  setfpccsr/cache routines, KMC's 64-bit helpers, the speech codec routines, boot entry).
- One function needs an inline-asm statement: src/game/func_8004EE78.c (`__asm__("" :: "r"(e))`, an extra use of the table
  pointer so the register allocator ranks it as the original did).
- Shared headers hold one declaration per name; files still carry private struct definitions where the per-function
  sources disagree, and local casts where a file's own view gives different code (listed in the lab's castlog).
- func_8000E94C's disabled-case literal is now -1.0 as in the ROM (D_8007A050); every object's own .data/.rodata equals the
  ROM at its linked address (lab check work/pi/secdiff.py: 0 differences).

## Published progress

This project is **100% source-complete**: all 973 catalogued functions and 444,672 executable bytes are built from matching source. ByteMatch separately excludes 52 original assembly routines and one inline-assembly register-allocation workaround from its ordinary-C score.

[![Source-complete code](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=complete_code_percent&label=Source-complete%20code)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)
[![Matching C bytes](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64.svg?version=usa&category=catalogued-code&measure=matched_code_percent&label=Matching%20C%20bytes)](https://bytematch.dev/TheGh0stShip/Sesame-Street-Elmos-Number-Journey-N64/usa)

`VERIFY.tsv` preserves the strict per-function v17 evidence; `progress-inventory.tsv` and `source-accounted.tsv` bind every catalogued interval to the current conventional-project source file and SHA-256.
