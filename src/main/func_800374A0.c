struct gd_1D4_s32 { u8 pad[0x1D4]; s32 v; };

void f(s32 arg0, s32 arg1) {
    ((struct gd_1D4_s32 *)arg0)->v = arg1;
}
