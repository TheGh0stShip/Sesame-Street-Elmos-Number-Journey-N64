struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };

void f(s32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    register s32 reg_v0;

    if (arg0 == 0) {
        return;
    }
    reg_v0 = arg0;
    ((struct gd_18_s32 *)reg_v0)->v = 0;
    ((struct gd_10_s32 *)reg_v0)->v = 0;
    ((struct gd_8_s32 *)reg_v0)->v = 0;
    if ((arg3 & 2) != 0) {
        ((struct gd_14_s32 *)reg_v0)->v = 0;
        ((struct gd_C_s32 *)reg_v0)->v = 0;
        ((struct gd_4_s32 *)reg_v0)->v = 0;
        return;
    }
    if ((arg3 & 1) == 0) {
        return;
    }
    if ((arg3 & 4) != 0) {
        ((struct gd_8_f32 *)reg_v0)->v = arg2;
    } else {
        ((struct gd_10_f32 *)reg_v0)->v = arg2;
    }
    ((struct gd_18_f32 *)reg_v0)->v = arg1;
}
