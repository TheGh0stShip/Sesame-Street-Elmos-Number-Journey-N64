#include "common.h"

extern u8 *D_800A05E4;
s32 func_800614C0(s32, Arg *);
void osSendMesg(s32, Task *, s32);
void osRecvMesg(void *, s32, s32);
/* builds a task descriptor from arg and submits it */
void func_80052530(Arg *a) {
    Task t;
    s32 w;
    s32 buf[8];
    t.unk_00 = 0;
    t.unk_54 = buf;
    t.unk_08 = 2;
    t.unk_50 = (s32)(D_800A05E4 + 0x30);
    t.unk_40 = a->unk_0;
    t.unk_44 = a->unk_4;
    t.unk_10 = 2;
    t.unk_18 = D_8006CD00;
    t.unk_1C = D_8006CDD0 - D_8006CD00;
    t.unk_14 = 0;
    t.unk_20 = a->unk_8;
    w = a->unk_C;
    t.unk_24 = 0x1000;
    t.unk_2C = 0x800;
    t.unk_30 = 0;
    t.unk_34 = 0;
    t.unk_38 = 0;
    t.unk_3C = 0;
    t.unk_48 = 0;
    t.unk_4C = 0;
    t.unk_28 = w;
    osSendMesg(func_800614C0(D_800A05E0, a), &t, 1);
    osRecvMesg(D_800A05E4 + 0x30, 0, 1);
}
