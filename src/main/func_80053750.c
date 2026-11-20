typedef struct { u32 w0, w1; } Gfx;
typedef struct {
    u8 pad0[0x14];
    s32 unk_14;
    u8 pad18[4];
    void **unk_1C;
} S34;
typedef struct { u8 pad[0x34]; S34 *unk_34; } S73B4;
extern S73B4 *D_80077BC4;
Gfx *func_80053D10(void *item, s32 arg, Gfx *g);
/* emits a display-list header then each item of the current list via func_80053D10 */
Gfx *func_80053750(s32 arg0, Gfx *g) {
    S34 *s = D_80077BC4->unk_34;
    void **items = s->unk_1C;
    s32 i;

    { Gfx *_g = g++; _g->w0 = 0x020007C0; _g->w1 = 0x2E0; }
    for (i = 0; i < s->unk_14; i++) {
        g = func_80053D10(items[i], arg0, g);
    }
    return g;
}
