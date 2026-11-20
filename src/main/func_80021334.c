struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
extern f64 D_8007A620;
f32 func_80056690(f32);

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 pad1;
    f32 sp30;
    f32 sp34;
    f32 sp38;
    s32 pad2;
    f32 sp40;
    f64 sp48;

    sp10 = (*(f32 *)arg2) - (*(f32 *)arg1);
    sp14 = ((struct gd_4_f32 *)arg2)->v - ((struct gd_4_f32 *)arg1)->v;
    sp18 = ((struct gd_8_f32 *)arg2)->v - ((struct gd_8_f32 *)arg1)->v;
    sp20 = (*(f32 *)arg3) - (*(f32 *)arg2);
    sp24 = ((struct gd_4_f32 *)arg3)->v - ((struct gd_4_f32 *)arg2)->v;
    sp28 = ((struct gd_8_f32 *)arg3)->v - ((struct gd_8_f32 *)arg2)->v;
    sp30 = (sp24 * sp18) - (sp28 * sp14);
    sp34 = (sp28 * sp10) - (sp20 * sp18);
    sp38 = (sp20 * sp14) - (sp24 * sp10);
    sp40 = func_80056690((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
    if (sp40 >= 1e-07f) {
        sp48 = D_8007A620 / ((f64)sp40);
        sp30 = (f32)(((f64)sp30) * sp48);
        sp34 = (f32)(((f64)sp34) * sp48);
        sp38 = (f32)(((f64)sp38) * sp48);
    } else {
    }
    ((struct gd_10_f32 *)arg0)->v = sp30;
    ((struct gd_14_f32 *)arg0)->v = sp34;
    ((struct gd_18_f32 *)arg0)->v = sp38;
    ((struct gd_1C_f32 *)arg0)->v = ((sp30 * (*(f32 *)arg1)) + (sp34 * ((struct gd_4_f32 *)arg1)->v)) + (sp38 * ((struct gd_8_f32 *)arg1)->v);
    ((struct gd_20_f32 *)arg0)->v = *(f32 *)arg1;
    ((struct gd_24_f32 *)arg0)->v = ((struct gd_4_f32 *)arg1)->v;
    ((struct gd_28_f32 *)arg0)->v = ((struct gd_8_f32 *)arg1)->v;
    ((struct gd_2C_f32 *)arg0)->v = *(f32 *)arg2;
    ((struct gd_30_f32 *)arg0)->v = ((struct gd_4_f32 *)arg2)->v;
    ((struct gd_34_f32 *)arg0)->v = ((struct gd_8_f32 *)arg2)->v;
    ((struct gd_38_f32 *)arg0)->v = *(f32 *)arg3;
    ((struct gd_3C_f32 *)arg0)->v = ((struct gd_4_f32 *)arg3)->v;
    ((struct gd_40_f32 *)arg0)->v = ((struct gd_8_f32 *)arg3)->v;
    ((struct gd_44_f32 *)arg0)->v = *(f32 *)arg4;
    ((struct gd_48_f32 *)arg0)->v = ((struct gd_4_f32 *)arg4)->v;
    ((struct gd_4C_f32 *)arg0)->v = ((struct gd_8_f32 *)arg4)->v;
}
