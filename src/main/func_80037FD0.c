struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1D4_s32 { u8 pad[0x1D4]; s32 v; };
s32 func_80037450();
s32 func_800375A8();

void f(s32 arg0) {
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        if (((struct gd_1D4_s32 *)arg0)->v == 10) {
            func_80037450(arg0, 11);
            return;
        }
        func_800375A8(arg0, 0);
    }
}
