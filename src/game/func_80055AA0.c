#include "common.h"

typedef struct {
    s32 unk_0;
    s32 unk_4;
    s16 unk_8;
    s32 unk_C;
    s32 unk_10;
} Msg;

typedef struct Node {
    u8 unk_0[0x8];
    struct Node *unk_8;
    s32 unk_C;
    u8 unk_10[0x4];
    s16 unk_14;
    u16 unk_16;
    u16 unk_18;
    s16 unk_1A;
    u8 unk_1C[0x6C];
    s32 unk_88;
} Node;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u8 unk_4;
} Def;

typedef struct {
    u8 unk_0[0x1C];
    s32 unk_1C;
} Ctx;

extern Ctx *D_80077BC4;
s32 func_80055BB8(Node **out, s16 id);
Msg *func_800563E8(void);
void n_alEnvmixerParam(Node *t, s32 kind, Msg *m);

/* initialises a node from a def, links it to a partner found by id and posts start messages; returns whether a partner exists */
s32 func_80055AA0(Node *n, Def *d) {
    Node *p = NULL;
    s32 ok;
    Msg *m;
    s32 t0;

    n->unk_16 = d->unk_0;
    n->unk_1A = d->unk_4;
    n->unk_C = 0;
    n->unk_18 = d->unk_2;
    n->unk_14 = 0;
    n->unk_8 = NULL;
    ok = func_80055BB8(&p, d->unk_0);
    if (p != NULL) {
        if (ok != 0) {
            p->unk_88 = 0x228;
            p->unk_8->unk_8 = NULL;
            p->unk_8 = n;
            n->unk_8 = p;
            m = func_800563E8();
            t0 = D_80077BC4->unk_1C;
            m->unk_8 = 0xB;
            m->unk_C = 0;
            m->unk_10 = 0x170;
            m->unk_4 = t0;
            n_alEnvmixerParam(n->unk_8, 3, m);
            m = func_800563E8();
            if (m != NULL) {
                s32 t = D_80077BC4->unk_1C + p->unk_88;
                m->unk_8 = 0xF;
                m->unk_0 = 0;
                m->unk_4 = t;
                n_alEnvmixerParam(n->unk_8, 3, m);
            }
        } else {
            p->unk_88 = 0;
            p->unk_8 = n;
            n->unk_8 = p;
        }
    }
    return p != NULL;
}
