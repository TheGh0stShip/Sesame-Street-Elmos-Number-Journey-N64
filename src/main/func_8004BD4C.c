struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
extern s32 D_8012C400;
f32 func_8004B828(f32, f32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
s32 func_80060E7C();
f64 func_80069920();
f64 func_8006A1F0();

void f(s32 arg0, s32 arg1, s32 arg2) {
    register s32 reg_s0;
    register s32 reg_s1;
    register s32 reg_s2;
    register s32 reg_s3;
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 pad1;
    f32 sp30;
    s32 pad2;
    f32 sp38;
    s32 pad3;
    f32 sp40;
    f32 sp44;
    f32 sp48;
    s32 pad4;
    s8 pad5;

    reg_s1 = arg0;
    reg_s0 = *(s32 *)reg_s1;
    reg_s3 = reg_s0 + 56;
    reg_s2 = reg_s1 + 8;
    ((struct gd_1C_f32 *)reg_s2)->v = 1.0f;
    ((struct gd_20_f32 *)reg_s2)->v = ((struct gd_20_f32 *)arg1)->v;
    ((struct gd_4_f32 *)reg_s2)->v = ((struct gd_4_f32 *)reg_s2)->v + ((struct gd_40_f32 *)reg_s1)->v;
    sp14 = -func_80057E70(((struct gd_10_f32 *)arg1)->v);
    sp10 = func_800566A0(((struct gd_C_f32 *)arg1)->v) * func_800566A0(((struct gd_10_f32 *)arg1)->v);
    sp18 = func_80057E70(((struct gd_C_f32 *)arg1)->v) * func_800566A0(((struct gd_10_f32 *)arg1)->v);
    sp40 = (*(f32 *)reg_s3) - (sp10 * ((struct gd_18_f32 *)reg_s3)->v);
    sp44 = ((struct gd_4_f32 *)reg_s3)->v - (sp14 * ((struct gd_18_f32 *)reg_s3)->v);
    sp48 = ((struct gd_8_f32 *)reg_s3)->v - (sp18 * ((struct gd_18_f32 *)reg_s3)->v);
    sp20 = (*(f32 *)reg_s2) - sp40;
    sp24 = ((struct gd_4_f32 *)reg_s2)->v - sp44;
    sp28 = ((struct gd_8_f32 *)reg_s2)->v - sp48;
    ((struct gd_10_f32 *)reg_s2)->v = (f32)func_80069920(((f64)((struct gd_38_f32 *)reg_s1)->v), ((f64)((struct gd_18_f32 *)reg_s2)->v));
    ((struct gd_18_f32 *)reg_s2)->v = (f32)func_8006A1F0(((f64)(((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28))));
    sp30 = (*(f32 *)reg_s3) - (*(f32 *)reg_s2);
    sp38 = ((struct gd_8_f32 *)reg_s3)->v - ((struct gd_8_f32 *)reg_s2)->v;
    if (((sp30 * sp30) + (sp38 * sp38)) > ((struct gd_3C_f32 *)reg_s1)->v) {
        ((struct gd_C_f32 *)reg_s2)->v = (f32)func_80069920(((f64)sp28), ((f64)sp20));
        ((struct gd_24_s32 *)reg_s2)->v = ((struct gd_24_s32 *)arg1)->v & 4;
        ((struct gd_24_s32 *)reg_s2)->v = ((struct gd_24_s32 *)reg_s2)->v | 1;
        D_8012C400 = 0;
    } else {
        ((struct gd_24_s32 *)reg_s2)->v = ((struct gd_24_s32 *)arg1)->v & 6;
        if ((((struct gd_24_s32 *)arg1)->v & 2) == 0) {
            if (!(((struct gd_44_f32 *)reg_s1)->v == 0.0f)) {
                if (D_8012C400 == 0) {
                    ((struct gd_C_f32 *)reg_s2)->v = (func_8004B828(((struct gd_30_f32 *)reg_s1)->v, (((struct gd_C_f32 *)reg_s2)->v + ((struct gd_48_f32 *)reg_s1)->v)) / ((struct gd_44_f32 *)reg_s1)->v) + ((struct gd_C_f32 *)reg_s2)->v;
                }
            }
        }
    }
    if (((struct gd_18_f32 *)reg_s2)->v > ((struct gd_34_f32 *)reg_s1)->v) {
        ((struct gd_18_f32 *)reg_s2)->v = ((struct gd_34_f32 *)reg_s1)->v;
    }
    func_80060E7C(arg2, reg_s2, 40);
}
