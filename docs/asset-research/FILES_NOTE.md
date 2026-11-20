# The ROM after the program image is a sequence of files (coordinator's first pass)

`${N64DK_LAB}/sesassets/FILES_letter.tsv` and `${N64DK_LAB}/sesassets/FILES_number.tsv` list every file: id, ROM start, ROM end, size, class guess, note.
Boundaries come from the ROM offsets the code references (`D_00xxxxxx` symbols in the matched sources; the game
loads a file by a start/end symbol pair) plus magic numbers. Class guesses are from the first bytes only: verify.

What is known:
- `gzip_level`: 8 per game, standard gzip members with a stored file name; the game inflates them with its zlib
  code (Letter func_80013AD8 / func_8001602C family). About 2.6 MB unpacked per game: level/scene data.
- `libmus_ptr_bank` / `libmus_wave_bank` / `libmus_song`: the sound system is Software Creations' "libmus" music
  library (containers begin "N64 PtrTable" / "N64 WaveTable"; song binaries begin with the word 0x215). The
  functions in Letter 0x8004C000-0x80053000 that we called "the game's own sequencer" are the libmus player.
- Speech (`MORT` GSM streams) is already done by agent speech: see `${N64DK_LAB}/sesassets/speech/` (FORMAT.md, MAP.tsv).
- `sprite_anim` (ROM 0x61AFB0 onward in Letter): files starting with u16 width, u16 height, then a table; probably
  run-length coded character animation frames. Referenced in Number by func_800287AC / func_80027FE0 and in Letter
  by the twin functions (find them by searching the matched sources for the file's `D_00xxxxxx` symbol).
- The four files before the first gzip member (Letter 0x7C6B0, 0xA54D0, 0xA8D50, 0x10D0F0) are graphics packs:
  one begins with an F3DEX end-of-display-list command; find how the code uses each through its `D_00xxxxxx` symbol.
- Number Journey's SOURCES sometimes carry Letter's symbol names (twin functions share one source), so for Number
  find references by disassembling (`GAME=number AG=<you> ${N64DK_LAB}/sestools/s.sh dis ADDRESS` gives real addresses) or
  by the structure of the file table, not by grepping names.
To see how a file is used: grep the matched sources (`${N64DK_LAB}/sesverify/v8L/letter/src`) for `D_00` + the start offset
in 6 hex digits (for example `D_0010D0F0`), then read that function and its callers.

## UPDATE (authoritative): agent `files` finished the loader analysis
Read `${N64DK_LAB}/sesassets/files/REPORT.md` and `${N64DK_LAB}/sesassets/files/FORMAT.md`; its tables `${N64DK_LAB}/sesassets/files/FILES_letter.tsv` /
`FILES_number.tsv` SUPERSEDE the coordinator's first-pass tables where they differ, and its extracted and
inflated files are in `${N64DK_LAB}/sesassets/files/out/<game>/` (with SHA1 lists). Key facts:
- There is no file table: each file is a start/end symbol pair in code; `files/tools/romrefs.py` lists them.
- The 8 gzip `.bif` files per game (Letter: streetab, chute, manor, orchard, garden, space, planet, bonusabc;
  Number: street, dunk, bump, mansion, maze, ski, river, bonus123) each inflate to a READY-MADE RAM IMAGE loaded
  at 0x80154000 (max 0x86800 bytes): textures, vertices and F3DEX display lists with ABSOLUTE pointers (no
  relocation). gzip reader: Letter func_80013560 [Number func_800139D0]; level loader func_8001D9D0 [func_8001E2CC].
- After a level is inflated, a raw `common` block is copied to 0x8024B000 (also a ready-made RAM image); modes 21
  and 22 load raw menu screen files (0x200-byte palette + image).
- Sound: func_800170B0 [func_80017520] sets up the sfx and fx banks; func_8001892C [func_8001933C] loads the music
  pointer bank and a song. Number's bank addresses differ from Letter's (0x2E6FC0 sfx pointer bank, 0x2E9A90 sfx
  wave bank, 0x3CB0C0-0x3CB880 fx bank).
- OPEN QUESTION for whoever owns Letter 0x61AFB0-0x741920 / Number 0x61F510-0x7CE410: agent files calls these
  "whole-clip speech ranges" loaded by the speech thread func_80026920 (fed by func_800272C8 / func_80026B70
  [func_800287AC / func_80027FE0]); agent speech thought they look like images (u16 width, u16 height, row
  table). Decide from the code which it is.

## UPDATE 2: where the code points into the level image and the common block
`LEVEL_REFS_letter.tsv` / `LEVEL_REFS_number.tsv` (this folder) list every RAM address in 0x80154000-0x802C0000
that the game refers to, computed from the ROM itself (not from source names): addresses built in code (with the
function), and pointer words stored in the program's .data (with the word's own address). These are the ROOTS into
the ready-made RAM images: objects there are found from these addresses (and from pointers inside the images).
Also finished: `sprA` (the files after the speech region are keyframed vertex morph animations, not sprites: see
sprA/FORMAT.md), `packB` (menu21 = six 320x240 screens, palette + CI8/CI4), `audio` (libmus banks and a full song
dump with command table: audio/FORMAT.md), `lvlA` (level container; SECTIONS_<game>.tsv first pass).
