# Elmo's Letter Adventure / Number Journey: the file/loader system (post-program ROM)

## How the game reads ROM
There is no file table: every ROM range is addressed by a pair of link-time ROM symbols `D_00xxxxxx`
(start, end), built in code with lui/addiu; size = end - start. `tools/romrefs.py ROM 1050 <text_end> 7C000 800000`
lists every such constant and the instructions using it (authoritative; the v8L number sources for
func_80017520 carry Letter values, the kit's number copy and the ROM carry the real ones: 0x2E6FC0/0x2E9A90/0x3CB0C0).

PI access functions (Letter names; Number in brackets):
- `func_8005EE10` [func_80060780] = osPiStartDma; `func_8005E6E0` [osEPiStartDma] used by the audio DMA callback `func_8005095C`.
- `func_80017318(rom, ram, size)` [func_80017788]: blocking copy in 0x4000-byte chunks (osInvalDCache + osPiStartDma + osRecvMesg).
- `func_800172B8(start, end, ram)`: wrapper -> func_80017318.  `func_8000C0E0(rom, ram, size)`: single DMA (used by the inflater).
- `func_80051C04`: single DMA for the audio driver.  `func_80026920` (thread): streams `D_800A75E0` bytes from
  `D_80106874` into a heap block (speech clips).
- `func_80013560(stream, rom, size)` [func_800139D0]: gzip reader. Reads 0x4000-byte chunks (rounded up to 8) into
  0x8012FD50, skips the 10-byte gzip header and the NUL-terminated FNAME (it assumes FLG=0x08), and feeds raw deflate to
  `func_80013AD8` (zlib 1.0.x `inflate`, `func_8001602C` = huft_build). The trailer CRC32/ISIZE is not checked.
  `func_8001375C` sets up the z_stream: output 0x80154000, avail_out 0x86800, window 0x80134080 (32 KB).

Loaders:
- `func_8001D9D0(mode)` [func_8001E2CC]: level loader. Modes 21/22: raw copy of a screen file to 0x80154000. Level modes:
  inflate the level `.bif` to 0x80154000. Every mode then raw-copies the `common` block to 0x8024B000.
- `func_800170B0` [func_80017520]: libmus init. Copies the SFX pointer bank to 0x800B0CA0 and the FX bank to
  0x80115390; musConfig.wbk = SFX wave bank ROM address (ROM-resident, DMA on demand via func_8005095C).
- `func_8001892C(start, end, vol, ...)` [func_8001933C]: music: alloc+copy music pointer bank, MusPtrBankInitialize
  (wbk = music wave bank ROM address), alloc+copy the song [start,end), start it. Callers: func_80018A60,
  func_8003A650, func_8003AB7C [func_80019470, func_8003BF64, func_8003C494, func_800287AC].
- sprite_anim: func_800272C8 / func_80026B70 [func_800287AC / func_80027FE0] set D_80106874/D_800A75E0 and wake func_80026920.
- clip sequencer `func_80012FA0`: walks lists of `{s32 rom_addr; s16 ms; s16 pad}` from .data (-1 rom = silence,
  ms -1/-2 = indirect entry, ms 0 = end) and plays each clip with `func_80051314(rom, pan 0x40, vol 0x7FFF)` ->
  `func_80069440` (voice streamer). Letter tables around ROM 0x73F80 (RAM 0x800733C8...), Number 0x75700..0x75B30.

## Formats
- **gzip** (.bif, 8 per game): RFC 1952, FLG=0x08 (FNAME, e.g. `chute.bif`), MTIME 0, raw deflate, CRC32+ISIZE trailer
  all valid. Each member is followed by 1..15 leftover non-zero bytes up to the next 16-byte boundary (class `pad`,
  inside the loaded range, ignored). Not byte-reproducible with zlib (any level/memLevel): lossless proof is CRC32+ISIZE.
- **.bif payload**: a RAM image prelinked for 0x80154000 (max 0x86800 bytes): texture data first, then vertices and
  F3DEX display lists (G_VTX/G_TRI2/G_DL with absolute 0x8015xxxx-0x801Dxxxx pointers) and level tables. Owned by gfx.
- **common** (raw): prelinked RAM image for 0x8024B000 (starts with a G_ENDDL), same kind of content. gfx.
- **menu21/menu22** (raw): screen images: 0x200-byte TLUT + image per 0x12E00 slot, drawn by func_8001CAA8. gfx.
- **libmus banks/songs** (audio_bank, music): "N64 PtrTablesV2", "N64 WaveTables ", FX bank, songs with header word 0x215. audio/music agents.
- **MORT clip region** (speech): begins with magic "MORT"; clips addressed by absolute ROM address from .data tables. speech agent.
- **sprite_anim** (Letter 0x61AFB0.., Number 0x61F510..): whole-file RAM loads by thread func_80026920 (D_80106874 = rom, D_800A75E0 = size); owner sprA.
