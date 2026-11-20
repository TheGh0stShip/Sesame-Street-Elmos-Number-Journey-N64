struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_28_s32 { u8 pad[0x28]; s32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_54_s8 { u8 pad[0x54]; s8 v; };
struct gd_54_u8 { u8 pad[0x54]; u8 v; };
extern s32 D_80033498;
extern f32 D_8006F75C;
extern u8 D_800769F0;
extern f64 D_8007AE10;
extern f64 D_8007AE18;
extern f64 D_8007AE20;
extern f64 D_8007AE28;
extern u8 D_800AD03A;
extern s32 D_80109DE0;
extern s32 D_8013EF28;
s32 func_80001424();
s32 func_800567F0(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32);
s32 func_80056F00();
s32 func_80057180();
s32 func_80057410(s32, s32, s32, s32, f32, f32, f32);

void f(s32 arg0) {
    register s32 reg_s0;
    s32 sp28[16];
    s32 sp68[16];
    s32 spA8[22];

    reg_s0 = arg0;
    if (D_800AD03A != 0) {
        func_80057410(((s32)&sp28), ((s32)&D_80109DE0), 0x42200000, 0x3FAAAAAB, 5e+01f, 1e+04f, 1.0f);
        func_800567F0(((s32)&sp68), ((struct gd_24_s32 *)reg_s0)->v, ((struct gd_28_s32 *)reg_s0)->v, ((struct gd_2C_s32 *)reg_s0)->v, ((struct gd_30_f32 *)reg_s0)->v, ((struct gd_34_f32 *)reg_s0)->v, ((struct gd_38_f32 *)reg_s0)->v, ((struct gd_3C_f32 *)reg_s0)->v, ((struct gd_40_f32 *)reg_s0)->v, ((struct gd_44_f32 *)reg_s0)->v);
        func_80057180(((s32)&sp68), ((s32)&sp28), ((s32)&spA8));
        func_80056F00(((s32)&spA8), D_8013EF28);
        func_80001424(((s32)&D_80033498), reg_s0, 1, 0, 0);
        return;
    }
    if (((struct gd_54_u8 *)reg_s0)->v != 0) {
        ((struct gd_54_s8 *)reg_s0)->v--;
        ((struct gd_30_f32 *)reg_s0)->v = ((struct gd_30_f32 *)reg_s0)->v + ((struct gd_48_f32 *)reg_s0)->v;
        ((struct gd_34_f32 *)reg_s0)->v = ((struct gd_34_f32 *)reg_s0)->v + ((struct gd_4C_f32 *)reg_s0)->v;
        ((struct gd_38_f32 *)reg_s0)->v = ((struct gd_38_f32 *)reg_s0)->v + ((struct gd_50_f32 *)reg_s0)->v;
        ((struct gd_24_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_24_f32 *)reg_s0)->v) + D_8007AE10);
        ((struct gd_28_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_28_f32 *)reg_s0)->v) + D_8007AE18);
        ((struct gd_2C_f32 *)reg_s0)->v = (f32)(((f64)((struct gd_2C_f32 *)reg_s0)->v) + D_8007AE20);
        D_8006F75C = (f32)(((f64)D_8006F75C) + D_8007AE28);
    }
    if (((struct gd_54_u8 *)reg_s0)->v == 0) {
        if (D_800769F0 == 4) {
            D_800769F0 = 5;
        }
    }
}
