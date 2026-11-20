
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s16 { u8 pad[0x20]; s16 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
extern s32 D_80079CE0;
s32 func_80000450();
s32 func_80061178();

s32 func_8000092C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s16 reg_s2;
    register s32 reg_s3;
    register s32 reg_s4;
    register s32 reg_s5;
    s32 sp18;
    s32 sp1C;

    if (((*(u16 *)arg0) & 0x2000) != 0) {
        reg_s1 = *(u8 *)(arg2 + 4);
        if (arg3 >= (reg_s1 & 255)) {
            if (((*(u16 *)arg0) & 0xFFFF7FFF) != 0) {
                sp18 = ((struct gd_C_s32 *)arg0)->v;
            } else {
                sp18 = ((struct gd_10_s32 *)arg0)->v;
            }
        } else {
            if (((*(u16 *)arg0) & 0x8000) != 0) {
                sp18 = ((struct gd_C_s32 *)arg0)->v;
                sp1C = ((struct gd_10_s32 *)arg0)->v;
                (*(s16 *)arg0) = (*(u16 *)arg0) & 0x7FFF;
            } else {
                sp18 = ((struct gd_10_s32 *)arg0)->v;
                sp1C = ((struct gd_C_s32 *)arg0)->v;
                (*(s16 *)arg0) = (*(u16 *)arg0) | 0x8000;
            }
            reg_s0 = *(u8 *)(arg2 + 5);
            reg_s2 = 0;
            reg_s5 = arg2 + 8;
            reg_s4 = *(u8 *)reg_s5;
            for (;;) {
                reg_s3 = reg_s4 & 255;
                reg_s4 = *(u8 *)reg_s5;
                if ((reg_s4 & 255) == arg3) {
                    func_80000450(arg0, arg2, (u16)reg_s2, sp18, 0);
                    break;
                }
                if (arg3 < (reg_s4 & 255)) {
                    func_80000450(arg0, arg2, ((reg_s2 == 0) ? 0 : (u16)(reg_s2 - 1)), sp18, (arg3 - reg_s3));
                    break;
                }
                reg_s0 = reg_s0 - 1;
                if (reg_s0 == 0) {
                    func_80061178(((s32)&D_80079CE0));
                    func_80000450(arg0, arg2, (u16)reg_s2, sp18, (arg3 - reg_s3));
                    break;
                }
                reg_s2 = reg_s2 + 1;
                reg_s5 = reg_s5 + 1;
            }
        }
    } else {
        sp18 = ((struct gd_C_s32 *)arg0)->v;
    }
    arg4 = arg4 + (((*(f32 *)((struct gd_24_s32 *)arg0)->v) + (*(f32 *)(((struct gd_24_s32 *)arg0)->v + 12))) / 2.0f);
    arg5 = arg5 + (((*(f32 *)(((struct gd_24_s32 *)arg0)->v + 4)) + (*(f32 *)(((struct gd_24_s32 *)arg0)->v + 16))) / 2.0f);
    arg6 = arg6 + (((*(f32 *)(((struct gd_24_s32 *)arg0)->v + 8)) + (*(f32 *)(((struct gd_24_s32 *)arg0)->v + 20))) / 2.0f);
    ((struct gd_C_s32 *)arg1)->v = sp18;
    ((struct gd_10_s32 *)arg1)->v = ((struct gd_8_s32 *)arg0)->v;
    ((struct gd_14_s32 *)arg1)->v = arg8;
    ((struct gd_18_s32 *)arg1)->v = ((struct gd_1C_s32 *)arg0)->v;
    ((struct gd_20_s16 *)arg1)->v = *(u16 *)arg0;
    return arg1;
}
