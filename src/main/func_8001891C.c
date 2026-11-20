struct gd_74_s32 { u8 pad[0x74]; s32 v; };

void f(s32 arg0) {
    ((struct gd_74_s32 *)arg0)->v = ((struct gd_74_s32 *)arg0)->v | 1;
}
