# libmus song binaries and the default effect bank (Elmo's Letter Adventure / Elmo's Number Journey)

The music player is Software Creations' libmus. Letter addresses come first, with Number's in [brackets]. The two
games use the same player code: every handler in Number is at the Letter address + 0x1970.
All values are big-endian. Offsets are relative to the start of the file.

## 1. How the game finds and starts a song
- No song table exists. Every call site passes a ROM start/end symbol pair to
  `func_8001892C(romStart, romEnd, vol, reverb)` [func_8001933C]. That function:
  1. copies the music pointer bank (Letter 0x3E4830-0x3EA160 [0x3CB880-0x3D0980]) and the song into heap memory
     (`func_80040400` alloc + `func_80017318` [func_80017788] PI copy);
  2. relocates the bank with `func_8004D274(bank, waveBankRom)`. The bank code is in func_8004E9E0 (it adds the
     wave bank ROM address to each sample offset, converts the tune bytes, and so on). The bank format belongs to
     agent audio;
  3. makes the bank current with `func_8004D2B0`;
  4. starts the song through `func_8004C9B0` -> song start `func_8004F1A0` [func_80050B10], which returns a handle
     that is stored in D_800A7AFC;
  5. sets the handle reverb with `func_8004D1EC(handle, reverb)` (clamped 0..127, stored in channel +0xCB) and the
     volume scale with `func_8004D020(handle, vol)` (channel +0x9E; 0x80 = unity, set by func_8004EC0C). Every call
     passes reverb 0x14. vol is 0x40..0x55.
- Songs stop through `func_8004CF14(D_800A7AFC, 0)` before the next song starts.
- Song start, `func_8004F1A0` [func_80050B10]: if header word 0x28 is 0, it sets the word to 1 and turns the 7
  offsets at 0x0C..0x24 into pointers (zero stays zero). It then does the same for the entries of the channel,
  volume and bend tables (`tracks` entries each). It allocates one channel for the master track (flags |= 3) and one
  channel for each track whose channel pointer is non-zero. On every channel it sets: data pointer +0x04 (copied to
  base +0x80), volume stream +0x38 (copied to base +0x8C), bend stream +0x34 (copied to base +0x88), velocity mode
  +0xD2 = 1, and the song handle.
- Channel defaults come from the reset function `func_8004EC0C`: tempo 0x6000/fps (= 120 bpm), volume 127,
  pan 0x40, velocity 127, fixed length off, bend range 1/32 (±2 semitones), envelope (1,0,1,127,255,127,15).

## 2. Song header (0x38 bytes)
| off | size | field |
|---|---|---|
| 0x00 | u32 | version, always 0x00000215 |
| 0x04 | u32 | `tracks`: number of entries in each of the three per-track tables |
| 0x08 | u32 | `waves`: number of u16 entries in the wave map |
| 0x0C | u32 | offset of the channel-data pointer table (tracks × u32 offsets, 0 = track unused) |
| 0x10 | u32 | offset of the volume-stream pointer table (tracks × u32, 0 = none) |
| 0x14 | u32 | offset of the pitch-bend-stream pointer table (tracks × u32, 0 = none) |
| 0x18 | u32 | offset of the envelope table (7-byte entries; command 0x90 indexes it) |
| 0x1C | u32 | offset of the drum table (6-byte entries; command 0x9E points into it) |
| 0x20 | u32 | offset of the wave map (`waves` × u16) |
| 0x24 | u32 | offset of the master-track command stream |
| 0x28 | u32 | relocated flag (0 in ROM; set to 1 by func_8004F1A0) |
| 0x2C | 12 bytes | zero in all 37 songs; the player never reads them |

Typical layout, which every song in both games follows: header, the zero block, the channel/volume/bend tables at
0x38, then the wave map, drum table, envelope table, the volume streams, the bend streams, the master track and the
channel streams. The extractor does not assume this order. It splits the file at every offset that the header or a
table points to.

