# g2 report: proper forms for the classes r4 still placed as original bytes

All checks byte-exact for both games (`tools/run.sh` via job.sh; last run rc=0). MAP.tsv: 809 rows.

| class | Letter regenerated / still literal | Number regenerated / still literal |
|---|---|---|
| ADPCM frames (all samples) | 213,324 frames from WAV + header rule; 2,771 header bytes kept as an explicit per-frame field; 34 clamped frames (306 B) still verbatim in audio's JSON | 204,044 frames; 2,467 header bytes as a field; 68 frames (612 B) verbatim |
| ROM header | 64 / 0 (13 named fields, CRCs computed) | 64 / 0 |
| IPL3 | 4,032 / 0 (assembled from ipl3.s; 2,864 B code + 1,168 B data table as .word) | same |
| RSP microcode (5 blobs) | 11,268 / 0 (RSP assembly + data lines) | 11,268 / 0 |
| packC textures + TLUTs | 96,728 / 0 (182 PNGs) | 73,960 / 0 (176 PNGs) |
| stale pad bytes | 340 / 0 (39 leftover rows, rule copy -0x2000; 339 non-zero) | 487 / 0 (53 rows; 486 non-zero) |

## Findings
- ADPCM: the faithful SDK `vadpcm_enc` port, run on the decoded PCM, reproduces 94.1% (Letter) / 93.5% (Number) of the
  frame headers. It does not reproduce all of them: the original was encoded from 16-bit source audio that the decode
  does not preserve, and the SDK's choice depends on the unquantised error against that source. 5,301 / 4,991 frames
  differ only in scale, and 7,389 / 8,182 differ in predictor. Rule "SDK choice if it reproduces the PCM, else first
  exact header" leaves 2,771 / 2,467 frames whose header is stored explicitly in headers.json. That is 1 byte of
  metadata per frame, not a verbatim frame. This is fewer than r4's 3,264 / 2,836.
- IPL3 is the standard 6102 and identical in both games. Code runs 0x40-0xB70 (716 instructions, 56 labels) and a
  data table runs 0xB70-0x1000.
- RSP: the 5 blobs are identical between the games, and every text word decodes and re-encodes (0 `.word`
  fallbacks). Mnemonics agree with rabbitizer's RSP decoder.
- Pad leftovers: these are file tails padded to 16 bytes from an 8 KiB write buffer. They follow 8 gzip members per
  game, Letter's two wave-file ends (bank 1 at 0x3E3FA2, bank 2 at 0x4D9D78) and Number's bank-1 end, and 29 / 44
  speech clips.

## Caveats
- The leftover sources for the 8 gzip tails per game lie inside the deflate streams, which r4 still places as
  exceptions. They regenerate only once those streams do.
- tex_g2 imports packC's analyser, and leftover_g2 / tex_g2 read r4's REBUILD tsv for their territory. adpcm_g2 reads
  audio's WAV/JSON.
- Not covered: the 224-byte packC `dl` verbatim row in Letter (0x1BF5D0) and the gfx/r1 verbatim rows. Together they are
  1,532 (Letter) / 2,742 (Number) bytes of r4's verbatim 109,528 / 87,970. The other verbatim bytes are covered here:
  96,728 + 11,268 RSP for Letter and 73,960 + 11,268 for Number.
