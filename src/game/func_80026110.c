#include "common.h"

s32 func_80025AD0();
s32 func_80025DF0();

void func_80026110(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = arg0;
    if ((*(s32 *)sp10) == 4) {
        goto L13;
    }
    func_80059540(((s32)&D_8007A7D0), ((s32)&D_8007A7D4), 350);
L13: ;
    sp14 = func_80042080(80);
    switch (sp14) {
    case 0:
        func_80059540(((s32)&D_8007A7D0), ((s32)&D_8007A7D4), 354);
    }
    ((struct gd_10_s32 *)sp14)->v = (s32)&D_800274AC;
    ((struct gd_14_s32 *)sp14)->v = (s32)&D_80027D4C;
    ((struct gd_18_s32 *)sp14)->v = 0;
    ((struct gd_1C_s32 *)sp14)->v = 0;
    ((struct gd_20_s32 *)sp14)->v = 0;
    ((struct gd_8_s16 *)sp14)->v = ((struct gd_2_u16 *)sp10)->v;
    ((struct gd_A_s16 *)sp14)->v = ((struct gd_6_u16 *)sp10)->v;
    ((struct gd_C_s32 *)sp14)->v = ((struct gd_8_s32 *)sp10)->v;
    ((struct gd_24_s32 *)sp14)->v = 0;
    ((struct gd_40_s32 *)sp14)->v = 0;
    ((struct gd_48_s32 *)sp14)->v = 0;
    ((struct gd_4C_s16 *)sp14)->v = (func_80021640() & 180) + 60;
    (*(s32 (*))&D_800BD860) = 0;
    D_8011540C = 0;
    D_8013C7C4 = 0;
    D_800B1094 = 0;
    func_80041A80(sp14, D_80115408, 4, 0);
    switch (D_8012F154) {
    case 1:
        ((struct gd_44_s32 *)sp14)->v = func_800179D8(9);
        func_80046250(((s32)&D_801A9E50), ((s32)&D_801A9E44), 2, 0);
        return;
    case 9:
        ((struct gd_44_s32 *)sp14)->v = func_800179D8(17);
        func_80046250(((s32)&D_8017C6D8), ((s32)&D_8017C6CC), 2, 0);
        return;
    case 10:
        ((struct gd_44_s32 *)sp14)->v = func_800179D8(17);
        func_80046250(((s32)&D_80173B00), ((s32)&D_80173AF4), 2, 0);
        return;
    case 8:
        func_80046250(((s32)&D_80176BF0), ((s32)&D_80176BE4), 2, 0);
        return;
    case 7:
        D_8012FDE0 = func_80042080(800);
        D_8010E640 = func_80042080(800);
        func_80025DF0(D_8012FDE0);
        func_80025DF0(D_8010E640);
        D_8012FEB0 = D_8012FDE0;
        func_80046250(((s32)&D_8018E520), ((s32)&D_8018E514), 2, 0);
        return;
    case 6:
        func_80046250(((s32)&D_80175780), ((s32)&D_80175774), 2, 0);
        return;
    case 5:
        D_8012FDE0 = func_80042080(800);
        D_8010E640 = func_80042080(800);
        func_80025AD0(D_8012FDE0);
        func_80025AD0(D_8010E640);
        D_8012FEB0 = D_8012FDE0;
        func_80046250(((s32)&D_80197440), ((s32)&D_80197434), 2, 0);
        return;
    case 11:
        func_80046250(((s32)&D_80167C58), ((s32)&D_80167C4C), 2, 0);
        return;
    }
}
