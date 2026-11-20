#include "common.h"

/* if idx is in range, act on entry idx of D_800778D4 and notify the player; 0 on success, 1 if out of range */

extern void *D_800778D4[];
typedef struct { char pad0[0x20]; void *unk_20; } T;
typedef struct { char pad0[0x34]; T *unk_34; } U;
extern U *D_8013C6F8;
s32 ChangeCustomEffect(s32 idx)
{
    s32 ret;
    if (idx < D_800778D0) {
        func_80051C30(D_800778D4[idx]);
        { T *t = D_8013C6F8->unk_34; func_8005BE24(t->unk_20, 1, t); }
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}
