# lvlF report
Territory: Letter garden, space, planet, bonusabc: 412,888 bytes previously uncovered, all classified in MAP.tsv, none unknown; check mode rebuilds every range byte-exactly.
Classes (bytes): hud_texture 181,760; hud_palette 6,048; morph_anim 170,372; morph_vertex_order 21,368; texframe texture+palette 25,600; unused texture+palette 5,632; model_bounds 408; model_header_ext 68; collision quads 260; display_list 48; pad 1,324.
Unused: bonusabc 4 palette+32x32 cards and 2 64x32 images (0x2EB30, 0x30C10) referenced by no table; space 3 palette+CI8 textures (0xC40, 0x17D70, 0x18D70) nothing loads (sizes inferred from gaps).
Not proven from code: meaning of model_bounds; morph_anim vertex counts (from item size, cross-checked against vertex-order lists where checked).
Formats: FORMAT.md.
