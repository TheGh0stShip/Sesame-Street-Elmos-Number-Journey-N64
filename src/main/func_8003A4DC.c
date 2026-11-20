struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1F4_f32 { u8 pad[0x1F4]; f32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
extern s32 D_800A7F2C;
extern s32 D_8012F138;
s32 func_80037450();
s32 func_8004E5A0();
s32 func_8004E884();
s32 func_8004E990();

void f(s32 arg0) {
    func_80037450(arg0, 17);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 1;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
    ((struct gd_1F4_f32 *)arg0)->v = -2.0f;
    func_8004E884(D_800A7F2C, 0);
    D_800A7F2C = func_8004E5A0(27);
    D_8012F138 = 68;
    func_8004E990(D_800A7F2C, D_8012F138);
}
