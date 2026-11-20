#include "common.h"

s32 MusStartEffect(s32 arg0) {
    Snd *p;
    s32 r;
    p = D_800A057C;
    if (p == NULL) {
        p = D_800A0578;
        if (p == NULL) { D_800A0568 = 0; return 0; }
    } else {
        D_800A057C = NULL;
    }
    if (D_800A0568 == 0) D_800A0568 = p->unk_10;
    r = func_80050968(p, arg0, 0x80, 0x80, -1);
    D_800A0568 = 0;
    return r;
}
