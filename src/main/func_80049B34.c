struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
struct gd_FC_f32 { u8 pad[0xFC]; f32 v; };
struct n64dk_e8 { u8 b[8]; };
extern s32 D_8004B698;
extern s32 D_80076800;
extern struct n64dk_e8 *D_8013EC7C;
extern s32 D_8013ED00;
extern s32 D_8013EF28;
s32 func_80001424();
s32 func_80049D60();
s32 func_8004A050();
s32 func_8004A318();
s32 func_8004A478();
s32 func_8004B6B8();
s32 func_80060E7C();

void func_80049B34(s32 arg0, s32 arg1) {
    register s32 reg_s0;
    s32 pad0[12];
    s32 sp48;

    if (arg0 == 0) {
        return;
    }
    func_80060E7C((arg0 + 56), (arg0 + 16), 40);
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        if (((struct gd_44_s32 *)reg_s0)->v != 0) {
            ((s32 (*)())((struct gd_44_s32 *)reg_s0)->v)(((struct gd_14_s32 *)reg_s0)->v, (arg0 + 16), (reg_s0 + 28));
        }
        func_80060E7C((arg0 + 16), (reg_s0 + 28), 40);
        reg_s0 = ((struct gd_18_s32 *)reg_s0)->v;
    }
    if ((((struct gd_34_s32 *)arg0)->v & 32) != 0) {
        ((struct gd_FC_f32 *)arg0)->v = ((struct gd_30_f32 *)arg0)->v;
    } else {
        ((struct gd_FC_f32 *)arg0)->v = ((struct gd_1C_f32 *)arg0)->v;
        ((struct gd_30_f32 *)arg0)->v = ((struct gd_1C_f32 *)arg0)->v;
    }
    if (((struct gd_20_f32 *)arg0)->v > 0.55f) {
        ((struct gd_20_f32 *)arg0)->v = 0.55f;
    }
    if (((struct gd_28_f32 *)arg0)->v < 4e+02f) {
        ((struct gd_28_f32 *)arg0)->v = 4e+02f;
    }
    func_8004A050(arg0);
    func_8004A318(arg0, arg1);
    func_8004A478(arg0);
    if (D_80076800 != 0) {
        func_80049D60(arg0);
    }
    func_80001424(((s32)&D_8004B698), 0, 6, 0, 0);
    func_8004B6B8(((s32)&D_8013ED00), D_8013EF28);
    sp48 = (s32)D_8013EC7C++;
    (*(s32 *)sp48) = 0x1030040;
    ((struct gd_4_s32 *)sp48)->v = D_8013EF28;
}