### Wave map (u16 entries)
Command 0x81 `wave n` stores n in the channel (+0xAE). When a note starts, the sample is
`bank wave index = wavemap[n]`; func_8004DA5C reads `song->wavemap[ch+0xAE]`. If the entry is 0xFFFF, the note is
played as a rest. The bank wave index selects the instrument pointer (`ptrbank+0x2C`) and the tune
(`ptrbank+0x28`) in the music pointer bank.

### Envelope (7 bytes, func_8004C5B0 / func_8004F4DC)
`speed, init_vol, attack_time, peak_vol, decay_time, sustain_vol, release_time`. A speed of 0 is treated as 1.
The channel gets `+0x64 = 0x400/speed`. The attack ramp runs from init to peak over attack_time, the decay runs
from peak to sustain over decay_time, and the release runs over release_time. Command 0x84 gives the same 7 bytes
inline.

### Drum entry (6 bytes)
`u16 wave (wave-map index), u16 envelope index, u8 pan (stored >>1), u8 note`. After `drums n` (0x9E) the channel
points at entry n (+0x84). In drum mode the note byte of each note event indexes the table from that entry. The
entry supplies the wave, the envelope, the pan and the real pitch (func_8004DA5C). `drumsoff` (0x9F) clears the
pointer.

## 3. Volume and pitch-bend streams (func_8004E674 volume, func_8004E738 bend)
Each stream is a byte stream that one track reads once per tick, in step with the channel clock:
- a byte `v < 0x80` sets value v for 1 tick;
- a byte `0x80|v` sets value v, then a length follows as a varint (see below); the value holds for length+2 ticks.

Volume values go to the channel (+0xBC). A bend value v gives `(v-64) × bendrange` (+0x70/+0x24). The default
bend range is 1/32 semitone per unit (64 units = 2 semitones). Command 0xA8 changes it to n/64 per unit. Neither
stream has a terminator: it is read for as long as the channel plays. `goto` (0xA1) moves both stream pointers.

## 4. Channel command stream (func_8004DA5C, jump table D_80076E10 [D_80077620], 45 entries)
**Varint** (`v`): `b < 0x80` → b; otherwise `((b & 0x7F) << 8) | next`.

**Note event** (first byte < 0x80): `note`, then
- `velocity` byte, only in velocity mode (+0xD2). If bit 7 is set, the velocity is `b & 0x7F`, it stays fixed for
  the following notes, and velocity mode ends (+0xD3 holds the fixed velocity);
- `length` varint. It is omitted when a fixed length is set (+0xAC, command 0x8B). An `ignore` (0x8C) makes the
  next note read a length anyway.

A note value of 0x60 is a rest. The length is in ticks: the time until the next event. A tick is 1/48 of a beat at
the current tempo. The channel clock advances `tempo_word = bpm×3×8192/120/fps` per frame in units of 1/256
tick, which gives 0.8×bpm ticks per second.

**Commands** (byte ≥ 0x80). A handler returns the new stream pointer. stop returns NULL, which ends the channel.
Handlers are listed as Letter address [Number = Letter + 0x1970]; the names are the libmus names. Channel offsets
are in the channel structure of 0x13C bytes.

