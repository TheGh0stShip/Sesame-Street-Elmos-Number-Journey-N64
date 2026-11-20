# Speech streams ("MORT" GSM 06.10) and the adjacent libmus song block

## Where
| game | speech region | clips | preceding songs |
|---|---|---|---|
| Letter | 0x510A10-0x61AFB0 | 260 | 0x4D9D80-0x510A10 (17 songs) |
| Number | 0x4E9810-0x61F510 | 210 | 0x4B00C0-0x4E9810 (19 songs) |

There is no clip directory in the ROM. Every reference to a clip is a **ROM offset stored directly in a
phrase table in .data**. Clips start on 16-byte boundaries and are laid end to end; the gap between
a stream's end (header word count * 4) and the next 16-byte boundary is filler (usually zero, sometimes
stale bytes, never read).

## Phrase tables (how the game picks a clip)
`struct SndCmd { u32 rom; s16 ms; u16 pad; }` (8 bytes), arrays terminated by `ms == 0`.
- The sound sequencing thread (Letter `func_80012FA0`) walks the list held in `D_80115374`:
  - `rom == -1`: pause `ms` milliseconds;
  - `ms == -1` / `ms == -2`: substitute the entry pointed to by `D_80115258` / `D_8013E680`
    (slots filled per letter/number, e.g. `func_80036DE8` picks `D_80073EE0[letter]`, `D_80073FF0[...]` etc.);
  - otherwise stop the previous voice (`func_80051418`) and start `rom` (`func_80051314`), then wait `ms`
    (timer of `ms * 46875` CPU counts = ms at 46.875 MHz).
- Game code sets `D_80115374` to a table (e.g. `D_800732F0`) and wakes the thread. `CLIPS_<game>.tsv` lists, per
  clip, every table entry that holds its offset (`tables` column = `symbol+byteoffset`, entry index = offset/8) and
  the functions naming that table.

## Stream container (read by `func_80069504`, the per-voice streaming update)
Big-endian 32-bit words:
| word | contents |
|---|---|
| 0 | magic `'MORT'` (not checked by the code) |
| 1 | `frames << 16 | sample_rate` (frames incl. silent ones; rate is 16000 in every clip) -> `unk_11A0`, `unk_11A2` |
| 2 | total stream length in 32-bit words, header included (low 24 bits used) -> `unk_11A4` |
| 3.. | bit stream, starts at bit 0x60 |

Streaming: `func_80069440(voice, rom, ringbuf, 0xA00, dmaCallback)` initialises; `func_80069504` DMAs 0x400-byte
chunks (`len = min(0x400, remaining_words*4)`) from `rom + off` into a 4 KB ring (`voice+0x1A0`, 0x400 words),
keeping at most 0xC00 bytes ahead of the decoder, then decodes up to `unk_11DE` frames per call into a
2560-sample (0xA00) s16 PCM ring (`unk_198`). After the last frame it pads with silence, state goes 1->2 (buffer
primed: `func_800511D4` starts playback with pitch = rate/output_rate) ->3 (playing) ->4 (done, voice freed).
The mixer `func_8005173C` resamples (`aResample`) the PCM ring into the stereo output with `volL/volR` (no
synthesizer instrument involved: speech bypasses libmus and is mixed into the same audio command list).

## Bit stream (`func_80069E90`)
Bits are read **LSB first** out of each big-endian 32-bit word (word i holds bits 32i..32i+31, bit 0 = value
bit 0x1). Fields are little-endian across word boundaries. The stream is a sequence of runs:
- run header, 1 bit `s`:
  - `s=1`: 4 bits `n` -> `n+1` silent frames (160 zero samples each; decoder state untouched);
  - `s=0`: 7 bits `n` -> `n+1` coded GSM frames follow.
- coded frame, 260 bits: `LARc[0..7]` with widths 6,6,5,5,4,4,3,3, then 4 sub-frames of
  `Nc`(7) `bc`(2) `Mc`(2) `xmaxc`(6) `xMc[0..12]`(3 each). (Standard GSM 06.10 parameter order and widths, without
  the 4-bit 0xD signature of the 33-byte libgsm frame.)
A new run header is read only when the previous run is exhausted. Frames total `frames`; the bits after the last
frame up to `words*32` are zero in every clip.

`func_80069C00` is an alternative unpacker for a byte-packed 33-byte frame layout (5 header bytes + 4x7 bytes,
MSB first); no caller of it was found in the matched C (dead library code); the streaming path uses `func_80069E90`.

## Decoder
Standard GSM 06.10 full-rate decode, 160 samples/frame (10 ms at 16 kHz in these files; the codec was used at
16 kHz instead of the usual 8 kHz):
- `func_80069920`: LARc -> LARpp (MIC/B/INVA constants of the standard), double-buffered, bank index at 0x188; calls the
  short-term synthesis (`func_8006B104`, hand assembly) ;
- `func_80069B18`: per sub-frame RPE decode (`func_8006AD90`) + long-term synthesis (`func_8006AE84`, clamps Nc
  to 40..120 else keeps nrp at 0x166) ;
- de-emphasis (constant 28180 / 0x6E14 at 0x8006B0A8), upscaling, truncation as in the standard.
Number Journey equivalents: init `func_8006ADB0`, advance `func_8006B1EC`, LAR `func_8006B290`, subframes
`func_8006B488`, byte unpack `func_8006B570`, bit reader `func_8006B800`; the streaming update sits between.

## Songs (not decoded here)
0x4D9D80-0x510A10 (Letter) / 0x4B00C0-0x4E9810 (Number) are libmus songs (header word 0x00000215, then a
channel count and offset table), started by `func_8001892C` / `func_8001933C(start, end, volume, ...)` together with
sound bank D_003E4830..D_003EA160. Their decoding belongs to the libmus/audio extractor.

## Extractor
`tools/extract_speech.py ROM OUTDIR` -> `<game>_<romoff>.wav` (16-bit mono 16 kHz) + `.params.json`
(lossless: run structure and every field) + `<game>_clips.tsv`. `extract_speech.py check ROM` re-packs all
parameters (header + runs + frames) and compares with the ROM bytes of every stream.
