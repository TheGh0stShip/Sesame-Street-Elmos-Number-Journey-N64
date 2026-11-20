typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { u8 pad[0x80C8]; void *unk_80C8; } GfxCtx;
extern Gfx *D_8013EC7C;
extern void *D_8013EF28;
extern Gfx D_800775F0[];
extern Gfx D_800774F0[];
extern Gfx D_800775D8[];
extern s32 D_8006F6D4;
extern u16 D_801DA800[];
u32 func_80059460(void *);

/* Emits the frame's setup display list: segments, depth buffer, clears and colour image. */
void func_80001818(void *arg) {
    GfxCtx *ctx;
    Gfx *g;

    ctx = arg;
    g = D_8013EC7C++;
    g->w0 = 0xBC000006;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xBC000806;
    g->w1 = func_80059460(D_8013EF28);
    g = D_8013EC7C++;
    g->w0 = 0x06000000;
    g->w1 = (u32)D_800775F0;
    if (D_8006F6D4 != 0) {
        g = D_8013EC7C++;
        g->w0 = 0x06000000;
        g->w1 = (u32)D_800774F0;
        D_8006F6D4 = 0;
    }
    g = D_8013EC7C++;
    g->w0 = 0x06000000;
    g->w1 = (u32)D_800775D8;
    g = D_8013EC7C++;
    g->w0 = 0xFE000000;
    g->w1 = func_80059460(D_801DA800);
    g = D_8013EC7C++;
    g->w0 = 0xBA001402;
    g->w1 = 0x300000;
    g = D_8013EC7C++;
    g->w0 = 0xFF10013F;
    g->w1 = func_80059460(D_801DA800);
    g = D_8013EC7C++;
    g->w0 = 0xF7000000;
    g->w1 = -1;
    g = D_8013EC7C++;
    g->w0 = 0xF64FC3BC;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xFF10013F;
    g->w1 = func_80059460(ctx->unk_80C8);
}
