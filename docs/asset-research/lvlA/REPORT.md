# lvlA report: level containers

Done
- tools/extract_levels.py ROM GAME OUTDIR [check]: inflates the 8 members per game to out/<game>/<name>.bin.
  All 16 CRC32/ISIZE OK. No zlib level 1-9 reproduces the compressed bytes: the proof is decompress + CRC only.
  (Agent `files` reached the same result independently in ../files/.)
- MAP.tsv: 32 rows (16 gzip members + 16 junk tails) tiling Letter 0x1BF6F0-0x2F9B60 and Number 0x177680-0x2E6FC0
  (Number's 0x2B5120-0x2B8FD0 is the end of river.bif, not a separate file).
- tools/level_sections.py + SECTIONS_letter.tsv / SECTIONS_number.tsv: per-file map by kind with code references.

Byte counts (compressed ROM): Letter 1,287,202 B gzip in 1,287,280 B (0x13A470) of territory; Number 1,505,535 B in 1,505,600 B (0x16F940);
pad tails 78 B (Letter), 65 B (Number). Inflated: Letter 2,624,338 B, Number 2,965,768 B.

Odd
- Gzip tails are non-zero junk (stale buffer), not zero padding. MTIME 0 / OS 0: produced by a non-gzip tool.
- space.bif (Letter) has almost no display lists reachable by the scan (12 DLs, 1 vertex array, 0 textures): 98% is
  `data`; its geometry is likely built or referenced differently (open for lvlB).
- `data` (unreached) remains large in several files (bonusabc, planet, bump): needs code-driven decoding by lvlB-D.

Not done
- Per-section semantic decoding of `data` (collision, actors, paths) and confirmed root lists for Number.
