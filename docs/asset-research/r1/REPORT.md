# r1 report
All 24 ranges formerly unknown are identified (MAP.tsv, 37 rows); extractor check re-encodes every object byte-exactly (Letter 10/10, Number 13/13).
Letter ROM: 36 B alignment padding (five ranges); 0x1BF3D0-0x1BF5D0 vertex array and 0x1BF6B0-0x1BF6F0 bbox + model header of the last common-block model (used: listed in .data model tables); 0x196CE0 unused 32x32 CI4 image + palette.
Letter manor.bif 0x60B0: unused 256-colour palette + 32x64 CI8 image (a further strip of the level glowing arc).
Number ROM 0x1717D8-0x172499: six used 0x220 blocks (16-colour palette + 32x32 CI4), selected by func_8003670C per level and object state; packC had split them.
Number bump.bif: four 0x600 blocks = four palette variants of one 32x32 CI8 gradient tile (not all zero), unused; 0x4FB80 = byte copy of a texture Letter planet.bif uses (sister-game leftover).
Caveat: pointer search covers code address pairs, all .data words and the containing images; an address computed at run time from another base would not be found.
