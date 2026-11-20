# Formats: ROM header, IPL3, RSP microcode (agent ucode)

## N64 ROM header (ROM 0x00-0x40), big-endian
| off | size | field |
|---|---|---|
| 0x00 | 4 | PI BSD DOM1 timing word (latency/pulse/page/release) used by IPL to configure the cartridge bus; also the z64 byte-order magic |
| 0x04 | 4 | clock-rate override (low bits; 0 = default). Both games: small nonzero value (printed by the tool) |
| 0x08 | 4 | boot entry point: RAM address IPL3 copies 1 MB of ROM from 0x1000 to and jumps to (both: 0x80000400) |
| 0x0C | 4 | libultra release word; low byte is the SDK revision letter (both: 2.0I) |
| 0x10 | 4 | CRC1 (see checksum below) |
| 0x14 | 4 | CRC2 |
| 0x18 | 8 | reserved, zero |
| 0x20 | 20 | internal name, ASCII, space padded (truncated game title) |
| 0x34 | 7 | reserved, zero |
| 0x3B | 1 | media category, ASCII ('N' = cartridge) |
| 0x3C | 2 | game id, ASCII two letters (differs per game) |
| 0x3E | 1 | destination/region, ASCII ('E' = North America) |
| 0x3F | 1 | revision (0) |

## IPL3 (ROM 0x40-0x1000, 4032 bytes)
Executed from SP DMEM after the PIF boot ROM. Identified by crc32 of the 4032 bytes: both games carry the standard
**CIC-NUS-6102** (= 7101) IPL3, byte-identical between the two games (same crc32, printed by `tools/check_header.py`).
Checksum for 6102: seed `0xF8CA4DDC` into t1..t6, then for each big-endian word d over ROM 0x1000..0x101000 (1 MiB):
`t6' = t6+d` (if it wrapped, t4 += 1); `t3 ^= d`; `r = rol(d, d & 31)`; `t5 += r`; `t2 ^= r if t2 > d else (t6' ^ d)`;
`t1 += t5 ^ d`. CRC1 = t6^t4^t3, CRC2 = t5^t2^t1. (Tool also implements the 6103/6105/6106 variants.) Both games MATCH.

## RSP microcode
Found from the OSTask builders (letter `func_8000B470` graphics task, `func_80050BC0` audio task, ucode/data pointers
given by `func_80050DD8`; number `func_8000BA3C` and its libmus siblings). OSTask layout: +0x10 type (1 gfx, 2 audio),
+0x18 ucode_boot, +0x1C boot size (= text_start - boot_start: the boot stub is followed directly by F3DEX text),
+0x20 ucode, +0x24 ucode_size (audio task: 0x1000 = whole IMEM), +0x28 ucode_data, +0x2C data size (both tasks: 0x800,
an upper bound; the audio data is really 0x2B0). `osSpTaskLoad` (letter `func_800620DC`, number `func_80063A4C`) DMAs
the boot stub to IMEM; the stub then loads ucode/data. Text sections are raw RSP instruction words for IMEM 0x04001080;
data sections are DMEM images. No compression.

Layout (RAM, both games contiguous): `rspboot` (0xD0) | `gspF3DEX` text (0x1430, ends with the 0xBEEF tail marker) |
audio text (0xC54) | zero pad to 16 | game .data ... and in .rodata: `gspF3DEX` data (0x800, contains the ASCII id string
(naming F3DEX 1.23, at +0x2B0) | audio data (0x2B0) | game strings.
Note the survey's .text/.data boundary (letter 0x8006C9AC, number 0x8006E31C) falls inside the audio microcode text;
the RSP blobs are not R4300 code at all.

`tools/extract_ucode.py ROM OUTDIR [--check]` writes `rspboot.text.bin`, `gspF3DEX.text.bin`, `gspF3DEX.data.bin`,
`aspMain.text.bin`, `aspMain.data.bin` and `ucode.json` (offsets, sizes, sha1). The build is recognised by the ROM
position of the F3DEX id string; `--check` re-reads and compares (lossless; the files are the raw bytes).

## Call graph TSV (`out/callgraph_<game>.tsv`, from `tools/callgraph.py ROM GAME functions.json SRCDIR OUT`)
Columns: func, cls (game/sdk/handwritten/boot), size, callers via jal or j (tail call), functions that load its address
with lui+addiu/ori, .data/.rodata word addresses holding its address, `YES` if none of these, first comment of its
matched source.
