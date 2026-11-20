struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1D4_s32 { u8 pad[0x1D4]; s32 v; };
struct gd_224_s16 { u8 pad[0x224]; s16 v; };

void f(s32 arg0, s32 arg1) {
    ((struct gd_1D4_s32 *)arg0)->v = arg1;
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -9;
    ((struct gd_224_s16 *)arg0)->v = 0;
}
