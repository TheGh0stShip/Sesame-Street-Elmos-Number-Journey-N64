# packB report: menu21 screen pack (both games)

Territory as corrected by the coordinator: one file each, Letter 0x0A54D0-0x10D0F0, Number 0x0A5940-0x10D560
(424992 bytes each). The first-pass boundary 0x0A8D50 was not a file: `D_000A8D50` in func_80012164 is the
physical (K0-stripped) address of a RAM matrix buffer D_800A8D50 used in a G_MTX command (`0x01020040`).

Done: every byte classified, 12 MAP lines per game, extractor runs and `check` passes 12/12 (all 6 PNGs per
game round-trip byte-exact).

Bytes per class (each game): texture 418,560 (5 x 76,800 CI8 + 38,400 CI4); palette 2,592 (5 x 512 + 32);
pad 0; unknown 0.

Contents (looked at the PNGs): title / splash / publisher-logo screens, e.g. Letter slot 0 is the "Elmo's Letter
Adventure" title screen, Number slot 5 is the Realtime Associates logo.

Odd: slot 4 (file 0x4B800, CI4 with 16-colour palette, 0x9620 bytes) is never drawn: the only blitter is
CI8-only and no code references RAM 0x8019F800. It is a text screen "Voice Compression Technology licensed by
Factor 5 and Lucas Arts Entertainment Company" (the MORT speech codec credit), present in both games: a
leftover/unused licence screen.

Note: the brief says PIL is in the lab venv; it is not (`No module named 'PIL'`), so the extractor has its own
minimal PNG reader/writer.

Outputs: MAP.tsv, FORMAT.md, tools/extract_packB.py, out/<game>_menu21_<slot>_<offset>_ci<bpp>.png (lab only).
