# packC report: the `common` pack (4th file after the program image)

## Done
- Identified the file. It is the raw `common` RAM image at 0x8024B000, loaded for every level by the level loader
  (`func_8001D9D0` in Letter, `func_8001E2CC` in Number). Its pointers are absolute; see FORMAT.md.
- Every byte of both games is in `MAP.tsv` (header plus 835 Letter rows and 514 Number rows, tiling with no gaps).
- `tools/extract_packC.py` extracts: 135+57 display lists as GBI macro text, 68+38 vertex arrays as JSON,
  70+38 model headers, 70+38 OBJ meshes (10,084 and 5,178 triangles), 180 PNGs, and every texture and palette as `.bin`.
  `check` rebuilds both files byte-exactly from the text and JSON forms: **OK for Letter and Number**.
  Every display-list command assembled from a real macro; there are no `gsDPWord` fallbacks.

## Bytes per class
| class | Letter objs | Letter bytes | Number objs | Number bytes |
|---|---|---|---|---|
| texture | 216 | 288,264 | 124 | 181,776 |
| tlut | 216 | 81,312 | 128 | 32,416 |
| vtx | 68 | 228,304 | 38 | 133,056 |
| dl | 135 | 63,504 | 57 | 37,312 |
| morph_anim | 7 | 49,872 | 4 | 35,300 |
| table (vertex expansion lists) | 14 | 12,716 | 26 | 8,000 |
| model_hdr | 70 | 2,800 | 38 | 1,520 |
| bbox | 68 | 1,632 | 38 | 912 |
| coll_hdr / ptr_table / coll_quad | 2/2/8 | 688 | 2/2/8 | 688 |
| pad | 0 | 0 | 9 | 742 |
| unknown | 29 | 1,532 | 40 | 2,742 |
| **total** | | **730,624** | | **434,464** |

## Cross-checks
- The unique-vertex counts from the models' expansion lists match the vertex-morph animation files found by sprA:
  Letter 380/346/291 (headers at file offset 0x82988, 0x8F870, 0x88250) and Number 380/339/309 (0x4F898, 0x574C8,
  0x5D6D0). Those are the character meshes the morph files animate. Number's 214-vertex animation has no mesh in this pack.
- All 280 (Letter) and 137 (Number) LEVEL_REFS addresses into `common` fall at an object start or are noted inside
  an object. Example: Number `func_800287AC` reads and stores into the 380-vertex mesh's vertex array, for morphing.

## Not done / limits
- 124 Letter and 36 Number texture ranges are reached only by code-built display lists (HUD, letter/number glyphs,
  icons). The code picks their size and format, so they are dumped as `.bin` only, not as PNG. Their ends are the next
  known object or root, so zero tails are counted inside them, which is why Letter shows no `pad`. Decoding them needs
  the per-call SetTile/LoadBlock values from `func_80004010` [`func_80003E18`] and from the .data pointer list at Letter
  0x8006DDF0 [Number 0x8006F760]. Most are CI8 with a 256-colour palette or CI4 with a 16-colour palette.
- The OBJ files ignore matrices, morph and skinning (some meshes are posed by the expansion list plus animation),
  so animated characters come out in their bind/rest pose.
- The PNG decoder is a viewer only. Losslessness is proven on the `.bin` texels, not by re-encoding the PNGs.
- `unknown` (1.5 KB / 2.7 KB): small unreferenced tails between objects. Two larger ones:
  - Letter 0x1BF3D0-0x1BF6F0 (800 B) at the end of the file looks like an unreferenced Vtx array (16-byte records,
    flag 0, colour 0xFFE75FFF).
  - Letter 0x196CE0 (544 B) looks like an unreferenced RGBA16 palette.
  - Number 0x1717D8-0x172471 is a cluster of unreferenced palette and texel bytes.
  These are probably leftovers from the asset packer.

## Odd things
- The file starts with a lone `gsSPEndDisplayList` at 0x8024B000 (the address the loader DMAs to), followed by the
  RGBA32 fade tile. The first HUD tiles are separated by single 8-byte end-of-list commands (`B8000000 00000000`, at 0x1010, 0x2020, ...),
  which are kept inside the code-rooted texture ranges.
- Letter's copy is 1.7 times the size of Number's: Letter has 70 models, Number 38.
- No strings, build dates or debug text were found in this file.
