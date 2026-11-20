struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
s32 func_800374D0();
s32 func_8003BEA4();

void f(s32 arg0) {
    func_800374D0(arg0);
    func_8003BEA4(arg0);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -5;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
}
