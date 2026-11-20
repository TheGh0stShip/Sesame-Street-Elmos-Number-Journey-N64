struct gd_AC_f32 { u8 pad[0xAC]; f32 v; };
struct gd_B0_f32 { u8 pad[0xB0]; f32 v; };
struct gd_B4_f32 { u8 pad[0xB4]; f32 v; };
struct gd_1FC_s32 { u8 pad[0x1FC]; s32 v; };
extern f64 D_8007B038;
s32 func_800375A8();

void f(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    if ((((struct gd_1FC_s32 *)arg0)->v & 0x80000000) != 0) {
        sp10 = ((struct gd_AC_f32 *)arg0)->v;
        sp14 = ((struct gd_B4_f32 *)arg0)->v;
        sp18 = -((struct gd_B0_f32 *)arg0)->v;
        if (((f64)sp14) > D_8007B038) {
            func_800375A8(arg0, 21);
        }
    }
}