| byte | name | handler | args | effect (from the handler code) |
|---|---|---|---|---|
| 80 | stop | 8004F460 | – | clears the vol/bend/data pointers and the song/bank; the channel ends |
| 81 | wave | 8004F480 | v | wave-map index → +0xAE |
| 82 | port | 8004F4B0 | b | portamento time → +0xB8; if non-zero, the start pitch (+0x50) is the current pitch |
| 83 | portoff | 8004F4D0 | – | +0xB8 = 0 |
| 84 | envdef | 8004F4DC | 7×b | inline envelope (same 7 fields as the envelope table) |
| 85 | tempo | 8004F604 | b | bpm. Sets the tempo word of every channel of this song (+0xA8) and the scaled value (+0x9C = tempo×+0x98>>7) |
| 86 | cutoff | 8004F6FC | u16 | release the note n ticks after it starts (+0xB2); clears endit |
| 87 | endit | 8004F6E8 | b | release the note n ticks before its length ends (+0xB4); clears cutoff (func_8004E280) |
| 88 | vibup | 8004F720 | 3×b | vibrato: delay (+0xB6), period (+0xD5, rate 2π/period), depth/50 semitone (+0x20) |
| 89 | vibdown | 8004F790 | 3×b | the same with negative depth |
| 8A | viboff | 8004F804 | – | vibrato off (+0xD5 = 0) |
| 8B | length | 8004F814 | v | fixed note length (+0xAC); the following notes have no length field |
| 8C | ignore | 8004F844 | – | the next note reads a length despite the fixed length (+0xB7 = 1) |
| 8D | trans | 8004F854 | s8 | transpose in semitones (+0xB9) |
| 8E | ignoretrans | 8004F864 | – | +0xBA = 1: transpose is not applied |
| 8F | distort | 8004F874 | s8 | fine detune in 1/100 semitone (+0x90, added to the pitch base +0x30) |
| 90 | env | 8004C5B0 | v | load envelope n from the song envelope table |
| 91 | envoff | 8004F8CC | – | +0xD6 = 1: the envelope does not restart on new notes |
| 92 | envon | 8004F8DC | – | +0xD6 = 0 |
| 93 | troff | 8004F8E8 | – | +0xD7 = 1: notes change pitch only, with no new sample start (legato) |
| 94 | tron | 8004F8F8 | – | +0xD7 = 0 |
| 95 | for | 8004F904 | b | push a loop: count n (0xFF = forever), saves the data, volume and bend pointers and their counters, the volume, and the bend (stack depth 4) |
| 96 | next | 8004F970 | – | if the count is not 0xFF, decrement it; pop when it reaches 0, otherwise return to the loop start and restore the volume/bend state |
| 97 | wobble | 8004FA0C | 3×b | pitch wobble: amount (+0xD8), on-time (+0xCE), off-time (+0xCF) (func_8004E5B8) |
| 98 | wobbleoff | 8004FA34 | – | +0xCE = 0 |
| 99 | velon | 8004FA40 | – | velocity mode on: each note carries a velocity byte |
| 9A | veloff | 8004FA50 | – | velocity mode off |
| 9B | velocity | 8004FA5C | b | fixed velocity (+0xD3), velocity mode off |
| 9C | pan | 8004FA70 | b | pan = b>>1 (+0xBD), 0..63 |
| 9D | stereo | 8004FA84 | 2×b | ignored (the handler only skips the 2 bytes) |
| 9E | drums | 8004FA8C | v | drum mode: +0x84 = &song.drums[n] |
| 9F | drumsoff | 8004FAD4 | – | +0x84 = NULL |
| A0 | print | 8004FAE0 | b | ignored (debug print in libmus) |
| A1 | goto | 8004FAE8 | 3×u16 | data = base+a, volume stream = volbase+b, bend stream = bendbase+c; both counters reset to 1 |
| A2 | reverb | 8004FB58 | b | reverb send (+0xCA). The voice reverb is `handle_reverb + ((0x80-handle_reverb)×v>>7)` |
| A3 | randnote | 8004FB68 | 2×b | transpose = random(a) + b (func_800504E8) |
| A4 | randvolume | 8004FBB8 | 2×b | volume = random(a) + b (func_8004EB78) |
| A5 | randpan | 8004FC08 | 2×b | pan = random(a) + b |
| A6 | volume | 8004FC58 | b | channel volume (+0xBC) |
| A7 | startfx | 8004FC68 | v | start effect n of the channel's effect bank (+0x78) with func_8004EFF8 (same handle volume and pan, priority +1). The new voice takes over this channel's handle |
| A8 | bendrange | 8004FD38 | b | bend range = n/64 semitone per bend unit |
| A9 | sweep | 8004FD74 | b | pan sweep speed (+0xD4; func_8004E4E4/func_8004E500) |
| AA | changefx | 8004FD84 | b | if the synth fx mode D_800A0570 == 1: select reverb preset n (func_80050640, table D_800670C4) |
| AB | marker | 8004FDC8 | b, b[, b] | two bytes; if the second has bit 7 set, one more byte follows. If the channel is a song channel (flags&3==2) and a callback D_800A0580 is set: callback(handle, first byte) |
| AC | length0 | 8004FE38 | – | fixed length off (+0xAC = 0) |

