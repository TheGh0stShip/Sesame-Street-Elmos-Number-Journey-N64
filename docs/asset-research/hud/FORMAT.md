# hud: code-only textures of the `common` block (and what packC mistook for them)

Territory: the ranges agent packC classed as `texture / code-set format` (124 Letter, 36 Number) plus the code-rooted
`tlut` ranges between them. `common` is loaded raw at RAM 0x8024B000 (ROM = RAM - 0x8024B000 + 0x10D0F0 Letter,
+ 0x10D560 Number). Addresses below are RAM; Number in [brackets]. Number addresses were confirmed from the
disassembly of func_80003E18 (lui/addiu pairs), not from source names.

## How the HUD draws (Letter func_80004010 [Number func_80003E18])
The function writes raw F3DEX words into the display list at D_8013EB0C. Each sprite is the same block:
```
FD100000 tlut          G_SETTIMG RGBA16 (palette address)
E8000000 0 / F5000100 07000000 / E6000000 0
F0000000 073FC000      G_LOADTLUT tile 7, 256 entries (always 256, even for CI4: it over-reads past a 16-colour TLUT)
E7000000 0
FD500000 texels        G_SETTIMG CI, 16b (block-load trick)
F5500000 07080200 / E6000000 0
F3000000 07sssddd      G_LOADBLOCK: (sss+1) 16-bit words, dxt ddd
E7000000 0
F5ffLL00 00080200      G_SETTILE tile 0: render format ff (0x48=CI8, 0x80=CI4), line LL
F2000000 00wwwhhh      G_SETTILESIZE: width=(www>>2)+1, height=(hhh>>2)+1
E4xxxyyy 00xxxyyy      G_TEXRECT, then B4 / B3 (dsdx,dtdy = 0x0400,0x0400: 1:1)
```
Size derivation: LoadBlock word count x 2 = texel bytes; SetTileSize gives w,h; SetTile gives CI4/CI8. The
RGBA32 tiles use `FD180000` (RGBA 32b), `F5180000 07094250`, LoadBlock 0x3FF (1024 texels), tile size 0x7C07C.

| element (HUD enable bit in D_8006EDD8 [D_8006FE58]) | texels | format | palette |
|---|---|---|---|
| bit 2: 64x64 picture as four 32x32 tiles, alpha pulsing via D_8006EDD0 | 0x8024C018, D028, E038, F048 | RGBA32 32x32 | - |
| bit 2: 48x48 picture as four 24x24 tiles | L 0x80250250/0690/0AD0/0F10 [0x80258550/8990/8DD0/9210] | CI8 24x24 (LoadBlock 0x11F) | 0x200 TLUT right before each |
| bits 4/0x200/0x400, 8, 0x800/0x1000: small icons | L 0x80251170, 1210, 12B0 [0x80259470, 9510, 95B0] | CI4 16x16 | 0x20 TLUT before each |
| bit 16: current glyph, big (mode 22 always entry 0) | TexPair table L 0x8006DFC8 [52] [N 0x8006F7E0 [9]] | CI8 32x32 (0x400 bytes) | pair .m0 (0x200) |
| bit 32: row of collected glyphs, small | TexPair table L 0x8006DE28 [52] [N 0x8006F798 [9]] | CI8 16x16 (0x100) | pair .m0 |
| bit 32: empty slot (count D_800ACBA0 exhausted) | L 0x8026E930 [0x80257370] | CI8 16x16 | L 0x8026E730 [0x80257170] |
| bits 64 and 256: icon chosen per slot by D_8013EB38[i].m4 [D_8013ECA8] | TexPair table L 0x8006DDF0 [7] [N 0x8006F760 [7]] | CI4 32x32 (0x200 bytes) | pair .m0 (0x20) |

Glyph index: Letter D_8012FD3C = ch-'A' (0..25), lowercase ch-'a'+26 (26..51) in modes 1/2; Number
D_8012FEC4 = ch-'1' (digits 1..9). TexPair = {u32 tlut, u32 texels}, RAM addresses (see ../data1/FORMAT.md).
The extractor reads the pair addresses from the ROM's .data at run time.

Screen-fade tile: Letter func_80018278 [Number func_800186E8] loads 0x8024B008 as RGBA32 32x32 (same tile words).

The other HUD tables used by the same functions (Letter D_8006E5A0/E6B0/E7C0/E8D0/EA78/EC20, Number
D_8006FC60/FCA8) and the menu drawers Letter func_8001A318 / func_8001CAA8 [Number func_8001AC1C / func_8001D3AC]
point into the level image at 0x80154000-0x801xxxxx, not into `common`: they are outside this territory.

## Layout details
- Every RGBA32 tile is preceded by 8 zero bytes (alignment) and an 8-byte empty display list `B8000000 00000000`
  (gsSPEndDisplayList); the first one is at 0x8024B000, the start of `common`. Nothing references these stubs
  (the level loader uses 0x8024B000 only as the copy destination). Class `dl`.
- Number has two more RGBA32 32x32 tiles in the same layout (0x80250058, 0x80251068) that nothing references.
- CI4 TLUTs are 0x20 bytes; G_LOADTLUT still loads 256 entries, but CI4 rendering with palette 0 uses only the first 16.

## Vertex morph animations (class `morph_anim`, not textures)
packC took ten ranges referenced only by .data words for textures. The words are entries of the per-actor-type
animation lists (Letter `D_80076B20[type] -> Anim*[]`, read by func_8003CF50 / func_8003BB00 and stored in actor+0x1BC;
Number func_8003D3D0). The data is the sprA morph format:
`u16 hdr_end (=8+nkey); u16 data_off (8-aligned); u8 ticks; u8 nkey; u16 flags; u8 key_ticks[nkey]; zero pad to
data_off; u32 frames[nunique][nkey]`, each word packed z:s10 (bits 31..22) | y:s11 (21..11) | x:s11 (10..0), signed (proven from func_80000450, r3/FORMATS_PROVEN.md §1). data_off is read from the file by the game (func_80000450, floored to 4); the packer always wrote ((8+nkey) & ~7) + 8 (= (hdr_end+8) & ~7; 1..8 pad bytes, 141/141).
nunique is not stored: it is the unique-vertex count of the model (vertex expansion list, ../packC/FORMAT.md); here it
is (distance to the next anim or object - data_off) / (4*nkey), which gives 371, 132 or 267 exactly, with 0, 4 or 8 zero
bytes left over (class pad).

## Extractor
`tools/extract_hud.py ROM GAME OUTDIR` writes `OUTDIR/GAME/tex/RAM_name.png` (CI as indexed PNG whose PLTE+tRNS is
the TLUT; RGBA32 as RGBA), `anim/RAM.json` (header fields, key ticks, header pad hex, frames as [x,y,z] per vertex per
key), `dl/RAM_enddl.gbi.txt`, `index.json` and `MAP_GAME.tsv`. `extract_hud.py check ROM GAME OUTDIR` re-encodes every
PNG (texels and TLUT), re-packs every anim JSON, re-assembles the DL stubs, zero-fills pads, compares each object with
the ROM and checks the objects tile the territory exactly. `tools/run.sh` does both games and writes MAP.tsv.
Codec: `tools/n64tex.py`, `tools/png.py` (copies of ../gfx/tools).
