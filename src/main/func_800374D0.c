struct gd_1AC_s32 { u8 pad[0x1AC]; s32 v; };
struct gd_1B0_s32 { u8 pad[0x1B0]; s32 v; };
struct gd_1B4_s32 { u8 pad[0x1B4]; s32 v; };

void f(s32 arg0) {
    ((struct gd_1AC_s32 *)arg0)->v = 0;
    ((struct gd_1B0_s32 *)arg0)->v = 0;
    ((struct gd_1B4_s32 *)arg0)->v = 0;
}
