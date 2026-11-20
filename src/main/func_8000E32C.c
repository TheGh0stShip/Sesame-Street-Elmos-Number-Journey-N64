struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct n64dk_e8 { u8 b[8]; };
extern s32 D_80072D7C;
extern s32 D_80072D88;
extern u8 D_80072E90;
extern u8 D_80072E91;
extern u8 D_80072E92;
extern u8 D_80072E93;
extern s32 D_80072E94;
extern s32 D_80072E98;
extern s32 D_80072E9C;
extern s32 D_80072EA0;
extern f64 D_80072EA8;
extern f64 D_80072EB0;
extern struct n64dk_e8 *D_8013EC7C;
s32 func_8000DC80();
s32 func_8005CF90();
s32 func_8005CFD0(s32, f32, f32);
s32 func_8005D068();
s32 func_8005E4F8();

void f(s32 arg0) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;

    sp18 = (s32)&D_80072D88;
    ((struct gd_4_s16 *)sp18)->v = (D_80072E9C * 8) + 8;
    ((struct gd_6_s16 *)sp18)->v = (D_80072EA0 * 16) + 8;
    func_8000DC80(sp18, arg0, ((s32)&D_80072D7C), D_80072E9C, D_80072EA0);
    func_8005D068(sp18, D_80072E94, D_80072E98);
    func_8005CF90(sp18, D_80072E90, D_80072E91, D_80072E92, D_80072E93);
    func_8005CFD0(sp18, ((f32)D_80072EA8), ((f32)D_80072EB0));
    sp1C = func_8005E4F8(sp18);
    sp20 = (s32)D_8013EC7C++;
    (*(s32 *)sp20) = 0x6000000;
    ((struct gd_4_s32 *)sp20)->v = sp1C;
}
