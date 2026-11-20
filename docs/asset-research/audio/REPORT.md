# Audio (sample banks): report

Scope (after the coordinator's re-split): the libmus ptr files and wave files of both games. The songs, the fx
bank and MIDI belong to agent songs. The GSM speech belongs to agent speech.

## Status
- MAP.tsv tiles the bank territory with no gaps: letter 0x2FBD50-0x3E3FB0 and 0x3E4830-0x4D9D80; number
  0x2E6FC0-0x3CB0C0 and 0x3CB880-0x4B00C0. The two holes are the fx banks (agent songs). The ranges match
  FILES_<game>.tsv.
- tools/extract_audio.py extracts all 356 samples to WAV and JSON (letter 187, number 169, the same counts as the
  kit). `check` passes for both games: the ptr files re-pack byte for byte, and every sample also re-encodes
  to its ROM bytes. The clamped frames are stored verbatim: letter 34, number 68 frames.
- The kit's "libmus" guess is confirmed by the signatures and by the code (func_8004E9E0 = MusPtrBankInitialize
  relocation).

## Bytes by class
| game | class | lines | bytes |
|---|---|---|---|
| letter | ptr files | 2 | 33,968 |
| letter | wave-file headers | 2 | 32 |
| letter | VADPCM samples | 187 | 1,920,222 |
| letter | pad: zero holes | 176 | 1,516 |
| letter | pad: non-zero end fill | 2 | 22 |
| number | ptr files | 2 | 31,696 |
| number | wave-file headers | 2 | 32 |
| number | VADPCM samples | 169 | 1,837,008 |
| number | pad: zero holes | 158 | 1,394 |
| number | pad: non-zero end fill | 1 | 14 |

Letter has 63+124 entries with 9+38 loops, and number has 62+107 entries with 9+54 loops. Every book is order 2 with 4
predictors. Every sample is VADPCM. Detunes are nonzero only in bank 2.

## Unused samples (from the wave maps)
- Bank 1 (effects), entries that the fx wave map does not list. Letter: 3, 7, 26. Number: 1, 7, 16, 22.
- Bank 2 (music), entries that no song wave map lists. Letter: 28, 33, 47, 54, 78, 79. Number: 19, 28, 29, 30, 42,
  43, 57, 58, 74, 76, 78, 79, 86, 98, 106.
- Letter bank-2 entries 13, 14 and 48 are used only by songs that no code starts (0x4DD320, which appears only as an
  end bound, and the files at 0x2F9B60 and 0x50F9E0).

## Odd things
- Non-zero leftover bytes from the build tool fill the padding at the end of the ptr files (4-12 bytes) and of the
  wave files (letter 14 and 8 bytes; number bank 1 14 bytes). The song files show the same thing.
- The number sources reuse letter symbol names for ROM addresses (`D_002FBD50` in number's init means ROM
  0x2E6FC0). Use the signatures and VERIFY.tsv instead.

## Handed to agent songs (done before the re-split)
extract_audio.py also dumps songs and the fx bank and re-assembles them (`check` OK for all of them), and writes
approximate MIDI (out/<game>/song*.txt|.mid, fxbank.txt). The full command table (0x80-0xAC, 45 handlers) is
in the docstring/OPS table of the script. Song header: version 0x215, tracks, waves, and 7 offsets (channel,
volume, bend, envelope, drum, wave map, master). The script times songs at 48 ticks per beat. Number picks
effect ids at random from the .data tables {10,11,12,13,15}, {39,40} and {13,15,20}.
