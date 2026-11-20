# Sample banks: libmus "N64 PtrTablesV2" + "N64 WaveTables" (both games)

The sound driver is Software Creations' libmus on libultra n_audio. Each game has two bank pairs:

| game | pair | ptr file (ROM) | wave file (ROM) | entries | used by |
|---|---|---|---|---|---|
| letter | 1 | 0x2FBD50-0x2FE8D0 | 0x2FE8D0-0x3E3FB0 | 63 | sound effects (the fx bank's wave map) |
| letter | 2 | 0x3E4830-0x3EA160 | 0x3EA160-0x4D9D80 | 124 | music (song wave maps) |
| number | 1 | 0x2E6FC0-0x2E9A90 | 0x2E9A90-0x3CB0C0 | 62 | sound effects |
| number | 2 | 0x3CB880-0x3D0980 | 0x3D0980-0x4B00C0 | 107 | music |

All values are big-endian. Offsets inside a ptr file are relative to the start of that ptr file. Sample
offsets are relative to the start of the wave file.

## How the game loads them (letter name / number name)
- Pair 1: the init function (letter `func_800170B0`, number `func_80017520`) PI-copies the whole ptr file into
  RAM (`D_800B0CA0`) and fills a config block: ptr = that RAM copy, wbk = **the ROM address** of the
  wave file. `func_8004C6FC` / `func_8004E06C` (MusInitialize) calls `func_8004D274` / `func_8004EBE4`
  (MusPtrBankInitialize) with them, which calls `func_8004E9E0` / `func_80050350` and makes this bank the
  default bank (`D_800A00FC`).
- Pair 2: before each song, `func_8001892C` (letter) / `func_8001933C` (number) allocates RAM, copies the
  ptr file, calls MusPtrBankInitialize(ptr, ROM address of wave file 2), then `func_8004D2B0` /
  `func_8004EC20` makes it the bank for the next song start.
- The wave files stay in ROM. Samples are DMAed on demand by n_audio, because relocation turns every
  sample offset into a ROM address.

## Relocation (`func_8004E9E0`, run once per file)
If bit 31 of header word 0x10 is clear, it sets that bit, then:
1. adds the file's RAM base to the three nonzero words at 0x24, 0x28 and 0x2C (`func_8004EF10`);
2. adds the RAM base to every nonzero word of the wave list;
3. for each entry i: `tune[i] = (s8)detune_byte[i] / 100.0 + (s8)(basenote[i] - 0x30)`. The f32 is written
   in place over the 4-byte detune slot;
4. for each wave-table entry whose `flags` byte is 0: if `base` is not 0xFFxxxxxx it adds the wave-file ROM
   address, sets `flags = 1`, adds the RAM base to `loop` if it is nonzero, and adds the RAM base to `book`
   if `type == 0`.

## Ptr file layout
| off | size | field |
|---|---|---|
| 0x00 | 16 | signature `N64 PtrTablesV2\0` |
| 0x10 | 4 | flags (0 in ROM; bit 31 = relocated) |
| 0x14 | 12 | 0 (not read) |
| 0x20 | 4 | count = number of wave-table entries |
| 0x24 | 4 | offset of basenote[count] (u8 each) |
| 0x28 | 4 | offset of detune[count] (4 bytes each: s8 cents in byte 0 and 3 zero bytes; replaced at run time by the f32 tune) |
| 0x2C | 4 | offset of wavelist[count] (u32 offsets of wave-table entries) |

The body starts at 0x30 and contains, for each entry in order: the wave-table entry (0x14 bytes), then
the loop record if there is one, then the ADPCM book. Each structure is 8-byte aligned, and the 4-byte
holes between them are zero. Then come the wave list, the base notes (padded to 4 bytes) and the detunes. The
tool parses every structure that a pointer reaches. All remaining bytes are zero holes, except the padding after the detune table at the end of each file
(4-12 bytes up to the 16-byte boundary). That padding holds non-zero leftover bytes that nothing reads:
letter 12 and 8 bytes, number 4 and 12 bytes. Those bytes are kept as raw regions in the JSON, and `check`
re-packs the file from the parsed fields to the exact ROM bytes.

Wave-table entry (ALWaveTable, 0x14 bytes):
| off | size | field |
|---|---|---|
| 0x00 | 4 | base: sample offset in the wave file (always 0x10-aligned, first = 0x10) |
| 0x04 | 4 | len: sample bytes |
| 0x08 | 1 | type: 0 = VADPCM (all 356 samples), 1 = raw PCM16 (not used) |
| 0x09 | 1 | flags (0 in ROM; 1 = relocated) |
| 0x0A | 2 | pad |
| 0x0C | 4 | loop: offset of the loop record, or 0 |
| 0x10 | 4 | book: offset of the ADPCM book |

Loop record (ALADPCMloop, 0x2C bytes): `u32 start, u32 end` (in sample frames, that is decoded samples), `u32 count`
(0xFFFFFFFF = endless), `s16 state[16]` (the decoder history at `start`).

ADPCM book: `s32 order, s32 npredictors`, then `order*npredictors*8` s16 coefficients. Every book in both
games has order 2 and 4 predictors (8 + 128 = 136 bytes).

## Wave file layout
`N64 WaveTables \0` (16 bytes), then the sample data. Each sample starts on a 16-byte boundary, and the 1-15
byte holes between samples are zero. Samples follow wave-list order and tile the file with no overlaps
and no sharing. After the last sample, the file is padded to the next 16-byte boundary. That padding is
non-zero leftover bytes in letter bank 1 (14 bytes) and bank 2 (8 bytes) and in number bank 1 (14 bytes);
nothing references those bytes.

## VADPCM sample data
The data is made of 9-byte frames that each decode to 16 samples. Byte 0: high nibble = scale shift s, low nibble = predictor p.
Bytes 1-8: 16 signed 4-bit residuals, high nibble first, each multiplied by 2^s. Decode each group of 8
outputs from the previous 2 outputs (`order`) and the 8 residuals:
`out[i] = clamp16( floor( (sum_k book[p][k][i]*prev[k] + 2048*r[i] + sum_{j<i} book[p][order-1][i-j-1]*r[j]) / 2048 ) )`.
The history starts at zero for each sample. All sample lengths are whole frames.

## Pitch, base notes, sample rate
The synthesizer runs at 22050 Hz (the init config passes 0x5622). The sequencer computes
`ratio = 2^((note + tune[wave]) / 12)` (`func_8004E174`, `func_8004E84C`), caps it at 2.0 and sends it to the voice.
A ratio of 1.0 plays the sample at 22050 Hz, so a wave sounds at its recorded rate at sequencer note `-tune`
(the side file calls this `unity_note`). The base note bytes in use are 0, 3, 4, 6, 12, 0xF4 (-12) and 0xFA (-6), which makes
the unity note 48, 45, 44, 42, 36, 60 or 54. The detunes are -22..+56 cents and are used only in bank 2.

## Who refers to a wave
- Sound effects: the fx bank (letter ROM 0x3E3FB0, number 0x3CB0C0; owned by agent songs) has a u16 wave map at
  header+0x14 (count at +0x08) that maps effect wave numbers to bank-1 entry indices.
- Songs: the song header word 0x20 points to a u16 wave map (count at +0x08; 0xFFFF = none, which plays as a
  rest) that maps the song's wave numbers to bank-2 entry indices (`func_8004DA5C`).

## Extractor
`tools/extract_audio.py ROM OUTDIR` writes `OUTDIR/<game>/bank{1,2}/`:
- `ptrbank.json`: the header, every region of the ptr file and the parsed fields of every entry
- `wNNN.wav`: 16-bit mono at 22050 Hz, with a `smpl` loop chunk when the entry has a loop
- `wNNN.json`: ROM offset, size, frames, book, loop record, base note, detune, tune and unity note, plus
  `exceptions` (see below)

`check ROM OUTDIR` does two things:
1. It re-packs both ptr files from the JSON and compares them byte for byte.
2. As an extra, it re-encodes every WAV back to VADPCM with the stored book and compares that with the ROM.
   For each frame, the residuals are exactly determined once the (scale, predictor) pair is chosen. The
   frames where the decoded PCM was clamped cannot be inverted (letter 34, number 68 of about 417k
   frames), so they are stored verbatim in `exceptions`.

`map ROM` prints the MAP lines. They show that the samples and the zero holes tile each wave file.
(The same script can also dump songs and the fx bank. Those formats belong to agent songs; see REPORT.md.)