Bytes 0xAD..0xFF have no handler (the table ends with zero words). None occurs in the data.

How a note starts (func_8004DA5C): if a drum entry is active, it supplies the wave, the envelope, the pan and the
note. If envelopes are on, func_8004E280 restarts the envelope. If sweep is set, func_8004E4E4 restarts the sweep.
`key = wavemap[wave]`. If troff is not set, the sample starts (func_8004DFA4). The pitch is `note + tune[key] +
transpose (unless ignoretrans)`. If the reverb changed, it goes to the voice. A rest starts the release phase.
The per-frame driver is `func_8004D76C`. For each channel it runs the commands while the channel clock is past the
next event time, then the volume and bend streams, the envelope, the sweep, the vibrato and the wobble.

The master track (header +0x24) is an ordinary command stream run on its own channel. In these games it holds
`for 255; tempo n; velon; changefx n; rest ...; next`, which sets the tempo and the reverb type, then waits.

## 5. Default effect bank (Letter 0x3E3FB0-0x3E4830 [0x3CB0C0-0x3CB880])
`func_800170B0` [func_80017520] copies it to RAM 0x80115390 [0x80115530] and passes it as musConfig +0x20.
`func_8004C6FC` (MusInitialize) gives it to `func_8004D43C`, which relocates it once.
| off | size | field |
|---|---|---|
| 0x00 | u32 | `count`, the number of effect entries (Letter 70, Number 67) |
| 0x04 | u32 | not read by the player (Letter 65, Number 61) |
| 0x08 | u32 | number of wave-map entries (Letter 60, Number 58) |
| 0x0C | u32 | flags; bit 0 = relocated (set by func_8004D43C) |
| 0x10 | u32 | pointer bank to use (0 in ROM = the default bank; becomes the voice's +0x7C) |
| 0x14 | u32 | offset of the wave map (u16 × waves, then 4 zero bytes to the end of the file) |
| 0x18 | 8 × count | `{u32 offset of the effect command stream, u32 default priority}` |

An effect is started by number with `func_8004EFF8(bank, n, volume, pan, priority)` (priority -1 = the entry
default). It takes a free voice channel, or the lowest-priority one if the new priority is higher. The voice runs
the effect stream with the same command language as songs, and wave numbers go through the effect bank's wave map
(func_8004DA5C: `unk_78->wavemap` when the channel has no song). Unlike song channels, effect channels start with
velocity mode off.

## 6. Text dump syntax (tools/extract_songs.py)
One line per item. `@OFFSET kind [tag]` starts a region: header, raw, ptrs chan|vol|bend, wavemap, drums, envs,
vol N, bend N, chan N|master, fxheader, fxlist, fx N.
- `header k=v ...`, `fxheader k=v ...`, `ptr 0x..`, `u16 ...`, `drum wave env pan note`, `envelope 7 bytes`,
  `fxent offset priority`
- volume/bend streams: `set V` (1 byte), `hold V LEN` (0x80|V + varint)
- channels: `note K [vel V[!]] [len N]`, `rest [vel V[!]] [len N]`. `!` = velocity byte with bit 7 set.
  Command lines use the names in the table, with signed args for trans/distort and hex args for cutoff/goto.
- A varint written with a suffix `L` (e.g. `len 5L`) is a value below 0x80 stored in the 2-byte form.
- `pad N` = N zero bytes after a stop or goto. `hex ...` = raw bytes: the zero block at 0x2C, and the unreachable
  tails after an infinite `next` or a `stop`, which do not decode.

`check` re-assembles every dump and compares it byte for byte with the ROM.

MIDI (approximation): 48 PPQN; the master-track tempo goes to the tempo track. There is one MIDI track per song
track; drum mode goes to MIDI channel 10. Notes are note+transpose+12 (the sample tune is ignored), velocities are
taken from the stream, and 0x81 wave changes become program changes with the wave-map index. The volume streams go
to CC7, bends to the pitch wheel (±2 semitones), pan to CC10, A6 volume to CC11 and reverb to CC91. Loops are
expanded, an infinite loop is played once, and a `goto` ends the track with a `loop` marker.
