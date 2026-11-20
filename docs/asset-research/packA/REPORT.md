# packA report
Territory (corrected): Letter 0x7C6B0-0xA54D0, Number 0x7CB20-0xA5940 (mode-22 menu sprite pack, 167456 B each).
## Done
- 100% mapped in MAP.tsv: 83 objects/game, all class texture; 0 pad, 0 unknown.
  Bytes: 77 CI4 64x64 Elmo frames 159,140 B; 2 CI8 32x32 icons 3,072 B; 4 CI4 64x32 labels 4,224 B (+20-byte TLUTs incl.) = 167,456 B.
- tools/extract_packA.py ROM GAME OUTDIR -> indexed PNGs + packA.json; `check` = MATCH both games.
- FORMAT.md documents loader (func_8001D9D0/func_80017318, draw func_8001A318; Number func_8001E2CC/func_80017788/func_8001AC1C) and encoding.
## Odd
- File is byte-identical in both games.
- Unreferenced CI8 "123" icon at +0x271A0 (Number art) in both; both games draw "ABC".
- Unreferenced Elmo frame at +0x0.
- FILES_*.tsv description (320x240 screens) is wrong for this file; that is the mode-21 layout.
## Not done
- Nothing within territory.
