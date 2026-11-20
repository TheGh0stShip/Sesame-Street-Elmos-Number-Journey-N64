# speech: report

## Done
- Speech container fully identified (FORMAT.md): "MORT" streams, 12-byte header, LSB-first bit stream of run
  headers (silence runs / coded runs) and standard 260-bit GSM 06.10 frames, 16 kHz, 160 samples/frame.
- `tools/extract_speech.py` decodes every clip to WAV with a Python GSM 06.10 decoder (standard algorithm,
  same constants as `func_80069920` etc.) and dumps every frame parameter to JSON.
  `check` mode: **Letter 260/260, Number 210/210 streams re-pack byte-identical** (header, runs, frames, zero tail).
- Decoded audio sanity: Letter 431.6 s, Number 486.2 s of speech; RMS median ~7400, i.e. normal speech levels.
- `CLIPS_letter.tsv` / `CLIPS_number.tsv`: id, ROM range, frames, rate, duration, every .data phrase-table
  entry holding the clip's offset (with the ms wait), the table symbol and the functions naming it.
- MAP.tsv tiles Letter 0x4D9D78-0x61AFB0 and Number 0x4B00C0-0x61F510 with no gaps (checked by script).

## Bytes per class
| game | class | ranges | bytes |
|---|---|---|---|
| letter | speech (MORT) | 260 | 1,089,224 |
| letter | libmus songs | 17 | 224,400 |
| letter | pad (zero) | 183 | 1,512 |
| letter | pad (stale non-zero filler) | 29 | 240 |
| letter | unknown | 1 | 8 (tail of the file starting at D_003EA160) |
| number | speech (MORT) | 210 | 1,267,656 |
| number | libmus songs | 19 | 235,344 |
| number | pad (zero) | 109 | 928 |
| number | pad (stale non-zero filler) | 44 | 408 |

## Who plays what
- Letter: almost all clips sit in per-letter tables used by `func_80036DE8`, `func_8002F274`, `func_800377FC`,
  `func_800272C8` (letter name / word slots `D_80115258`, `D_8013E680` substituted into phrase templates such as
  `D_80073708`), plus `func_8002AB50` (67, idle/hint phrases `D_800733C8`...), `func_800336BC` (43),
  `func_8003AB7C` (14, intro e.g. `D_800732F0`), `func_80036068`, `func_8001A12C`.
- Number: `func_800287AC` (79), `func_8002D0A4` (35), `func_8001AC1C` (32), `func_800356F0` (25), `func_8003B620`,
  `func_8003C494`, `func_80018CB0/E08` (number-name tables) and ~20 more (see CLIPS_number.tsv).
- No clip offset is built in code: the C sources reference no D_00 symbol inside either speech region.

## Unused
- Clips never referenced by any table: Letter 4: 0(0x510A10-0x51195C, 1.260s) 9(0x51BB10-0x51DEF4, 3.520s) 83(0x5B0780-0x5B2464, 2.400s) 84(0x5B2470-0x5B415C, 2.800s) 
- Number 19: 0(0x4E9810-0x4EA75C, 1.260s) 9(0x4F4910-0x4F6CF4, 3.520s) 30(0x513FC0-0x519EE0, 9.150s) 31(0x519EE0-0x51B920, 2.580s) 32(0x51B920-0x51E5E8, 4.260s) 33(0x51E5F0-0x5208F4, 3.280s) 38(0x529470-0x52DFAC, 8.800s) 41(0x535B80-0x537378, 2.680s) 50(0x54E2B0-0x5525D4, 6.100s) 51(0x5525E0-0x5553CC, 4.140s) 52(0x5553D0-0x55A7FC, 7.890s) 53(0x55A800-0x55F300, 6.790s) 65(0x580850-0x5839D0, 4.810s) 79(0x59DF40-0x5A15F0, 5.170s) 85(0x5AD740-0x5B0654, 4.920s) 87(0x5B2D40-0x5B5024, 3.580s) 89(0x5B6BC0-0x5B9214, 3.730s) 90(0x5B9220-0x5BBE48, 4.240s) 114(0x5CF860-0x5CFE08, 0.710s) 
- Letter songs never started by code: 0x4DD320-0x4E32C0 and 0x50F9E0-0x510A10 (2 of 17). Number: all 19 used.
- `func_80069C00` (byte-packed GSM frame unpacker) has no caller.

## Odd
- Clips 0..18 of both games have identical headers (same recordings shared between the two titles; clip 0 and 9
  unused in both).
- Stale non-zero filler after some streams (left over from the packer's buffer), never read.
- Decoder: only the parameter path is proven bit-exact; the PCM matches the standard GSM decoder, the game's
  hand-written assembly (0x8006AD90-0x8006B780 in Letter) was not emulated instruction by instruction, so tiny
  saturation differences are possible.

## Not mine / leads
- Number 0x739C20-0x75F380 and 0x79CD90-0x7AB810 (survey "unknown") are image files: header u16 width, u16 height,
  then a per-row offset table (looks like an RLE sprite format; Letter 0x61AFB0 starts the same way). Referenced by
  `func_800287AC`/`func_80027FE0` through `D_80106C74`/`D_800A7A50`. Left to the graphics/files agent.
- The libmus songs are listed in MAP.tsv with exact boundaries but are extracted by the audio agent.
