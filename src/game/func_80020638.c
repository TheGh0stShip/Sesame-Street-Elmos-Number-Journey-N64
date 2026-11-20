#include "common.h"

typedef struct { u8 pad[0x18]; s32 unk_18; s32 unk_1C; s32 unk_20; } Node;
Mtx *func_80020758(s32 a, s32 b, MtxF *m, s32 c, Mtx *d);

/* Builds a node matrix, emits matrix + display list commands, recurses into children. */
Mtx *func_80020638(Mtx *arg0, Node *arg1, s32 arg2, Mtx *arg3) {
    s32 pad[2];
    MtxF mf;
    Gfx *g;

    func_8001FDA0(&mf, arg1, arg2);
    guMtxCatF(&mf, arg0, arg0);
    guMtxF2L(arg0, arg3);
    g = D_8013EC7C++;
    g->words.w0 = 0x01020040;
    g->words.w1 = (u32)arg3;
    g = D_8013EC7C++;
    g->words.w0 = 0x06000000;
    g->words.w1 = arg1->unk_20;
    arg3++;
    if (arg1->unk_18 != 0) {
        arg3 = func_80020758(arg1->unk_1C, arg1->unk_18, &mf, arg2, arg3);
    }
    return arg3;
}
