typedef struct { u32 w0, w1; } Gfx;
typedef struct { u8 pad[0x48]; u32 unk_48; } S73B4;
extern S73B4 *D_80077BC4;
Gfx *func_80056610(void);
/* appends two display-list commands (0x0D, 0x06 segment) after func_80056610's list */
Gfx *func_80056570(void) {
    Gfx *g = func_80056610();
    { Gfx *_g = g++; _g->w0 = 0x0D000000; }
    { Gfx *_g = g++; _g->w0 = 0x062E0000; _g->w1 = D_80077BC4->unk_48; }
    return g;
}
