# r1: formats of the ranges the ledger had left `unknown`

All ranges turned out to be one of five already-known formats. Offsets: `common` rows are ROM offsets (the common
block, Letter ROM 0x10D0F0 / Number 0x10D560, is DMA'd raw to RAM 0x8024B000 by func_8001D9D0 / func_8001E2CC ->
func_80017318 / func_80017788); `<level>.bif:` rows are offsets in the inflated level image (RAM 0x80154000).

## pad (zero)
All-zero bytes whose only job is to put the next object on a 16-byte boundary (every following object starts at an
address ending in 0). No code writes into them; they are not buffers. Extractor checks they are zero.

## Palette + CI texture block ("pal+tex")
One contiguous object: an RGBA16 (RGBA5551, big-endian) TLUT immediately followed by its texels.
- CI4: TLUT 16 entries (0x20 bytes) + 32x32 texels, 4 bits each, high nibble = left pixel (0x200 bytes). Total 0x220.
- CI8: TLUT 256 entries (0x200 bytes) + W x H bytes. Bump: 32x32 (0x400, total 0x600); manor: 32x64 (0x800, total 0xA00).
- Unused leading TLUT entries are 0x0000; the texels use exactly the remaining indices (bump: 206-255; manor: 33-255;
  r1 verified this: the zero run at the start of each bump block is unused palette, not zero fill).
- How the game uses it: either a display list loads TLUT and texels with absolute addresses (level images), or the
  code sets F3DEX segment 4 to the block start and a shared model display list loads TLUT from seg4+0 and texels from
  seg4+0x20. Number func_8003670C: `gsSPSegment(4, block)` (G_MOVEWORD 0xBC001006) choosing one of six blocks at
  RAM 0x802AF280 + k*0x220 by level number D_8012F154 (5/6, 7/8, 9/10) and object state (+0x1D8 >= 2 or not).
  Letter's twin func_8002BADC reads the segment-4 base from a model record field (+0x18).
- Extraction: one indexed PNG per block (PLTE = the TLUT colours, tRNS = alpha bit as 0/255). Lossless: re-encoding
  with gfx/tools/n64tex.encode gives back TLUT+texels byte for byte (`check`).

## Vtx (F3DEX vertex)
16 bytes: s16 x, y, z; u16 flag (0); s16 s, t; u8 r, g, b, a. JSON: list of the 10 fields per vertex.

## Model bbox and header (packC ModelHdr)
bbox: 6 x f32 (min x,y,z, max x,y,z), stored right before the header. Header 0x28 bytes = 10 big-endian words:
+0x00 u32 flags/type (0x11), +0x04 u16 vertex count, +0x08 display list, +0x0C vertex array, +0x1C vertex array
(again), +0x24 bbox pointer; other words 0. See packC/FORMAT.md for the full ModelHdr. JSON keeps all 10 words.

## Tools
- `tools/extract_r1.py ROM GAME OUTDIR [check]`: writes OUTDIR/GAME/*.png|json, `view/` (4x copies), `index.json`
  (per object: RAM, index range used, zero TLUT entries, round-trip result). `check` exits 1 on any mismatch.
- `tools/probe.py ROM GAME`: hex context + pointer search (all .data words, all words of common and the owning
  level image, lui/addiu|ori|load|store address pairs over the whole .text, window +-0x40).
- `tools/twins.py ROM_LETTER ROM_NUMBER`: byte-identical copies of each texel/TLUT blob across both games.
- `tools/r1common.py`: range tables, image loader (gzip inflate), code address scanner.
