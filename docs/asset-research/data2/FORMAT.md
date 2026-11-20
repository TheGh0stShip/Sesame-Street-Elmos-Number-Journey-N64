# data2 formats: second half of .data and all of .rodata (both games)

Territory (ROM): letter 0x73400-0x791B0 (.data half) + 0x791B0-0x7C6B0 (.rodata);
number 0x73DE0-0x799C0 (.data half) + 0x799C0-0x7CB20 (.rodata). RAM = ROM + 0x7FFFF400.
Number's split point 0x73DE0 (RAM 0x800731E0) is the referenced symbol nearest the middle of its .data
(0x6EF1C-0x799C0, middle 0x7446E): D_800731E0, a 0x1000-byte object used by func_800125D4.

All objects are big-endian program data with no container format; the "format" is the C type.

| class | C type | encoding | how the game finds it |
|---|---|---|---|
| string | `const char[]` | ASCII, NUL-terminated, objects padded with zeros to 4 (rodata) | `lui/addiu` address taken (printf/assert args), or a pointer word in a .data table (`ptr@data_X`) |
| float / double | `f32` / `f64` | IEEE-754 BE | `lwc1` / `ldc1` with `%lo(sym)`; compiler literal pools, one per use, 8-aligned for doubles |
| jumptable | `void *const[]` | u32 absolute code addresses inside the switch function | `sltiu n; sll 2; lui; addu; lw; jr` (count = sltiu immediate); emitted as `func_X + 0xNN` |
| data | u8/u16/s16/u32/s32/s64 arrays | BE integers; u32 also holds pointers and struct words | loads/stores of the given width, or address taken |
| texture (letter only) | `u8[]` | tail of the I-format texture D_80072160 (0x1000 bytes, starts in data1's half) loaded with G_SETTIMG/G_LOADBLOCK in func_80012164 | display-list word |
| rsp_data | `u32[]` | RSP DMEM image: F3DEX 1.23 data (0x800 bytes) and audio microcode data (0x2B0 bytes) | `task->list.ucode_data` in func_8000B470 (L) / func_8000BA3C (N); audio task `unk_C` in func_80050DD8 (L) / func_80052748 (N). Content analysis: agent ucode |
| pad | `u8[] = {0}` | zero bytes, unreferenced | none |

Notable typed objects: s64 CORDIC arctangent table `s64[64]` (L 0x79400, N 0x79C10; func_80067CE0 / func_80068410
add or subtract tbl[i]); `_Ldtob` powers-of-ten `f64[9]` (L 0x7C4E0, N 0x7C950); OSViMode entries (0x50 each) at the start
of .rodata; level-name table `char[][20]` in letter .data at 0x76978 (level-select debug menu, func_800301C8).

## Tools (all take the ROM path; nothing from the ROM is stored in the tools)
- `tools/scan_refs.py ROM GAME VERIFY.tsv OUT` disassembles every matched function (rabbitizer) and records each
  %hi/%lo reference into .data/.rodata with access kind, plus .data/.rodata words pointing into .rodata.
- `tools/draft_types.py ROM GAME REFS OUT` builds `types_<game>.tsv` (rom_start, size, C type, name, evidence; no values):
  object boundaries = sorted referenced addresses; type from access kind; strings/zero padding by shape; hand
  corrections in its OVERRIDES / RSP_DATA tables.
- `tools/gen_data.py ROM GAME OUTDIR` emits `data2_<game>.c` with every initialiser read from the ROM;
  `gen_data.py check ...` parses that C back, re-serialises each initialiser and compares with the ROM bytes and checks
  that the objects tile the territory; `gen_data.py strings ROM GAME OUTDIR` writes `strings_<game>.txt`.
- `tools/make_map.py MAP.tsv`, `tools/run.sh scan|draft|gen|check|strings` (drives both games).
