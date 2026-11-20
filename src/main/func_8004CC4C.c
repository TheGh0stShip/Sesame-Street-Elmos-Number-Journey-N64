struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
extern f64 D_8007B4A8;
extern f64 D_8007B4B0;
extern f64 D_8007B4B8;
extern f64 D_8007B4C0;
extern f64 D_8007B4C8;
extern f64 D_8007B4D0;
extern f64 D_8007B4D8;
s32 func_80060E7C();

void f(s32 arg0, s32 arg1, s32 arg2) {
    register s32 reg_s0;

    reg_s0 = arg0;
    func_80060E7C(arg2, arg1, 40);
    ((struct gd_4_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_4_f32 *)reg_s0)->v) - (((f64)((struct gd_8_f32 *)reg_s0)->v) * D_8007B4A8));
    ((struct gd_C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_C_f32 *)reg_s0)->v) + (((f64)((struct gd_10_f32 *)reg_s0)->v) * D_8007B4B0));
    if (((f64)((struct gd_C_f32 *)reg_s0)->v) > D_8007B4B8) {
        ((struct gd_C_f32 *)reg_s0)->v = 1.2566371f;
    } else {
        if (((f64)((struct gd_C_f32 *)reg_s0)->v) < D_8007B4C0) {
            ((struct gd_C_f32 *)reg_s0)->v = -0.31415927f;
        }
    }
    ((struct gd_14_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_18_f32 *)reg_s0)->v) * D_8007B4C8);
    ((struct gd_C_f32 *)arg2)->v = ((struct gd_C_f32 *)arg2)->v + ((struct gd_14_f32 *)reg_s0)->v;
    ((struct gd_10_f32 *)arg2)->v = ((struct gd_10_f32 *)arg2)->v + ((struct gd_C_f32 *)reg_s0)->v;
    ((struct gd_18_f32 *)arg2)->v = ((struct gd_18_f32 *)arg2)->v + ((struct gd_4_f32 *)reg_s0)->v;
    if (((f64)((struct gd_18_f32 *)arg2)->v) > D_8007B4D0) {
        ((struct gd_18_f32 *)arg2)->v = 1.25e+04f;
        return;
    }
    if (((f64)((struct gd_18_f32 *)arg2)->v) < D_8007B4D8) {
        ((struct gd_18_f32 *)arg2)->v = 1e+01f;
    }
}
