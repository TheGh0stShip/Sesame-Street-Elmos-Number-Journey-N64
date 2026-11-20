#include "common.h"

#define G(a, b) { _g = (Gfx *)(gdl++); _g->words.w0 = (a); _g->words.w1 = (b); }
/* emits a display list that loads an animated texture frame (13 frames, advances every other call) */
void func_80025AD0(Gfx *gdl) {
    Gfx *_g; s32 pad[2];
    if (++D_800BD860 == 26) {
        D_800BD860 = 0;
    }
    G(0xE7000000, 0);
    G(0xBA000E02, 0x8000);
    G(0xFD100000, D_801B3C90[(u8)(D_800BD860 >> 1)]);
    G(0xE8000000, 0);
    G(0xF5000100, 0x07000000);
    G(0xE6000000, 0);
    G(0xF0000000, 0x0703C000);
    G(0xE7000000, 0);
    G(0xFD500000, D_801B3C90[(u8)(D_800BD860 >> 1)] + 0x20);
    G(0xF5500000, 0x07014050);
    G(0xE6000000, 0);
    G(0xF3000000, 0x070FF400);
    G(0xE7000000, 0);
    G(0xF5400400, 0x00014050);
    G(0xF2000000, 0x0007C07C);
    G(0xB8000000, 0);
}
