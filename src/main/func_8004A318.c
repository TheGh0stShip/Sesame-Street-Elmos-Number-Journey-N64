struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_60_s32 { u8 pad[0x60]; s32 v; };
struct gd_64_s32 { u8 pad[0x64]; s32 v; };
struct gd_68_s32 { u8 pad[0x68]; s32 v; };
struct gd_84_f32 { u8 pad[0x84]; f32 v; };
struct gd_88_f32 { u8 pad[0x88]; f32 v; };
struct gd_8C_f32 { u8 pad[0x8C]; f32 v; };
struct gd_F0_f32 { u8 pad[0xF0]; f32 v; };
struct n64dk_e8 { u8 b[8]; };
extern f64 D_8007B410;
extern u16 D_800B1092;
extern s32 D_80108FB4;
extern struct n64dk_e8 *D_8013EC7C;
extern s32 D_8013ED00;
s32 func_800567F0(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32);
s32 func_80057180();
s32 func_80057410(s32, s32, s32, f32, f32, f32, f32);

void f(s32 arg0, s32 arg1) {
    s32 sp28[16];
    s32 sp68[16];
    u16 spA8;
    s32 spAC;

    func_80057410(((s32)&sp68), ((s32)&spA8), D_80108FB4, ((f32)(D_8007B410 / ((f64)D_800B1092))), 5e+01f, ((struct gd_F0_f32 *)arg0)->v, 1.0f);
    if (arg1 != 0) {
        spAC = (s32)D_8013EC7C++;
        (*(s32 *)spAC) = 0xBC00000E;
        ((struct gd_4_s32 *)spAC)->v = spA8;
    }
    func_800567F0(((s32)&sp28), ((struct gd_60_s32 *)arg0)->v, ((struct gd_64_s32 *)arg0)->v, ((struct gd_68_s32 *)arg0)->v, ((struct gd_10_f32 *)arg0)->v, ((struct gd_14_f32 *)arg0)->v, ((struct gd_18_f32 *)arg0)->v, ((struct gd_84_f32 *)arg0)->v, ((struct gd_88_f32 *)arg0)->v, ((struct gd_8C_f32 *)arg0)->v);
    func_80057180(((s32)&sp28), ((s32)&sp68), ((s32)&D_8013ED00));
}
