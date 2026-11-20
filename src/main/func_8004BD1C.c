struct gd_48_f32 { u8 pad[0x48]; f32 v; };

void f(s32 arg0, f32 arg1) {
    ((struct gd_48_f32 *)arg0)->v = arg1;
}
