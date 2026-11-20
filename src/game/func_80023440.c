#include "common.h"

typedef struct { s32 unk_0; s32 unk_4; u32 unk_8; f32 unk_C; f32 unk_10; f32 unk_14; } S;
/* unpacks fields of an object into locals (results unused) */
void func_80023440(S *arg0) {
    S *p;
    f32 v[3];
    f32 sp18;
    u32 sp1C;
    f32 sp20;
    f32 sp24;
    s32 sp28;

    p = arg0;
    v[0] = p->unk_C;
    v[1] = p->unk_10;
    v[2] = p->unk_14;
    sp18 = p->unk_4;
    sp1C = p->unk_8 / 100000000;
    sp28 = p->unk_8 / 10000;
    sp24 = sp28 % 10000;
    sp20 = p->unk_8 % 10000;
}
