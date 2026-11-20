struct gd_4_s32 { u8 pad[0x4]; s32 v; };

void f(s32 arg0) {
    (*(s32 *)arg0) = 0;
    ((struct gd_4_s32 *)arg0)->v = 0;
}
