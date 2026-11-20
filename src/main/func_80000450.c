
struct gd_2_s16 { u8 pad[0x2]; s16 v; };
struct gd_2_u16 { u8 pad[0x2]; u16 v; };
struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_4_u16 { u8 pad[0x4]; u16 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
extern s32 D_80079CC0;
s32 func_80061178();

struct Pk { s32 z : 10; s32 y : 11; s32 x : 11; };

void func_80000450(s32 arg0, s32 arg1, register s32 arg2, s32 arg3, s32 arg4) {
    register s16 reg_s1;
    register s16 reg_s2;
    register s32 reg_s3;
    register s32 reg_s4;
    register s32 reg_s5;
    register s32 reg_s6;
    register s32 reg_s7;
    register s32 sp1C;
    register s32 sp24;
    register u8 sp2F;
    register s32 sp34;
    u16 sp10;
    s32 sp14;
    sp10 = arg2;
    sp2F = *(u8 *)(arg1 + 5);
    sp24 = ((struct gd_14_s32 *)arg0)->v;
    sp14 = arg1 + 8;
    sp34 = *(u8 *)((sp10 + sp14) + 1);
    sp34 = sp34 - (*(u8 *)(sp10 + sp14));
    if (sp34 == 0) {
        func_80061178(((s32)&D_80079CC0));
        sp34 = sp34 + 1;
    }
    reg_s3 = arg1;
    reg_s3 = reg_s3 + (((u32)((*(u16 *)(arg1 + 2)) + (sp10 * 4)) >> 2) * 4);
    reg_s4 = reg_s3 + 4;
    if (((*(u16 *)(arg1 + 6)) & 0x8000) != 0) {
        if (sp10 == (sp2F - 2)) {
            reg_s4 = (s32)((u32 *)arg1 + (u16)((*(u16 *)(arg1 + 2)) / 4));
        }
    }
    reg_s6 = arg3;
    reg_s5 = arg3;
    for (;;) {
        if ((*(u16 *)sp24) == 0) {
            return;
        }
        sp1C = *(u16 *)sp24;
        if (arg4 != 0) {
            while (sp1C != 0) {
                reg_s1 = ((struct Pk *)reg_s3)->x;
                reg_s2 = ((struct Pk *)reg_s4)->x;
                reg_s2 = reg_s2 - reg_s1;
                reg_s2 = ((s16)reg_s2) * arg4;
                reg_s2 = ((s16)reg_s2) / sp34;
                (*(s16 *)reg_s6) = reg_s1 + reg_s2;
                reg_s1 = ((struct Pk *)reg_s3)->y;
                reg_s2 = ((struct Pk *)reg_s4)->y;
                reg_s2 = reg_s2 - reg_s1;
                reg_s2 = ((s16)reg_s2) * arg4;
                reg_s2 = ((s16)reg_s2) / sp34;
                ((struct gd_2_s16 *)reg_s6)->v = reg_s1 + reg_s2;
                reg_s1 = ((struct Pk *)reg_s3)->z;
                reg_s2 = ((struct Pk *)reg_s4)->z;
                reg_s2 = reg_s2 - reg_s1;
                reg_s2 = ((s16)reg_s2) * arg4;
                reg_s2 = ((s16)reg_s2) / sp34;
                ((struct gd_4_s16 *)reg_s6)->v = reg_s1 + reg_s2;
                reg_s3 = reg_s3 + (sp2F * 4);
                reg_s4 = reg_s4 + (sp2F * 4);
                reg_s6 = reg_s6 + 16;
                sp1C = sp1C - 1;
            }
        } else {
            while (sp1C != 0) {
                reg_s1 = ((struct Pk *)reg_s3)->x;
                (*(s16 *)reg_s6) = reg_s1;
                reg_s1 = ((struct Pk *)reg_s3)->y;
                ((struct gd_2_s16 *)reg_s6)->v = reg_s1;
                reg_s1 = ((struct Pk *)reg_s3)->z;
                ((struct gd_4_s16 *)reg_s6)->v = reg_s1;
                reg_s3 = reg_s3 + (sp2F * 4);
                reg_s6 = reg_s6 + 16;
                sp1C = sp1C - 1;
            }
        }
        sp24 = sp24 + 2;
        sp1C = *(u16 *)sp24;
        sp24 = sp24 + 2;
        if (sp1C == 0) {
            return;
        }
        while (sp1C != 0) {
            reg_s7 = reg_s5 + ((*(u16 *)sp24) * 16);
            reg_s1 = *(u16 *)reg_s7;
            reg_s2 = ((struct gd_2_u16 *)reg_s7)->v;
            (*(s16 *)reg_s6) = reg_s1;
            reg_s1 = ((struct gd_4_u16 *)reg_s7)->v;
            ((struct gd_2_s16 *)reg_s6)->v = reg_s2;
            ((struct gd_4_s16 *)reg_s6)->v = reg_s1;
            sp24 = sp24 + 2;
            reg_s6 = reg_s6 + 16;
            sp1C = sp1C - 1;
        }
    }
}
