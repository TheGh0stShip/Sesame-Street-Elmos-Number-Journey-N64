struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
s32 func_80042080();

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    register s32 reg_s0;
    register s32 reg_s1;

    reg_s0 = func_80042080(52);
    (*(s32 *)reg_s0) = arg1;
    ((struct gd_4_s32 *)reg_s0)->v = arg2;
    ((struct gd_14_f32 *)reg_s0)->v = arg5 * arg5;
    ((struct gd_18_f32 *)reg_s0)->v = (*(f32 *)arg4) - arg6;
    ((struct gd_1C_f32 *)reg_s0)->v = (*(f32 *)arg4) + arg6;
    ((struct gd_20_f32 *)reg_s0)->v = ((struct gd_8_f32 *)arg4)->v - arg7;
    ((struct gd_24_f32 *)reg_s0)->v = ((struct gd_8_f32 *)arg4)->v + arg7;
    ((struct gd_28_f32 *)reg_s0)->v = ((struct gd_4_f32 *)arg4)->v - arg8;
    ((struct gd_2C_f32 *)reg_s0)->v = ((struct gd_4_f32 *)arg4)->v - arg8;
    ((struct gd_8_f32 *)reg_s0)->v = *(f32 *)arg3;
    ((struct gd_C_f32 *)reg_s0)->v = ((struct gd_4_f32 *)arg3)->v;
    ((struct gd_10_f32 *)reg_s0)->v = ((struct gd_8_f32 *)arg3)->v;
    ((struct gd_30_s32 *)reg_s0)->v = 0;
    reg_s1 = ((struct gd_4_s32 *)arg0)->v;
    if (reg_s1 == 0) {
        ((struct gd_4_s32 *)arg0)->v = reg_s0;
    } else {
        while (!(((struct gd_30_s32 *)reg_s1)->v == 0)) {
            reg_s1 = ((struct gd_30_s32 *)reg_s1)->v;
        }
        ((struct gd_30_s32 *)reg_s1)->v = reg_s0;
    }
    return reg_s0;
}
