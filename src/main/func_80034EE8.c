
struct n64dk_mF8_40 { u8 pad[0xF8]; struct { u8 b[0x40]; } m[1]; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_58_f32 { u8 pad[0x58]; f32 v; };
struct gd_58_f64 { u8 pad[0x58]; f64 v; };
struct gd_5C_f32 { u8 pad[0x5C]; f32 v; };
struct gd_60_f32 { u8 pad[0x60]; f32 v; };
struct gd_60_s32 { u8 pad[0x60]; s32 v; };
struct gd_1C4_f32 { u8 pad[0x1C4]; f32 v; };
struct gd_1C8_f32 { u8 pad[0x1C8]; f32 v; };
struct gd_1CC_f32 { u8 pad[0x1CC]; f32 v; };
struct gd_1D0_f32 { u8 pad[0x1D0]; f32 v; };
struct gd_1E4_s32 { u8 pad[0x1E4]; s32 v; };
struct gd_1EC_s32 { u8 pad[0x1EC]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
extern s32 D_8003670C;
extern s32 D_800717E0;
extern f64 D_8007AE58;
extern f64 D_8007AE60;
extern f64 D_8007AE68;
extern f64 D_8007AE70;
extern f64 D_8007AE78;
extern u8 D_800AD03A;
extern s32 D_80109D4C;
s32 func_80001424();
struct V3 { f32 x, y, z; };
s32 func_8001FAA8();
f32 func_800566A0(f32);
f32 func_80057E70(f32);

void func_80034EE8(s32 arg0) {
    s32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;

    sp18 = arg0;
    if (D_800AD03A != 0) {
        func_8001FAA8(((s32)&((struct n64dk_mF8_40 *)sp18)->m[D_800717E0]), (sp18 + 36), ((struct { u8 pad[0x58]; struct V3 v; } *)sp18)->v);
        func_80001424(((s32)&D_8003670C), sp18, ((struct gd_1F0_s32 *)sp18)->v, 1, 0);
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        }
        return;
    }
    if ((++((struct gd_1EC_s32 *)sp18)->v) >= 81) {
        ((struct gd_C_s32 *)sp18)->v = ((struct gd_C_s32 *)sp18)->v | 1;
        return;
    }
    if (((f64)((struct gd_1CC_f32 *)sp18)->v) < D_8007AE58) {
        ((struct gd_1CC_f32 *)sp18)->v = (f32)(((f64)((struct gd_1CC_f32 *)sp18)->v) + D_8007AE60);
    }
    ((struct gd_58_f32 *)sp18)->v = ((struct gd_1CC_f32 *)sp18)->v;
    ((struct gd_5C_f32 *)sp18)->v = ((struct gd_1CC_f32 *)sp18)->v;
    ((struct gd_60_f32 *)sp18)->v = ((struct gd_1CC_f32 *)sp18)->v;
    ((struct gd_2C_f32 *)sp18)->v = ((struct gd_2C_f32 *)sp18)->v + ((struct gd_1D0_f32 *)sp18)->v;
    ((struct gd_28_f32 *)sp18)->v = ((struct gd_28_f32 *)sp18)->v + ((struct gd_1C8_f32 *)sp18)->v;
    ((struct gd_2C_f32 *)sp18)->v = ((struct gd_2C_f32 *)sp18)->v + ((struct gd_1C4_f32 *)sp18)->v;
    if (((f64)((struct gd_1C8_f32 *)sp18)->v) > D_8007AE68) {
        ((struct gd_1C8_f32 *)sp18)->v = (f32)(((f64)((struct gd_1C8_f32 *)sp18)->v) - D_8007AE70);
    }
    ((struct gd_1E4_s32 *)sp18)->v = ((struct gd_1E4_s32 *)sp18)->v + 8;
    if (((struct gd_1E4_s32 *)sp18)->v >= 361) {
        ((struct gd_1E4_s32 *)sp18)->v = ((struct gd_1E4_s32 *)sp18)->v - 360;
    }
    sp24 = (f32)(((f64)((f32)((struct gd_1E4_s32 *)sp18)->v)) * D_8007AE78);
    sp1C = func_800566A0(sp24);
    sp20 = func_80057E70(sp24);
    ((struct gd_30_f32 *)sp18)->v = sp1C;
    ((struct gd_34_s32 *)sp18)->v = 0;
    ((struct gd_38_f32 *)sp18)->v = sp20;
    ((struct gd_48_f32 *)sp18)->v = (((struct gd_34_f32 *)sp18)->v * ((struct gd_44_f32 *)sp18)->v) - (((struct gd_38_f32 *)sp18)->v * ((struct gd_40_f32 *)sp18)->v);
    ((struct gd_4C_f32 *)sp18)->v = (((struct gd_38_f32 *)sp18)->v * ((struct gd_3C_f32 *)sp18)->v) - (((struct gd_30_f32 *)sp18)->v * ((struct gd_44_f32 *)sp18)->v);
    ((struct gd_50_f32 *)sp18)->v = (((struct gd_30_f32 *)sp18)->v * ((struct gd_40_f32 *)sp18)->v) - (((struct gd_34_f32 *)sp18)->v * ((struct gd_3C_f32 *)sp18)->v);
}
