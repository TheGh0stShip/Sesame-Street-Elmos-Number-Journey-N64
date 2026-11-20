typedef struct { u32 w0; u32 w1; } Gfx;
extern u16 D_80077F14;

#define CMD(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

/* appends render-mode setup commands to a display list */
void func_8005F16C(Gfx **pp) {
    Gfx *g = *pp;

    CMD(g++, 0xBB000000, 0x80008000);
    CMD(g++, 0xFCFFFFFF, 0xFFFE793C);
    if (D_80077F14 & 1) {
        CMD(g++, 0xB900031D, 0x0F0A4000);
    }
    if (D_80077F14 & 2) {
        CMD(g++, 0xB9000002, 0);
    }
    CMD(g++, 0xB8000000, 0);
    *pp = g;
}
