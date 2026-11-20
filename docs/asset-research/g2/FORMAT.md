# g2: formats of the re-owned classes (both games)

All tools take the ROM path as an argument, have `extract` and `check` modes, and are run by `tools/run.sh`
through job.sh. Outputs are under `out/` (lab only).

## 1. VADPCM frame headers (`tools/adpcm_g2.py`, `out/adpcm/<game>/headers.json`)
A frame is 9 bytes: header `scale<<4 | predictor`, then 16 signed 4-bit residuals (audio/FORMAT.md). Given the PCM
(audio's WAV) and the header, the residuals are exactly determined, so the header choice is the only extra information.
- `choose()` is a line-by-line port of the SDK `vadpcm_enc` frame encoder (`vencodeframe`, `inner_product`,
  `qsample`, `clamp`, `clip` from the public reimplementation n64decomp/sdk-tools `adpcm/`). Predictor search: for
  each predictor, the unquantised prediction error over 16 samples (the second half is predicted from the
  unquantised input), the f32 sum of squares, strict `<` (lowest index wins ties). Scale: the 16 errors are rounded and
  clamped to s16; `max` = the first value of largest |e| (SDK bug kept); halve with C truncation until it lies in
  [-8, 7]; then the quantise loop (`qsample` = round half toward zero with the 0.4999999 epsilon, clip to [-8, 7],
  retry once with scale+1 if a residual had to be clipped by 2 or more, scale capped at 12). Encoder state = the
  unclamped decoded previous frame.
- Header rule used by the generator, per frame: (a) the SDK choice, if its header reproduces the frame's PCM
  exactly; otherwise (b) the first header in the order scale 0..12, predictor 0..3 that reproduces the PCM; unless
  (c) `headers.json` `samples[<bankN/wNNN>][<frame>]` gives the header (hex) explicitly. Residuals are then the exact
  inverse of the decoder for that header. Frames listed in audio's `wNNN.json` `exceptions` (decoded PCM clamped, not
  invertible) are still taken from there.
- Result: the SDK algorithm run on the decoded PCM chooses the ROM header for 200,634 / 213,324 (Letter) and
  190,871 / 204,044 (Number) frames. The rest differ (Letter 5,301 by scale only, 7,389 by predictor; Number 4,991 /
  8,182) because the original encoder saw the 16-bit source audio, not the decoded PCM: decoding is lossy, so the
  source's unquantised error (which drives predictor and scale) cannot be recovered. With rule (b) behind (a), only
  2,771 (Letter) / 2,467 (Number) frames need the explicit header field.

## 2. ROM header and IPL3 (`tools/boot_g2.py`, `out/boot/<game>/header.json`, `ipl3.s`)
- `header.json`: 13 fields (offset, size, name, kind u32/u8/ascii/zero/crc, value). `crc1`/`crc2` carry no value:
  they are recomputed with the CIC-6102 checksum (seed 0xF8CA4DDC over ROM 0x1000-0x101000; see ucode/FORMAT.md).
- `ipl3.s`: GNU as source for 0xA4000040-0xA4001000 (`.set noreorder/noat/nomacro`), rabbitizer 1.16 disassembly with
  pseudo-instructions off, labels `L_<addr>` on all 56 internal branch/jump targets, jumps outside the block as
  absolute addresses. 716 instruction words (0x40-0xB70), then `ipl3_data:` 0x490 bytes (0xA4000B70-0xA4001000) as
  `.word` lines (the first non-instruction word marks the start; that is the 6102's data table). Build:
  `mips-linux-gnu-as -EB -march=vr4300 -mabi=32` -> `ld -T` (text at 0xA4000040) -> `objcopy -O binary`
  (toolroot/mips-binutils; needs LD_LIBRARY_PATH=.../usr/lib/x86_64-linux-gnu, set by the tool). Identical in both games.

## 3. RSP microcode (`tools/rsp_g2.py`, `out/rsp/<game>/<blob>.s`)
No armips/bass is installed, so the tool is a table-driven RSP disassembler + two-pass assembler.
- Scalar: SPECIAL (sll srl sra sllv srlv srav jr jalr break add addu sub subu and or xor nor slt sltu), REGIMM
  (bltz bgez bltzal bgezal), j jal beq bne blez bgtz addi addiu slti sltiu andi ori xori lui, lb lh lw lbu lhu sb sh sw,
  COP0 mfc0/mtc0 (`$cN`), COP2 mfc2/mtc2 `$rt, $vS[e]` (element in bits 10..7), cfc2/ctc2 `$rt, $vcN`.
- Vector (COP2, bit 25): `op $vD, $vS, $vT[e]` for all 47 functs (vmulf..vnop); all four fields are always written,
  so even vrcp/vmov-style ops round-trip without special cases.
- LWC2/SWC2: `l|s` + {bv sv lv dv qv rv pv uv hv fv wv tv} `$vT[e], off($base)`, off = 7-bit field x access size
  (1,2,4,8,16,16,8,8,16,16,16,16).
- Directives: `.imem ADDR` (rspboot 0x04001000, F3DEX and audio text 0x04001080), `.dmem ADDR`, labels `L_xxxx`
  for branch/jump targets inside the blob (other targets numeric; F3DEX overlays make some targets lie outside the
  linear image), `.word`, `.byte`, `.ascii "json-escaped"`. Data blobs: printable NUL-terminated runs of 8+ chars as
  `.ascii` (the F3DEX id string), the rest as `.word` (4 per line).
- The disassembler re-encodes each line and falls back to `.word` if it does not round-trip: 0 such words in all
  five blobs. Cross-check: every non-nop mnemonic equals rabbitizer's RSP decoder (2,005 words in Letter's three texts).

## 4. packC textures and TLUTs (`tools/tex_g2.py`, `out/tex/<game>/*.png`, `index.json`)
Territory = r4 REBUILD rows (owner packC, class texture/tlut, source extracted-verbatim). Format and size come from
packC's own display-list walk (imported from packC/tools/extract_packC.py, `Pack.objs[..]['img']`): tile 0 SetTile =
render format, tile 0 SetTileSize = width/height, LoadTLUT = palette. All 92/88 textures got their size from a display
list; none has bytes beyond w*h*bpp. Textures: CI as indexed PNG (PLTE+tRNS = the TLUT used, for viewing only),
RGBA32 as RGBA PNG. TLUTs: RGBA16 colour strips, 16 colours per row (n64tex RGBA16 -> RGBA8 is invertible).
Codec: gfx/tools n64tex.py + png.py. `index.json` per item: rom range, RAM, class, fmt, bits, w, h, tlut_ram,
size_from, refs, tail_bytes (always 0).

## 5. Padding leftovers (`tools/leftover_g2.py`, `out/leftover/LEFTOVER_<game>.tsv`)
Every stale pad exception sits between a file's end and the next 16-byte boundary, after a gzip member trailer (8 per
game), a sample-bank wave file / fx bank end (2 Letter, 1 Number) or a speech clip (29 / 44). The whole tail, zero bytes
included, equals the ROM bytes 0x2000 earlier: the mastering tool wrote files through an 8 KiB buffer and padded the last
block to 16 bytes with the buffer's stale contents. Row columns: game, start, end, bytes, nonzero, class `leftover`,
rule `copy:-0x2000`, source range, the file it follows, note. Generator: `out[x] = image[x - 0x2000]`. Not every file
tail does this (most are zero), so the rule is per row, not global. The TSV holds no ROM bytes.
