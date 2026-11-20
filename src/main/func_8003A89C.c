struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1F4_f32 { u8 pad[0x1F4]; f32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
struct gd_226_s16 { u8 pad[0x226]; s16 v; };
s32 func_80037450();

void f(s32 arg0) {
    func_80037450(arg0, 18);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v | 128;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 1;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
    ((struct gd_226_s16 *)arg0)->v = 16;
    ((struct gd_1F4_f32 *)arg0)->v = -1.8f;
}
