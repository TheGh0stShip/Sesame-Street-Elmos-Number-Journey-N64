# Level files (`.bif`, class gzip_level), both games

## Container
- Each file is one RFC 1952 gzip member: ID 1F 8B, CM 8 (deflate), FLG 0x08 (FNAME only), MTIME 0, XFL 0, OS 0,
  zero-terminated stored name (`streetab.bif` ...), raw deflate stream, CRC32 (LE), ISIZE (LE).
- After the trailer, 2-15 bytes of non-zero junk pad the member to the next 16-byte-aligned file start (class `pad`).
- The game locates each file only by a start/end symbol pair in code (no file table): Letter func_8001D9D0, Number
  func_8001E2CC, `switch(level)`; size = end - start. Number's river.bif runs 0x27ADC0-0x2B8FD0 (the code pair is
  D_0027ADC0/D_002B8FD0); the coordinator's split at 0x2B5120 is wrong (D_002B5120 is a size expression for a
  RAM clear in func_80019258/func_80019470, not a file).
- Reader: func_8001375C [Number twin] sets up the stream: output to 0x80154000, limit 0x86800 bytes, zlib work
  areas 0x8012C280 / 0x80134080. func_80013560 [Number func_800139D0] DMAs 16 KB chunks (func_8000C0E0) into
  0x8012FD50; on the first chunk it skips the 10-byte fixed header and the name by scanning for the NUL from byte 9
  (FLG is assumed to be 0x08), then calls inflate func_80013AD8 (flush -5) until done. The trailer is never checked.
- Recompression: no zlib level 1-9 (wbits -15, memLevel 8, default strategy) reproduces any of the 16 deflate
  streams. The lossless proof is therefore decompress + CRC32 + ISIZE (all 16 pass); `extract_levels.py check`.

## Inflated file: a ready-made RAM image at 0x80154000
- No header, no section table, no relocation: all internal pointers are absolute KSEG0 addresses based at
  0x80154000 (offset = pointer - 0x80154000). The image starts directly with an F3DEX display list.
- Roots: the code holds hard-coded absolute addresses into the image, chosen by level or by object type IDs
  (e.g. func_8003BB00 `switch(type)` with 0x2106, 0x4605 ... giving per-object DL/data addresses; func_80023118,
  func_80024E20, func_800264F4, func_8002A120, func_8001A318, func_80018A60, func_80004010; func_8000B470 passes
  per-level (image, palette?) address pairs 0x200 apart to func_8001CAA8, case 4 = 0x80154200/0x80154000).
  These are listed per section in SECTIONS_<game>.tsv ("code refs"). Only Letter symbol names are reliable;
  Number twins carry Letter names, so Number code refs in the TSV are indicative only.
- Contents by kind (SECTIONS_<game>.tsv, from tools/level_sections.py): F3DEX display lists (runs of commands ending
  G_ENDDL 0xB8); vertex arrays (G_VTX 0x04 targets, 16-byte Vtx, length from w0&0xFFFF + 1); texture/palette data
  (G_SETTIMG 0xFD targets, extent = up to the next classified byte); pointer structures (words pointing into the
  image); `data` = bytes not reached from any display list: collision/BSP, placement, paths are expected there,
  reached only through code (lvlB-D).
