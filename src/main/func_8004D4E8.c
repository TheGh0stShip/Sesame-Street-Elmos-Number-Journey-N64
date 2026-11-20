struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_60_f32 { u8 pad[0x60]; f32 v; };
struct gd_64_f32 { u8 pad[0x64]; f32 v; };
struct gd_8C_f32 { u8 pad[0x8C]; f32 v; };
struct gd_90_f32 { u8 pad[0x90]; f32 v; };
struct gd_94_f32 { u8 pad[0x94]; f32 v; };
extern f64 D_8007B4E0;
extern s32 D_800A7A54;
extern s32 D_800A913C;
extern s32 D_80107CD0;
extern s32 D_801153F4;
extern s32 D_8012C400;
extern f32 D_8013ED40;
s32 func_800208D0();
f32 func_80056690(f32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
s32 func_80060E7C();
f64 func_800694D0();
f64 func_80069920();
f64 func_8006A1F0();

void f(s32 arg0, s32 arg1, s32 arg2) {
    register s32 reg_s0;
    register s32 reg_s1;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    s32 pad0;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    s32 pad1;
    s32 pad2[8];
    s32 sp58;
    s32 sp5C;
    f32 sp60;
    f32 sp64;
    f32 sp68;
    s32 pad3;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f64 sp80;

    if (arg0 == 0) {
        return;
    }
    reg_s0 = arg0;
    reg_s1 = *(s32 *)reg_s0;
    func_80060E7C(arg2, arg1, 40);
    switch (D_8012C400) {
    case 0:
        sp2C = -func_80057E70(((struct gd_10_f32 *)arg2)->v);
        sp28 = func_800566A0(((struct gd_C_f32 *)arg2)->v) * func_800566A0(((struct gd_10_f32 *)arg2)->v);
        sp30 = func_80057E70(((struct gd_C_f32 *)arg2)->v) * func_800566A0(((struct gd_10_f32 *)arg2)->v);
        sp18 = (*(f32 *)arg2) - (sp28 * ((struct gd_18_f32 *)arg2)->v);
        sp1C = ((struct gd_4_f32 *)arg2)->v - (sp2C * ((struct gd_18_f32 *)arg2)->v);
        sp20 = ((struct gd_8_f32 *)arg2)->v - (sp30 * ((struct gd_18_f32 *)arg2)->v);
        if (((struct gd_4_s32 *)reg_s0)->v == 0) {
            ((struct gd_8_f32 *)reg_s0)->v = sp18;
            ((struct gd_C_f32 *)reg_s0)->v = sp1C;
            ((struct gd_10_f32 *)reg_s0)->v = sp20;
            ((struct gd_8C_f32 *)reg_s0)->v = sp18;
            ((struct gd_90_f32 *)reg_s0)->v = -sp20;
            ((struct gd_94_f32 *)reg_s0)->v = sp1C;
            ((struct gd_4_s32 *)reg_s0)->v = 1;
            return;
        }
        sp5C = 0x80000000;
        sp58 = func_800208D0((reg_s0 + 20), (reg_s0 + 112), (reg_s0 + 8), ((s32)&sp18), ((s32)&sp28), ((s32)&sp5C));
        if ((sp5C & 0x80000000) != 0) {
            D_8012C400 = 1;
            sp74 = ((struct gd_60_f32 *)reg_s0)->v;
            sp78 = ((struct gd_64_f32 *)reg_s0)->v;
            sp70 = (f32)func_8006A1F0(((f64)((sp74 * sp74) + (sp78 * sp78))));
            if (!(sp70 == 0.0f)) {
                sp74 = sp74 / sp70;
                sp78 = sp78 / sp70;
            }
            ((struct gd_8_f32 *)reg_s0)->v = sp18 + (D_8013ED40 * sp74);
            ((struct gd_10_f32 *)reg_s0)->v = sp20 - (D_8013ED40 * sp78);
            ((struct gd_C_f32 *)reg_s0)->v = sp1C;
            D_8013ED40 = D_8013ED40 + ((f32)D_801153F4);
            if (((f32)D_800A913C) < D_8013ED40) {
                D_8013ED40 = (f32)D_800A913C;
            }
            sp60 = (*(f32 *)arg2) - sp18;
            sp64 = ((struct gd_4_f32 *)arg2)->v - sp1C;
            sp68 = ((struct gd_8_f32 *)arg2)->v - sp20;
            sp70 = func_80056690((((sp60 * sp60) + (sp64 * sp64)) + (sp68 * sp68)));
            if (sp70 >= 1e-07f) {
                sp80 = D_8007B4E0 / ((f64)sp70);
                sp60 = (f32)(((f64)sp60) * sp80);
                sp64 = (f32)(((f64)sp64) * sp80);
                sp68 = (f32)(((f64)sp68) * sp80);
            } else {
            }
            ((struct gd_C_f32 *)arg2)->v = (f32)func_80069920(((f64)sp68), ((f64)sp60));
            ((struct gd_10_f32 *)arg2)->v = (f32)func_800694D0(((f64)(-sp64)));
            ((struct gd_24_s32 *)arg2)->v = ((struct gd_24_s32 *)arg2)->v | 2;
        } else {
            D_8013ED40 = D_8013ED40 - ((f32)D_800A7A54);
            if (((f32)D_80107CD0) > D_8013ED40) {
                D_8013ED40 = (f32)D_80107CD0;
            }
            ((struct gd_8_f32 *)reg_s0)->v = sp18;
            ((struct gd_10_f32 *)reg_s0)->v = sp20;
            ((struct gd_C_f32 *)reg_s0)->v = sp1C;
        }
        ((struct gd_8C_f32 *)reg_s0)->v = sp18;
        ((struct gd_90_f32 *)reg_s0)->v = -sp20;
        ((struct gd_94_f32 *)reg_s0)->v = sp1C;
    }
}
