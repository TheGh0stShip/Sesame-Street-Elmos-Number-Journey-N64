typedef struct { u32 w0; u32 w1; } Gfx;
#define GFX2(pkt, a, b) { g = (pkt)++; g->w0 = (a); g->w1 = (b); }

extern u8 D_800BD860;
extern u8 *D_8019F170[];

/* advances an animation frame counter (0..15) and emits a display list loading two animated textures */
void func_80025DF0(Gfx *gdl) {
    Gfx *g;
    s32 unused[2];
    if (++D_800BD860 == 16) {
        D_800BD860 = 0;
    }
    GFX2(gdl, 0xE7000000, 0);
    GFX2(gdl, 0xBA000E02, 0x8000);
    GFX2(gdl, 0xFD100000, (u32)D_8019F170[(u8)(D_800BD860 >> 1)]);
    GFX2(gdl, 0xE8000000, 0);
    GFX2(gdl, 0xF5000100, 0x07000000);
    GFX2(gdl, 0xE6000000, 0);
    GFX2(gdl, 0xF0000000, 0x0703C000);
    GFX2(gdl, 0xE7000000, 0);
    GFX2(gdl, 0xFD500000, (u32)(D_8019F170[(u8)(D_800BD860 >> 1)] + 0x20));
    GFX2(gdl, 0xF5500000, 0x07014050);
    GFX2(gdl, 0xE6000000, 0);
    GFX2(gdl, 0xF3000000, 0x070FF400);
    GFX2(gdl, 0xE7000000, 0);
    GFX2(gdl, 0xF5400400, 0x00014050);
    GFX2(gdl, 0xF2000000, 0x0007C07C);
    GFX2(gdl, 0xB8000000, 0);
}
