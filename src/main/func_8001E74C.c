struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    (*(f32 *)arg0) = -(*(f32 *)arg4);
    (*(f32 *)(arg0 + 4)) = -((struct gd_4_f32 *)arg4)->v;
    (*(f32 *)(arg0 + 8)) = -((struct gd_8_f32 *)arg4)->v;
    (*(s32 *)(arg0 + 12)) = 0;
    (*(f32 *)(arg0 + 16)) = *(f32 *)arg3;
    (*(f32 *)(arg0 + 20)) = ((struct gd_4_f32 *)arg3)->v;
    (*(f32 *)(arg0 + 24)) = ((struct gd_8_f32 *)arg3)->v;
    (*(s32 *)(arg0 + 28)) = 0;
    (*(f32 *)(arg0 + 32)) = *(f32 *)arg2;
    (*(f32 *)(arg0 + 36)) = ((struct gd_4_f32 *)arg2)->v;
    (*(f32 *)(arg0 + 40)) = ((struct gd_8_f32 *)arg2)->v;
    (*(s32 *)(arg0 + 44)) = 0;
    (*(f32 *)(arg0 + 48)) = *(f32 *)arg1;
    (*(f32 *)(arg0 + 52)) = ((struct gd_4_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 56)) = ((struct gd_8_f32 *)arg1)->v;
    (*(f32 *)(arg0 + 60)) = 1.0f;
    return;
}
