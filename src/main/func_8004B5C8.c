struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_28_s32 { u8 pad[0x28]; s32 v; };

s32 f(s32 arg0, s32 arg1) {
    register s32 reg_v1;

    reg_v1 = ((struct gd_C_s32 *)arg0)->v;
    while (!(reg_v1 == 0)) {
        if ((*(s32 *)reg_v1) == arg1) {
            return reg_v1;
        }
        reg_v1 = ((struct gd_28_s32 *)reg_v1)->v;
    }
    return 0;
}
