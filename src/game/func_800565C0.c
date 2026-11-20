#include "common.h"

typedef struct { u8 pad[0x24]; s32 arr[1]; } Sub;
typedef struct { u8 pad[0x34]; Sub *unk_34; } Obj;
extern Obj *D_80077BC4;
void n_alFxNew(s32 *);
s32 func_800565C0(s16 idx) {
    n_alFxNew(&D_80077BC4->unk_34->arr[idx]);
    return D_80077BC4->unk_34->arr[idx];
}
