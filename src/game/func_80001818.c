#include "common.h"

extern Gfx D_800775F0[];
extern Gfx D_800774F0[];
extern Gfx D_800775D8[];

/* Emits the frame's setup display list: segments, depth buffer, clears and colour image. */
void func_80001818(void *arg) {
    GfxCtx *ctx;
    Gfx *g;

    ctx = arg;
    g = D_8013EC7C++;
    g->words.w0 = 0xBC000006;
    g->words.w1 = 0;
    g = D_8013EC7C++;
    g->words.w0 = 0xBC000806;
    g->words.w1 = osVirtualToPhysical(D_8013EF28);
    g = D_8013EC7C++;
    g->words.w0 = 0x06000000;
    g->words.w1 = (u32)D_800775F0;
    if (D_8006F6D4 != 0) {
        g = D_8013EC7C++;
        g->words.w0 = 0x06000000;
        g->words.w1 = (u32)D_800774F0;
        D_8006F6D4 = 0;
    }
    g = D_8013EC7C++;
    g->words.w0 = 0x06000000;
    g->words.w1 = (u32)D_800775D8;
    g = D_8013EC7C++;
    g->words.w0 = 0xFE000000;
    g->words.w1 = osVirtualToPhysical(D_801DA800);
    g = D_8013EC7C++;
    g->words.w0 = 0xBA001402;
    g->words.w1 = 0x300000;
    g = D_8013EC7C++;
    g->words.w0 = 0xFF10013F;
    g->words.w1 = osVirtualToPhysical(D_801DA800);
    g = D_8013EC7C++;
    g->words.w0 = 0xF7000000;
    g->words.w1 = -1;
    g = D_8013EC7C++;
    g->words.w0 = 0xF64FC3BC;
    g->words.w1 = 0;
    g = D_8013EC7C++;
    g->words.w0 = 0xFF10013F;
    g->words.w1 = osVirtualToPhysical(ctx->unk_80C8);
}
