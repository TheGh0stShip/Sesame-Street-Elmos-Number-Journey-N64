struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };

void f(s32 arg0, s32 arg1, s32 arg2) {
    if (arg2 != 0) {
        (*(f32 *)(arg0 + 16)) = *(f32 *)arg2;
        ((struct gd_4_f32 *)(arg0 + 16))->v = ((struct gd_4_f32 *)arg2)->v;
        ((struct gd_8_f32 *)(arg0 + 16))->v = ((struct gd_8_f32 *)arg2)->v;
    }
    if (arg1 != 0) {
        (*(f32 *)(arg0 + 4)) = *(f32 *)arg1;
        ((struct gd_4_f32 *)(arg0 + 4))->v = ((struct gd_4_f32 *)arg1)->v;
        ((struct gd_8_f32 *)(arg0 + 4))->v = ((struct gd_8_f32 *)arg1)->v;
    }
}
