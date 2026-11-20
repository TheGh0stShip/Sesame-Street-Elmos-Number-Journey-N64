#include "common.h"

s32 func_80013F48(struct IB *arg0, struct ZS *arg1, s32 arg2) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;
    s32 sp34;
    s32 sp38;
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
    s32 sp60;
    s32 sp64;
    s32 sp68;

    sp34 = *(s32 *)arg1;
    sp38 = arg1->f04;
    sp2C = arg0->f20;
    sp30 = arg0->f1C;
    sp3C = arg0->f34;
    sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
    while (1) {
        switch (arg0->f00) {
        case 0:
            {
            while (!((u32)sp30 >= 3)) {
                if (sp38 != 0) {
                    arg2 = 0;
                } else {
                    arg0->f20 = sp2C;
                    arg0->f1C = sp30;
                    arg1->f04 = sp38;
                    arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                    arg1->f00 = sp34;
                    arg0->f34 = sp3C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp38--;
                sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                sp30 = sp30 + 8;
            }
            }
            sp28 = sp2C & 7;
            arg0->f18 = sp28 & 1;
            switch ((u32)((u32)sp28 >> 1)) {
            case 0:
                sp2C = (u32)sp2C >> 3;
                sp30 = sp30 - 3;
                sp28 = sp30 & 7;
                sp2C = (u32)sp2C >> (u32)sp28;
                sp30 = sp30 - sp28;
                arg0->f00 = 1;
                break;
            case 1:
                ((s32 (*)())func_80016EE4)(((s32)&sp48), ((s32)&sp4C), ((s32)&sp50), ((s32)&sp54), arg1);
                arg0->f04 = ((s32 (*)())func_80015308)(sp48, sp4C, sp50, sp54, arg1);
                sp2C = (u32)sp2C >> 3;
                sp30 = sp30 - 3;
                arg0->f00 = 6;
                break;
            case 2:
                sp2C = (u32)sp2C >> 3;
                sp30 = sp30 - 3;
                arg0->f00 = 3;
                break;
            }
            break;
        case 1:
            {
            while (!((u32)sp30 >= 32)) {
                if (sp38 != 0) {
                    arg2 = 0;
                } else {
                    arg0->f20 = sp2C;
                    arg0->f1C = sp30;
                    arg1->f04 = sp38;
                    arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                    arg1->f00 = sp34;
                    arg0->f34 = sp3C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp38--;
                sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                sp30 = sp30 + 8;
            }
            }
            arg0->f04 = sp2C & 0xFFFF;
            sp30 = 0;
            sp2C = 0;
            arg0->f00 = (arg0->f04 != 0) ? 2 : ((arg0->f18 != 0) ? 7 : 0);
                break;
        case 2:
            if (sp38 == 0) {
                arg0->f20 = sp2C;
                arg0->f1C = sp30;
                arg1->f04 = sp38;
                arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                arg1->f00 = sp34;
                arg0->f34 = sp3C;
                return func_80013CBC(arg0, arg1, arg2);
            }
            if (sp40 == 0) {
                if (sp3C == arg0->f2C) {
                    if (arg0->f30 != arg0->f28) {
                        sp3C = arg0->f28;
                        sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
                    }
                }
                if (sp40 == 0) {
                    arg0->f34 = sp3C;
                    arg2 = func_80013CBC(arg0, arg1, arg2);
                    sp3C = arg0->f34;
                    sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
                    if (sp3C == arg0->f2C) {
                        if (arg0->f30 != arg0->f28) {
                            sp3C = arg0->f28;
                            sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
                        }
                    }
                    if (sp40 == 0) {
                        arg0->f20 = sp2C;
                        arg0->f1C = sp30;
                        arg1->f04 = sp38;
                        arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                        arg1->f00 = sp34;
                        arg0->f34 = sp3C;
                        return func_80013CBC(arg0, arg1, arg2);
                    }
                }
            }
            arg2 = 0;
            sp28 = arg0->f04;
            if ((u32)sp28 > (u32)sp38) {
                sp28 = sp38;
            }
            if ((u32)sp28 > (u32)sp40) {
                sp28 = sp40;
            }
            for (sp44 = 0; !((u32)sp44 >= (u32)sp28); sp44++) {
                (*(s8 *)(sp3C++)) = *(u8 *)(sp34++);
            }
            sp38 = sp38 - sp28;
            sp40 = sp40 - sp28;
            if ((arg0->f04 = arg0->f04 - sp28) != 0) {
                break;
            }
            arg0->f00 = (arg0->f18 != 0) ? 7 : 0;
                break;
        case 3:
            {
            while (!((u32)sp30 >= 14)) {
                if (sp38 != 0) {
                    arg2 = 0;
                } else {
                    arg0->f20 = sp2C;
                    arg0->f1C = sp30;
                    arg1->f04 = sp38;
                    arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                    arg1->f00 = sp34;
                    arg0->f34 = sp3C;
                    return func_80013CBC(arg0, arg1, arg2);
                }
                sp38--;
                sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                sp30 = sp30 + 8;
            }
            }
            arg0->f04 = sp28 = sp2C & 0x3FFF;
            arg0->f0C = &D_8013C200;
            sp2C = (u32)sp2C >> 14;
            sp30 = sp30 - 14;
            arg0->f08 = 0;
            arg0->f00 = 4;
        case 4:
            while ((u32)arg0->f08 < (u32)(((u32)arg0->f04 >> 10) + 4)) {
                while (!((u32)sp30 >= 3)) {
                    if (sp38 != 0) {
                        arg2 = 0;
                    } else {
                        arg0->f20 = sp2C;
                        arg0->f1C = sp30;
                        arg1->f04 = sp38;
                        arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                        arg1->f00 = sp34;
                        arg0->f34 = sp3C;
                        return func_80013CBC(arg0, arg1, arg2);
                    }
                    sp38--;
                    sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                    sp30 = sp30 + 8;
                }
                arg0->f0C[D_8007A110[arg0->f08++]] = sp2C & 7;
                sp2C = (u32)sp2C >> 3;
                sp30 = sp30 - 3;
            }
            while ((u32)arg0->f08 < 19) {
                arg0->f0C[D_8007A110[arg0->f08++]] = 0;
            }
            arg0->f10 = 7;
            sp28 = ((s32 (*)())func_80016D34)(arg0->f0C, &arg0->f10, &arg0->f14, arg0->f24, arg1);
            arg0->f08 = 0;
            arg0->f00 = 5;
        case 5:
            for (;;) {
                sp28 = arg0->f04;
                if ((u32)arg0->f08 >= (u32)(((sp28 & 31) + 258) + (((u32)sp28 >> 5) & 31))) {
                    break;
                }
                sp28 = arg0->f10;
                while (!((u32)sp30 >= (u32)sp28)) {
                    if (sp38 != 0) {
                        arg2 = 0;
                    } else {
                        arg0->f20 = sp2C;
                        arg0->f1C = sp30;
                        arg1->f04 = sp38;
                        arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                        arg1->f00 = sp34;
                        arg0->f34 = sp3C;
                        return func_80013CBC(arg0, arg1, arg2);
                    }
                    sp38--;
                    sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                    sp30 = sp30 + 8;
                }
                sp58 = arg0->f14 + ((sp2C & (*(s32 *)((sp28 * 4) + ((s32)&D_80074F40)))) * 8);
                sp28 = ((struct gd_1_u8 *)sp58)->v;
                sp64 = ((struct gd_4_s32 *)sp58)->v;
                if ((u32)sp64 < 16) {
                    sp2C = (u32)sp2C >> (u32)sp28;
                    sp30 = sp30 - sp28;
                    arg0->f0C[arg0->f08++] = sp64;
                } else {
                    sp5C = (sp64 == 18) ? 7 : (sp64 - 14);
                    sp60 = (sp64 == 18) ? 11 : 3;
                    while (!((u32)sp30 >= (u32)(sp28 + sp5C))) {
                        if (sp38 != 0) {
                            arg2 = 0;
                        } else {
                            arg0->f20 = sp2C;
                            arg0->f1C = sp30;
                            arg1->f04 = sp38;
                            arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                            arg1->f00 = sp34;
                            arg0->f34 = sp3C;
                            return func_80013CBC(arg0, arg1, arg2);
                        }
                        sp38--;
                        sp2C = sp2C | ((*(u8 *)(sp34++)) << sp30);
                        sp30 = sp30 + 8;
                    }
                    sp2C = (u32)sp2C >> (u32)sp28;
                    sp30 = sp30 - sp28;
                    sp60 = sp60 + (sp2C & (*(s32 *)((sp5C * 4) + ((s32)&D_80074F40))));
                    sp2C = (u32)sp2C >> (u32)sp5C;
                    sp30 = sp30 - sp5C;
                    sp5C = arg0->f08;
                    sp28 = arg0->f04;
                    sp64 = (sp64 != 16) ? 0 : arg0->f0C[sp5C - 1];
                    for (;;) {
                        arg0->f0C[sp5C++] = sp64;
                        if ((--sp60) == 0) {
                            break;
                        }
                    }
                    arg0->f08 = sp5C;
                }
            }
            arg0->f14 = 0;
            sp64 = 9;
            sp60 = 6;
            sp28 = arg0->f04;
            sp28 = ((s32 (*)())func_80016DBC)(((sp28 & 31) + 257), ((((u32)sp28 >> 5) & 31) + 1), arg0->f0C, ((s32)&sp64), ((s32)&sp60), ((s32)&sp5C), ((s32)&sp58), arg0->f24, arg1);
            sp68 = ((s32 (*)())func_80015308)(sp64, sp60, sp5C, sp58, arg1);
            arg0->f04 = sp68;
            arg0->f00 = 6;
        case 6:
            arg0->f20 = sp2C;
            arg0->f1C = sp30;
            arg1->f04 = sp38;
            arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
            arg1->f00 = sp34;
            arg0->f34 = sp3C;
            arg2 = func_80015380(arg0, arg1, arg2);
            if (arg2 != 1) {
                return func_80013CBC(arg0, arg1, arg2);
            }
            arg2 = 0;
            sp34 = *(s32 *)arg1;
            sp38 = arg1->f04;
            sp2C = arg0->f20;
            sp30 = arg0->f1C;
            sp3C = arg0->f34;
            sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
            if (arg0->f18 == 0) {
                arg0->f00 = 0;
                break;
            }
            if ((u32)sp30 >= 8) {
                sp30 = sp30 - 8;
                sp38++;
                sp34--;
            }
            arg0->f00 = 7;
        case 7:
            arg0->f34 = sp3C;
            arg2 = func_80013CBC(arg0, arg1, arg2);
            sp3C = arg0->f34;
            sp40 = ((u32)sp3C < (u32)arg0->f30) ? ((arg0->f30 - sp3C) - 1) : (arg0->f2C - sp3C);
            if (arg0->f30 != arg0->f34) {
                arg0->f20 = sp2C;
                arg0->f1C = sp30;
                arg1->f04 = sp38;
                arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
                arg1->f00 = sp34;
                arg0->f34 = sp3C;
                return func_80013CBC(arg0, arg1, arg2);
            }
            arg0->f00 = 8;
        case 8:
            arg2 = 1;
            arg0->f20 = sp2C;
            arg0->f1C = sp30;
            arg1->f04 = sp38;
            arg1->f08 = arg1->f08 + (sp34 - arg1->f00);
            arg1->f00 = sp34;
            arg0->f34 = sp3C;
            return func_80013CBC(arg0, arg1, arg2);
        }
    }
}
