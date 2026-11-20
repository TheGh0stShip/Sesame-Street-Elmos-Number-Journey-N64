#include "common.h"

s32 func_80015380(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;

    sp38 = ((struct gd_4_s32 *)arg0)->v;
    sp24 = *(s32 *)arg1;
    sp28 = ((struct gd_4_s32 *)arg1)->v;
    sp1C = ((struct gd_20_s32 *)arg0)->v;
    sp20 = ((struct gd_1C_s32 *)arg0)->v;
    sp2C = ((struct gd_34_s32 *)arg0)->v;
    sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
    for (;;) {
        goto L36;
        return;
L36: ;
        switch (*(s32 *)sp38) {
        case 0:
            ((struct gd_C_s32 *)sp38)->v = ((struct gd_10_u8 *)sp38)->v;
            ((struct gd_8_s32 *)sp38)->v = ((struct gd_14_s32 *)sp38)->v;
            (*(s32 *)sp38) = 1;
        case 1:
            sp10 = ((struct gd_C_s32 *)sp38)->v;
            while (!((u32)sp20 >= (u32)sp10)) {
                if (sp28 != 0) {
                    arg2 = 0;
                } else {
                    ((struct gd_20_s32 *)arg0)->v = sp1C;
                    ((struct gd_1C_s32 *)arg0)->v = sp20;
                    ((struct gd_4_s32 *)arg1)->v = sp28;
                    ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                    (*(s32 *)arg1) = sp24;
                    ((struct gd_34_s32 *)arg0)->v = sp2C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp28--;
                sp1C = sp1C | ((*(u8 *)(sp24++)) << sp20);
                sp20 = sp20 + 8;
            }
            sp14 = ((struct gd_8_s32 *)sp38)->v + ((sp1C & (*(s32 *)((sp10 * 4) + ((s32)&D_80074F40)))) * 8);
            sp1C = (u32)sp1C >> (u32)((struct gd_1_u8 *)sp14)->v;
            sp20 = sp20 - ((struct gd_1_u8 *)sp14)->v;
            sp18 = *(u8 *)sp14;
            if (sp18 == 0) {
                ((struct gd_8_s32 *)sp38)->v = ((struct gd_4_s32 *)sp14)->v;
                (*(s32 *)sp38) = 6;
                break;
            }
            if ((sp18 & 16) != 0) {
                ((struct gd_8_s32 *)sp38)->v = sp18 & 15;
                ((struct gd_4_s32 *)sp38)->v = ((struct gd_4_s32 *)sp14)->v;
                (*(s32 *)sp38) = 2;
                break;
            }
            if ((sp18 & 64) == 0) {
                ((struct gd_C_s32 *)sp38)->v = sp18;
                ((struct gd_8_s32 *)sp38)->v = ((struct gd_4_s32 *)sp14)->v;
                break;
            }
            if ((sp18 & 32) != 0) {
                (*(s32 *)sp38) = 7;
                break;
            }
            (*(s32 *)sp38) = 9;
            arg2 = -3;
            ((struct gd_20_s32 *)arg0)->v = sp1C;
            ((struct gd_1C_s32 *)arg0)->v = sp20;
            ((struct gd_4_s32 *)arg1)->v = sp28;
            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
            (*(s32 *)arg1) = sp24;
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            return func_80013CBC(arg0, arg1, arg2);
        case 2:
            sp10 = ((struct gd_8_s32 *)sp38)->v;
            while (!((u32)sp20 >= (u32)sp10)) {
                if (sp28 != 0) {
                    arg2 = 0;
                } else {
                    ((struct gd_20_s32 *)arg0)->v = sp1C;
                    ((struct gd_1C_s32 *)arg0)->v = sp20;
                    ((struct gd_4_s32 *)arg1)->v = sp28;
                    ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                    (*(s32 *)arg1) = sp24;
                    ((struct gd_34_s32 *)arg0)->v = sp2C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp28--;
                sp1C = sp1C | ((*(u8 *)(sp24++)) << sp20);
                sp20 = sp20 + 8;
            }
            ((struct gd_4_s32 *)sp38)->v = ((struct gd_4_s32 *)sp38)->v + (sp1C & (*(s32 *)((sp10 * 4) + ((s32)&D_80074F40))));
            sp1C = (u32)sp1C >> (u32)sp10;
            sp20 = sp20 - sp10;
            ((struct gd_C_s32 *)sp38)->v = ((struct gd_11_u8 *)sp38)->v;
            ((struct gd_8_s32 *)sp38)->v = ((struct gd_18_s32 *)sp38)->v;
            (*(s32 *)sp38) = 3;
        case 3:
            sp10 = ((struct gd_C_s32 *)sp38)->v;
            while (!((u32)sp20 >= (u32)sp10)) {
                if (sp28 != 0) {
                    arg2 = 0;
                } else {
                    ((struct gd_20_s32 *)arg0)->v = sp1C;
                    ((struct gd_1C_s32 *)arg0)->v = sp20;
                    ((struct gd_4_s32 *)arg1)->v = sp28;
                    ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                    (*(s32 *)arg1) = sp24;
                    ((struct gd_34_s32 *)arg0)->v = sp2C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp28--;
                sp1C = sp1C | ((*(u8 *)(sp24++)) << sp20);
                sp20 = sp20 + 8;
            }
            sp14 = ((struct gd_8_s32 *)sp38)->v + ((sp1C & (*(s32 *)((sp10 * 4) + ((s32)&D_80074F40)))) * 8);
            sp1C = (u32)sp1C >> (u32)((struct gd_1_u8 *)sp14)->v;
            sp20 = sp20 - ((struct gd_1_u8 *)sp14)->v;
            sp18 = *(u8 *)sp14;
            if ((sp18 & 16) != 0) {
                ((struct gd_8_s32 *)sp38)->v = sp18 & 15;
                ((struct gd_C_s32 *)sp38)->v = ((struct gd_4_s32 *)sp14)->v;
                (*(s32 *)sp38) = 4;
                break;
            }
            if ((sp18 & 64) == 0) {
                ((struct gd_C_s32 *)sp38)->v = sp18;
                ((struct gd_8_s32 *)sp38)->v = ((struct gd_4_s32 *)sp14)->v;
                break;
            }
            (*(s32 *)sp38) = 9;
            arg2 = -3;
            ((struct gd_20_s32 *)arg0)->v = sp1C;
            ((struct gd_1C_s32 *)arg0)->v = sp20;
            ((struct gd_4_s32 *)arg1)->v = sp28;
            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
            (*(s32 *)arg1) = sp24;
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            return func_80013CBC(arg0, arg1, arg2);
        case 4:
            sp10 = ((struct gd_8_s32 *)sp38)->v;
            while (!((u32)sp20 >= (u32)sp10)) {
                if (sp28 != 0) {
                    arg2 = 0;
                } else {
                    ((struct gd_20_s32 *)arg0)->v = sp1C;
                    ((struct gd_1C_s32 *)arg0)->v = sp20;
                    ((struct gd_4_s32 *)arg1)->v = sp28;
                    ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                    (*(s32 *)arg1) = sp24;
                    ((struct gd_34_s32 *)arg0)->v = sp2C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp28--;
                sp1C = sp1C | ((*(u8 *)(sp24++)) << sp20);
                sp20 = sp20 + 8;
            }
            ((struct gd_C_s32 *)sp38)->v = ((struct gd_C_s32 *)sp38)->v + (sp1C & (*(s32 *)((sp10 * 4) + ((s32)&D_80074F40))));
            sp1C = (u32)sp1C >> (u32)sp10;
            sp20 = sp20 - sp10;
            (*(s32 *)sp38) = 5;
        case 5:
            sp34 = ((u32)(sp2C - ((struct gd_28_s32 *)arg0)->v) < (u32)((struct gd_C_s32 *)sp38)->v) ? (((struct gd_2C_s32 *)arg0)->v - (((struct gd_C_s32 *)sp38)->v - (sp2C - ((struct gd_28_s32 *)arg0)->v))) : (sp2C - ((struct gd_C_s32 *)sp38)->v);
            while (!(((struct gd_4_s32 *)sp38)->v == 0)) {
                if (sp30 == 0) {
                    if (sp2C == ((struct gd_2C_s32 *)arg0)->v) {
                        if (((struct gd_30_s32 *)arg0)->v != ((struct gd_28_s32 *)arg0)->v) {
                            sp2C = ((struct gd_28_s32 *)arg0)->v;
                            sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                        }
                    }
                    if (sp30 == 0) {
                        ((struct gd_34_s32 *)arg0)->v = sp2C;
                        arg2 = func_80013CBC(arg0, arg1, arg2);
                        sp2C = ((struct gd_34_s32 *)arg0)->v;
                        sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                        if (sp2C == ((struct gd_2C_s32 *)arg0)->v) {
                            if (((struct gd_30_s32 *)arg0)->v != ((struct gd_28_s32 *)arg0)->v) {
                                sp2C = ((struct gd_28_s32 *)arg0)->v;
                                sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                            }
                        }
                        if (sp30 == 0) {
                            ((struct gd_20_s32 *)arg0)->v = sp1C;
                            ((struct gd_1C_s32 *)arg0)->v = sp20;
                            ((struct gd_4_s32 *)arg1)->v = sp28;
                            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                            (*(s32 *)arg1) = sp24;
                            ((struct gd_34_s32 *)arg0)->v = sp2C;
                            return func_80013CBC(arg0, arg1, arg2);
                        }
                    }
                }
                arg2 = 0;
                (*(s8 *)(sp2C++)) = *(u8 *)(sp34++);
                sp30--;
                if (sp34 == ((struct gd_2C_s32 *)arg0)->v) {
                    sp34 = ((struct gd_28_s32 *)arg0)->v;
                }
                ((struct gd_4_s32 *)sp38)->v--;
            }
            (*(s32 *)sp38) = 0;
            break;
        case 6:
            if (sp30 == 0) {
                if (sp2C == ((struct gd_2C_s32 *)arg0)->v) {
                    if (((struct gd_30_s32 *)arg0)->v != ((struct gd_28_s32 *)arg0)->v) {
                        sp2C = ((struct gd_28_s32 *)arg0)->v;
                        sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                    }
                }
                if (sp30 == 0) {
                    ((struct gd_34_s32 *)arg0)->v = sp2C;
                    arg2 = func_80013CBC(arg0, arg1, arg2);
                    sp2C = ((struct gd_34_s32 *)arg0)->v;
                    sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                    if (sp2C == ((struct gd_2C_s32 *)arg0)->v) {
                        if (((struct gd_30_s32 *)arg0)->v != ((struct gd_28_s32 *)arg0)->v) {
                            sp2C = ((struct gd_28_s32 *)arg0)->v;
                            sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
                        }
                    }
                    if (sp30 == 0) {
                        ((struct gd_20_s32 *)arg0)->v = sp1C;
                        ((struct gd_1C_s32 *)arg0)->v = sp20;
                        ((struct gd_4_s32 *)arg1)->v = sp28;
                        ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                        (*(s32 *)arg1) = sp24;
                        ((struct gd_34_s32 *)arg0)->v = sp2C;
                        return func_80013CBC(arg0, arg1, arg2);
                    }
                }
            }
            arg2 = 0;
            (*(s8 *)(sp2C++)) = ((struct gd_B_u8 *)sp38)->v;
            sp30--;
            (*(s32 *)sp38) = 0;
            break;
        case 7:
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            arg2 = func_80013CBC(arg0, arg1, arg2);
            sp2C = ((struct gd_34_s32 *)arg0)->v;
            sp30 = ((u32)sp2C < (u32)((struct gd_30_s32 *)arg0)->v) ? ((((struct gd_30_s32 *)arg0)->v - sp2C) - 1) : (((struct gd_2C_s32 *)arg0)->v - sp2C);
            if (((struct gd_30_s32 *)arg0)->v != ((struct gd_34_s32 *)arg0)->v) {
                ((struct gd_20_s32 *)arg0)->v = sp1C;
                ((struct gd_1C_s32 *)arg0)->v = sp20;
                ((struct gd_4_s32 *)arg1)->v = sp28;
                ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
                (*(s32 *)arg1) = sp24;
                ((struct gd_34_s32 *)arg0)->v = sp2C;
                return func_80013CBC(arg0, arg1, arg2);
            }
            (*(s32 *)sp38) = 8;
        case 8:
            arg2 = 1;
            ((struct gd_20_s32 *)arg0)->v = sp1C;
            ((struct gd_1C_s32 *)arg0)->v = sp20;
            ((struct gd_4_s32 *)arg1)->v = sp28;
            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
            (*(s32 *)arg1) = sp24;
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            return func_80013CBC(arg0, arg1, arg2);
        case 9:
            arg2 = -3;
            ((struct gd_20_s32 *)arg0)->v = sp1C;
            ((struct gd_1C_s32 *)arg0)->v = sp20;
            ((struct gd_4_s32 *)arg1)->v = sp28;
            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
            (*(s32 *)arg1) = sp24;
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            return func_80013CBC(arg0, arg1, arg2);
        default:
            arg2 = -2;
            ((struct gd_20_s32 *)arg0)->v = sp1C;
            ((struct gd_1C_s32 *)arg0)->v = sp20;
            ((struct gd_4_s32 *)arg1)->v = sp28;
            ((struct gd_8_s32 *)arg1)->v = ((struct gd_8_s32 *)arg1)->v + (sp24 - (*(s32 *)arg1));
            (*(s32 *)arg1) = sp24;
            ((struct gd_34_s32 *)arg0)->v = sp2C;
            return func_80013CBC(arg0, arg1, arg2);
        }
    }
}
