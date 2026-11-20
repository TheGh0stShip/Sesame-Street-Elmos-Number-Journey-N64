# gfx tools (shared library + extractor)

All scripts run with the lab Python through the job runner; none needs PIL (own PNG codec in `png.py`).

## Library
- `f3dex.py` - F3DEX 1.23 (GBI 1.x numbering: G_VTX=0x04, G_DL=0x06, G_MTX=0x01, TRI1=0xBF, TRI2=0xB1, ENDDL=0xB8 ...).
  - `disasm(w0, w1) -> str` one `gs*` macro; when a macro form would drop bits it prints `gsDPWord(w0, w1)`.
  - `assemble(text) -> (w0, w1)` inverse; `assemble(disasm(w0,w1)) == (w0,w1)` for every word pair.
  - `plausible(w0, w1)` heuristic used by the DL scanner.
  - `Tracer(resolve, segments)` symbolic run of a list: `run(addr)` follows G_DL, records `textures`
    (addr, fmt, bits, w, h, tlut), `vtx` loads, `tris` (with current texture index), segment writes (G_MOVEWORD 6).
    `resolve(addr, n)` returns bytes or None. Use `step(w0, w1, 0, 0)` to feed words built by code.
  - `python f3dex.py` = self-test (200k random words round trip).
- `n64tex.py` - texel codec for RGBA16, RGBA32, YUV16, CI4, CI8, IA4, IA8, IA16, I4, I8.
  - `decode(fmt, bits, w, h, texels, tlut_bytes=None) -> png.Img`; `encode(fmt, bits, img) -> (texels, tlut)`.
  - PNG modes: RGBA16/32 -> RGBA (5-bit channels bit-replicated), CI -> indexed PNG with PLTE+tRNS from the TLUT,
    IA -> LA, I -> L, YUV16 -> RGBA holding (Y,U,V,255) (lossless storage, not a colour conversion).
  - `python n64tex.py` = self-test: random texels/TLUTs of every format -> PNG file -> parse -> encode == original.
- `png.py` - `Img`, `encode`, `open_png` (all five PNG filters on read).
- `blobs.py` - image table of both games (main, common 0x8024B000, level images 0x80154000, gzip inflate).
- `gfxscan.py` - DL scan of an image, address space (main + common + one level).

## Extractor
- `extract_gfx.py ROM OUTDIR` -> `OUTDIR/<game>/{textures,dl,models,vtx,anim}`, `manifest.json`, `map.tsv`.
- `extract_gfx.py check ROM OUTDIR` -> re-encodes every PNG/vertex JSON/anim JSON and re-assembles every DL text, compares to ROM bytes.
- `gen_tables.py SRC GAME OUT.json` freezes code facts (command words written by C code, table bases, background calls,
  animation file list) from the matched sources into `gfx_<game>.json` (no ROM bytes). Already generated.
- `run_all.sh OUTDIR` does all of it for both games.
