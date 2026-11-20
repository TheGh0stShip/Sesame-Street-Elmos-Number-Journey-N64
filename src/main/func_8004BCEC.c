struct gd_44_f32 { u8 pad[0x44]; f32 v; };

void f(s32 arg0, f32 arg1) {
    ((struct gd_44_f32 *)arg0)->v = arg1;
}
