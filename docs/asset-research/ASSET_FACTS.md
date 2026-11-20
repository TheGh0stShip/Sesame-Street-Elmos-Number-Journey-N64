# Layout facts from the kit survey (both games)

## letter
- ROM file: `${N64DK_LAB}/roms/` (the letter title), 8 MB, big-endian z64. RAM address = ROM offset + 0x7FFFF400 inside the main segment.
- main .text: RAM 0x80000400-0x8000043C, ROM 0x1000-0x103C, 60 bytes
- main .data: RAM 0x8000043C-0x80000450, ROM 0x103C-0x1050, 20 bytes
- main .text: RAM 0x80000450-0x8006C9AC, ROM 0x1050-0x6D5AC, 443740 bytes
- main .data: RAM 0x8006C9AC-0x800785B0, ROM 0x6D5AC-0x791B0, 48132 bytes
- main .rodata: RAM 0x800785B0-0x8007BAB0, ROM 0x791B0-0x7C6B0, 13568 bytes
- main .bss: RAM 0x8007BAB0-0x80140FA0, ROM ---, 808176 bytes
- RSP microcode inside the main segment: [{"vram": "0x8006B780", "vram_end": "0x8006BAC0", "kind": "rsp", "note": "RSP microcode or its data inside the program image: not R4300 code"}, {"vram": "0x8006BD40", "vram_end": "0x8006C800", "kind": "rsp", "note": "RSP microcode or its data inside the program image: not R4300 code"}, {"vram": "0x8006CB00", "vram_end": "0x8006D4C0", "kind": "rsp", "note": "RSP microcode or its data inside the pro
- microcode id: ['F3DEX 1.23 @0x798B0']; SDK: ['2.0D/2.0E/2.0F/2.0G/2.0H:libgultra', '2.0I:libgultra']
- audio (kit scan): {"containers": 2, "samples": 187, "midi": 0, "intervals": 1027}; graphics: {"family": "f3dex", "display_lists": 152}
- bytes by class (kit scan): {"-/-": 1324164, "-/fill_00": 781516, "audio/libmus.ADPCMBook": 25432, "audio/libmus.ADPCMloop": 2068, "audio/libmus.BaseNotes": 187, "audio/libmus.Detunes": 748, "audio/libmus.PtrTablesHeader": 96, "audio/libmus.WaveList": 748, "audio/libmus.WaveTable": 3740, "audio/libmus.WaveTablesHeader": 32, "audio/libmus.adpcm_sample": 1920222, "compressed/gzip": 1287202, "graphics/display_list": 76080, "loader_range/raw": 2456757, "program/.data": 45656, "program/.rodata": 13568, "program/.text": 440216, "program/rsp_text": 6080, "rom_metadata/header": 64, "rom_metadata/ipl3": 4032}
- largest unknown: ["ROM 0x4D9D78..0x61AFB0 (0x141238 bytes) is unclassified; 7.6 bits/byte, zlib ratio 0.89: raw media or lightly packed data (audio, textures, animation)", "ROM 0x2F9B52..0x2FBD50 (0x21fe bytes) is unclassified; 6.1 bits/byte, zlib ratio 0.41: raw structured data (tables, text, level data)", "review function boundaries flagged in functions.toml: 4 cross_branch, 1 branch_outside, 1 falls_through"]
- ROM offsets referenced by the game code as `D_00xxxxxx` symbols (asset tables/files): 0007C6B0 000A54D0 000A8D50 0010D0F0 001BF6F0 00204D00 00227BA0 00243E20 002604B0 0028FDB0 002ACFA0 002D1FB0 002F9B60 002FBD50 002FD600 002FE8D0 003E3FB0 003E4830 003EA160 004D9D80 004DB3E0 004DD320 004E32C0 004EA530 004EADF0 004F1850 004F2030 004FA460 004FAC20 00501130 00501820 00507070 00507890 0050C970 0050D000 0050F9E0 0061AFB0 00620ED0 00624460 00633220 00644970 00657E70 00668FD0 0067BEE0 0068B260 00698AE0 006A0760 006A2D50 006B4150 006C4A80 006D5E80 006E3190 006F19C0 006F9020 0070A5D0 00718E00 0071F230 007307E0 0073CBA0 0073F930 00741920
- last non-padding ROM byte: 0x741914

## number
- ROM file: `${N64DK_LAB}/roms/` (the number title), 8 MB, big-endian z64. RAM address = ROM offset + 0x7FFFF400 inside the main segment.
- main .text: RAM 0x80000400-0x8000043C, ROM 0x1000-0x103C, 60 bytes
- main .data: RAM 0x8000043C-0x80000450, ROM 0x103C-0x1050, 20 bytes
- main .text: RAM 0x80000450-0x8006E31C, ROM 0x1050-0x6EF1C, 450252 bytes
- main .data: RAM 0x8006E31C-0x80078DC0, ROM 0x6EF1C-0x799C0, 43684 bytes
- main .rodata: RAM 0x80078DC0-0x8007BF20, ROM 0x799C0-0x7CB20, 12640 bytes
- main .bss: RAM 0x8007BF20-0x80140F60, ROM ---, 806976 bytes
- RSP microcode inside the main segment: [{"vram": "0x8006D100", "vram_end": "0x8006D440", "kind": "rsp", "note": "RSP microcode or its data inside the program image: not R4300 code"}, {"vram": "0x8006D680", "vram_end": "0x8006EE40", "kind": "rsp", "note": "RSP microcode or its data inside the program image: not R4300 code"}]
- microcode id: ['F3DEX 1.23 @0x7A0C0']; SDK: ['2.0D/2.0E/2.0F/2.0G/2.0H:libgultra', '2.0I:libgultra']
- audio (kit scan): {"containers": 2, "samples": 169, "midi": 0, "intervals": 971}; graphics: {"family": "f3dex", "display_lists": 73}
- bytes by class (kit scan): {"-/-": 1765055, "-/fill_00": 205154, "audio/libmus.ADPCMBook": 22984, "audio/libmus.ADPCMloop": 2772, "audio/libmus.BaseNotes": 169, "audio/libmus.Detunes": 676, "audio/libmus.PtrTablesHeader": 96, "audio/libmus.WaveList": 676, "audio/libmus.WaveTable": 3380, "audio/libmus.WaveTablesHeader": 32, "audio/libmus.adpcm_sample": 1837008, "compressed/gzip": 1505535, "graphics/display_list": 37336, "graphics/dl_blob": 16456, "loader_range/raw": 2480527, "program/.data": 40852, "program/.rodata": 12640, "program/.text": 446252, "program/rsp_text": 6912, "rom_metadata/header": 64, "rom_metadata/ipl3": 4032}
- largest unknown: ["ROM 0x4B00C0..0x61F510 (0x16f450 bytes) is unclassified; 7.5 bits/byte, zlib ratio 0.88: raw media or lightly packed data (audio, textures, animation)", "ROM 0x739C20..0x75F380 (0x25760 bytes) is unclassified; 6.9 bits/byte, zlib ratio 0.74: raw media or lightly packed data (audio, textures, animation)", "ROM 0x79CD90..0x7AB810 (0xea80 bytes) is unclassified; 7.0 bits/byte, zlib ratio 0.83: raw media or lightly packed data (audio, textures, animation)"]
- ROM offsets referenced by the game code as `D_00xxxxxx` symbols (asset tables/files): 0007CB20 000A5940 000A8D50 0010D560 00177680 001BCE90 001D6CA0 00206CC0 0022EC10 0025AF80 0027ADC0 002B5120 002B8FD0 002E6FC0 002FBD50 002FE8D0 003CB880 003D0980 003E3FB0 003E4830 004B00C0 004B8E70 004BA590 004BC400 004BCB50 004C12F0 004C1970 004C6350 004C69A0 004CB8D0 004CBCD0 004CFEC0 004D0690 004D7C90 004D83F0 004DF7C0 004E0870 004E1090 004E7620 004E9810 0061F510 00625430 006289C0 00637780 006482F0 006505B0 006634C0 00674C10 0067C2F0 00687B30 00691BB0 006A44D0 006A99B0 006B92E0 006C6700 006D25E0 006E3450 006F8CF0 00701190 00712000 00723E50 00732CF0 00739C20 0075F380 007710B0 00780C00 0078E0B0 0079CD90 007AB810 007B35C0 007BB850 007C57E0 007CE410
- last non-padding ROM byte: 0x7CE410
