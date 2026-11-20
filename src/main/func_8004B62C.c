struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_28_s32 { u8 pad[0x28]; s32 v; };
s32 func_80042300();

void f(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;

    reg_s0 = ((struct gd_C_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        reg_s1 = ((struct gd_28_s32 *)reg_s0)->v;
        func_80042300(reg_s0);
        reg_s0 = reg_s1;
    }
    ((struct gd_C_s32 *)arg0)->v = 0;
}
