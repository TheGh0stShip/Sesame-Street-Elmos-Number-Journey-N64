# packA: the mode-22 menu sprite pack ("menu22")

ROM range: Letter 0x7C6B0-0xA54D0, Number 0x7CB20-0xA5940. Size 0x28E20 (167456) bytes in both.
The two games' copies are byte-identical.

Correction to `files/FILES_<game>.tsv`: this file does not hold 320x240 screens with
0x200-byte palettes at 0x12E00 strides. That layout belongs to the mode-21 file (packB).
This file holds 83 small sprites, as described below.

## How the game loads it
- Letter `func_8001D9D0(22)` (Number `func_8001E2CC(22)`) copies the whole file
  `[D_0007C6B0, D_000A54D0)` with `func_80017318` (Number `func_80017788`) to RAM 0x80154000.
  The copy is in 0x4000-byte PI DMA chunks and is a verbatim copy, with no header, compression or relocation.
- The file is a RAM image. Code and .data tables address its objects with absolute
  0x8015xxxx-0x8017xxxx pointers, so an object's file offset is its RAM address minus 0x80154000.
- Letter `func_8001A318` (Number twin `func_8001AC1C`) draws the menu. It is called from the frame builder
  `func_8000B470` (Number `func_8000BA3C`) when the mode `D_8012EFD0 == 0x16`. It uses F3DEX
  texture rectangles (`G_TEXRECT` 0xE4). TLUT mode is RGBA16 (`G_SETOTHERMODE_H` 0xBA000E02 = 0x8000), and
  each TLUT is loaded with `G_LOADTLUT` count 256 (0xF0 ... 0x073FC000), even for 16-colour palettes.

## Object encoding
Each object is a TLUT followed directly by its pixels. There is no header.
- TLUT: N big-endian u16 RGBA5551 values (R bits 15-11, G 10-6, B 5-1, A bit 0). N is 16 for CI4 and 256 for CI8.
  Index 0 is 0x0000 (transparent) in the sprites.
- Pixels: row-major, with no padding between rows.
  - CI4: 2 pixels per byte, high nibble first. The game loads it as a 16-bit block
    (`G_SETTIMG` fmt 4/16b, then `G_LOADBLOCK`) and samples it through tile `fmt I, 4b` (0xF580...), which TLUT mode indexes.
  - CI8: 1 byte per pixel (tile 0xF548..., CI 8b).

## Layout (offsets from file start; RAM = 0x80154000 + offset)
| offset | count x stride | object | referenced by |
|---|---|---|---|
| 0x00000 | 1 x 0x820 | CI4 64x64 Elmo frame, TLUT16 | nothing (table index 0 starts at +0x820) |
| 0x00820 | 22 x 0x820 | CI4 64x64 Elmo animation A | Letter .data `D_80074664[22]` {tlut, image}, drawn when `D_800745E0==0` |
| 0x0B9E0 | 28 x 0x820 | CI4 64x64 Elmo animation B | `D_80074714[28]`, used when `D_800745E0==1` and in the default case |
| 0x19F60 | 26 x 0x820 | CI4 64x64 Elmo animation C | `D_800747F4[26]`, used when `D_800745E0==2` |
| 0x271A0 | 1 x 0x600 | CI8 32x32 "123" icon, TLUT256 | nothing (Number Journey artwork, present in both games) |
| 0x277A0 | 1 x 0x600 | CI8 32x32 "ABC" icon, TLUT256 | `D_8017B7A0` (TLUT) / `D_8017B9A0` (image), drawn at 3 positions (`D_8007493C..`) |
| 0x27DA0 | 3 x 0x420 | CI4 64x32 buttons "EASY"/.../"HARD", TLUT16 | `D_800748C4[3]`, positions `D_800748DC`/`D_800748F4` |
| 0x28A00 | 1 x 0x420 | CI4 64x32 "LEVELS" banner, TLUT16 | `D_8017CA00` (TLUT) / `D_8017CA20` (image) |

Each animation frame is drawn at 64x64 (tile 0x000FC0FC). The frame index is `D_800745EE / 2`.
The pointer tables are in .data:
- Letter: RAM 0x80074664-0x8007499C (ROM 0x75264).
- Number: the same table contents at RAM 0x80075240 (ROM 0x75E40), with the same pointer values.

## Extracted form (`tools/extract_packA.py ROM GAME OUTDIR`)
- One 8-bit indexed PNG per object. PLTE is the TLUT expanded to 8 bits per channel (x*255//31), and tRNS is the alpha bit.
  The PNG pixel values are the raw CI indices.
- `packA.json` lists each object's offset, size, kind, RAM address, reference and the raw TLUT as hex u16.
- `check ROM GAME OUTDIR` rebuilds the file from the JSON TLUTs and PNG indices and compares it byte-for-byte with the ROM.
- The tool has no dependencies beyond the standard library. It includes its own PNG writer and reader, because the lab venv has no PIL or numpy.
