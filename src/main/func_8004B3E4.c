struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_48_s32 { u8 pad[0x48]; s32 v; };
s32 func_80042300();

void f(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;

    if (arg0 == 0) {
        return;
    }
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        reg_s1 = ((struct gd_18_s32 *)reg_s0)->v;
        if (((struct gd_48_s32 *)reg_s0)->v != 0) {
            ((s32 (*)())((struct gd_48_s32 *)reg_s0)->v)(((struct gd_14_s32 *)reg_s0)->v);
        }
        func_80042300(reg_s0);
        reg_s0 = reg_s1;
    }
    ((struct gd_8_s32 *)arg0)->v = 0;
}
