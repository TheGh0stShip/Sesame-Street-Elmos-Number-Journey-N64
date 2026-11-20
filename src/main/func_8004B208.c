struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
struct gd_48_s32 { u8 pad[0x48]; s32 v; };
s32 func_80042080();
s32 func_80069C70();

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s32 reg_s2;

    if (arg0 == 0) {
        return 0;
    }
    reg_s1 = func_80042080(76);
    if (reg_s1 == 0) {
        return 0;
    }
    func_80069C70(reg_s1, 0, 76);
    (*(s32 *)reg_s1) = ((struct gd_4_s32 *)arg0)->v++;
    ((struct gd_4_s32 *)reg_s1)->v = arg4;
    ((struct gd_8_f32 *)reg_s1)->v = arg5;
    ((struct gd_C_f32 *)reg_s1)->v = 1.0f;
    ((struct gd_14_s32 *)reg_s1)->v = arg1;
    ((struct gd_44_s32 *)reg_s1)->v = arg2;
    ((struct gd_48_s32 *)reg_s1)->v = arg3;
    reg_s2 = 0;
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        if (((struct gd_4_s32 *)reg_s0)->v > arg4) {
            ((struct gd_18_s32 *)reg_s1)->v = reg_s0;
            if (reg_s2 == 0) {
                ((struct gd_8_s32 *)arg0)->v = reg_s1;
            } else {
                ((struct gd_18_s32 *)reg_s2)->v = reg_s1;
            }
            return;
        }
        reg_s2 = reg_s0;
        reg_s0 = ((struct gd_18_s32 *)reg_s0)->v;
    }
    if (reg_s2 == 0) {
        ((struct gd_8_s32 *)arg0)->v = reg_s1;
        return;
    }
    ((struct gd_18_s32 *)reg_s2)->v = reg_s1;
}
