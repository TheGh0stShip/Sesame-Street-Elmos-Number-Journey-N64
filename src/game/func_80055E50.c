#include "common.h"

typedef struct { s32 unk_00; s32 unk_04; Tgt *unk_08; } Owner;
typedef struct { u8 pad[0x1C]; s32 unk_1C; } Ctx;
extern Ctx *D_80077BC4;
Ev *func_800563E8(void);
void n_alEnvmixerParam(Tgt *, s32, Ev *);
/* Queues event type 11 for the owner's target. */
void func_80055E50(Owner *o, s16 a, s32 b) {
    Ev *e;
    if (o->unk_08 != NULL) {
        e = func_800563E8();
        if (e != NULL) {
            s32 t = D_80077BC4->unk_1C + o->unk_08->unk_88;
            e->unk_08 = 11;
            e->unk_0C = a;
            e->unk_04 = t;
            e->unk_10 = ((s32 (*)(s32, s32))func_80056514)(b, t);
            e->unk_00 = 0;
            n_alEnvmixerParam(o->unk_08, 3, e);
        }
    }
}
