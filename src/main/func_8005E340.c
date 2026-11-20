typedef struct { u32 w0, w1; } Gfx;
#define GW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }
extern s16 D_80077F14;
extern s32 D_800A07E0;
extern s32 D_800A07E4;
extern s32 D_800A07E8;
extern s32 D_800A07EC;
/* reset screen state and emit RDP setup display list */
void func_8005E340(Gfx **gp) {
    Gfx *g = *gp;
    D_800A07E0 = 320;
    D_800A07E4 = 240;
    D_80077F14 = 0;
    D_800A07E8 = 0;
    D_800A07EC = 0;
    GW(g++, 0xE7000000, 0);
    GW(g++, 0xBA001402, 0);
    GW(g++, 0xBB000001, 0x80008000);
    GW(g++, 0xB9000002, 0);
    GW(g++, 0xBA001301, 0);
    GW(g++, 0xBA000C02, 0x2000);
    GW(g++, 0xBA000903, 0xC00);
    GW(g++, 0xBA001102, 0);
    GW(g++, 0xBA001001, 0);
    GW(g++, 0xBA000E02, 0);
    GW(g++, 0xB900031D, 0x0F0A7008);
    *gp = g;
}
