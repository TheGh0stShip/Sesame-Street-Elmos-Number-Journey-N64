struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };

void f(s32 arg0, s32 arg1) {
    (*(f32 *)arg0) = -((struct gd_24_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 4)) = -((struct gd_28_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 8)) = -((struct gd_2C_f32 *)arg1)->v;
    (*(s32 *)(arg0 + 12)) = 0;
    (*(f32 *)(arg0 + 16)) = ((struct gd_18_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 20)) = ((struct gd_1C_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 24)) = ((struct gd_20_f32 *)arg1)->v;
    (*(s32 *)(arg0 + 28)) = 0;
    (*(f32 *)(arg0 + 32)) = ((struct gd_C_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 36)) = ((struct gd_10_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 40)) = ((struct gd_14_f32 *)arg1)->v;
    (*(s32 *)(arg0 + 44)) = 0;
    (*(f32 *)(arg0 + 48)) = *(f32 *)arg1;
    (*(f32 *)(arg0 + 52)) = ((struct gd_4_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 56)) = ((struct gd_8_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 60)) = 1.0f;
    return;
}
