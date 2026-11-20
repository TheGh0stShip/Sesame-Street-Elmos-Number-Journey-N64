# songs: libmus song binaries and default effect banks (both games)

## Done
- Territory: 18 Letter songs (233,088 bytes), 19 Number songs (235,344 bytes), and the default libmus effect banks
  (the "small unidentified files"): Letter 0x3E3FB0-0x3E4830 (2,176 bytes) and Number 0x3CB0C0-0x3CB880 (1,984
  bytes).
- Letter 0x2FD600 is not a file: `D_002FD600` is a size used in `memSize - 0x2FD600` (func_80018840 / func_80018A60).
  0x2FBD50-0x2FE8D0 is one pointer bank, owned by agent audio.
- MAP.tsv has one row per file (39 rows) covering every byte. Inside each file every byte appears in the text dump.
- FORMAT.md covers the header, the tables, envelopes, drums, the volume/bend stream coding, all 45 commands
  0x80-0xAC with their handlers (jump table D_80076E10; Number D_80077620, handlers at +0x1970), the fx bank
  header and the text syntax.
- tools/extract_songs.py writes a text dump, a MIDI file per song, and fxbank.txt.
- `check` re-assembles every dump: letter 19/19 and number 20/20 are byte-identical to the ROM.
- Bytes left as hex: 280 in Letter and 306 in Number. They are the 12-byte zero block at header +0x2C, plus 15
  unreachable tails after an infinite `next` or a `stop`.

## Which scene plays which song (everything goes through func_8001892C [Number func_8001933C])
Letter:
- 4D9D80: title (func_80018A60 state 0), and in func_8003AB7C level 19 when all three flags are set
- 4DB3E0: func_80018A60 state 19 (mode 22 screen)
- Level music: 4E32C0 = 19 bonusabc, 4EADF0 = 18 orchard, 4F2030 = 17 garden, 4FAC20 = 15 manor,
  501820 = 16 chute, 507890 = 13 planet and 14 space
- Item-collected jingles (func_8003A650): 4F1850 = 18, 4FA460 = 17, 501130 = 15, 507070 = 16, 50C970 = 13/14
- 50D000: level-complete fanfare (func_8003AB7C at timer 270; not levels 11/19)
- 4EA530: bonus-level end (func_8003AB7C, level 19 at timer 370)
- Never started: 2F9B60, 4DD320, 50F9E0

Number:
- 4B8E70: title, and the func_8003C494 level 11 task-7 branch
- 4BA590: state 19 (mode 22)
- 4E1090: hub (level 1)
- Level music: 4B00C0 = 10, 4BCB50 = 9, 4C1970 = 7, 4C69A0 = 8, 4CBCD0 = 6, 4D0690 = 5, 4D83F0 = 11 bonus123
- Jingles (func_8003BF64): 4BC400 = 10, 4C12F0 = 9, 4C6350 = 7, 4CB8D0 = 8, 4CFEC0 = 6, 4D7C90 = 5
- 4E7620: level-complete fanfare (func_8003C494 at timer 270)
- 4E0870: bonus end (timer 470)
- 4DF7C0: guide actor func_800287AC, state 0x48, when task 7 is reached
- All Number songs are used.

## Odd findings
- Letter's unused song 0x2F9B60 is byte-identical to Number's fanfare 0x4E7620: a leftover from the sibling build.
- Letter's hub (level 2) has no music call; Number's hub plays 4E1090. Letter's unused 4DD320 (16 tracks, a full
  piece) may be the orphaned hub tune.
- Every master track is `for 255; tempo; velon; changefx n; rest; next`: songs also select the synth reverb preset.
- The fx bank header word +4 (65 / 61) is never read. Stereo (0x9D) and print (0xA0) are parsed and ignored.
  Marker (0xAB) reaches a game callback D_800A0580.

## Caveats
- The MIDI files are approximations: no samples or envelopes, the sample tune is ignored, an infinite loop plays
  once, and `goto` ends the track.
- Overlap: agent audio's MAP.tsv and extract_audio.py also cover these songs and the fx bank. The boundaries agree;
  the coordinator should keep one set of rows.
- The scene descriptions for the level-end and guide scripts come from reading the code; they are not checked in an
  emulator.
