struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_48_s32 { u8 pad[0x48]; s32 v; };

void f(s32 arg0, s32 arg1) {
    register s32 reg_s0;

    if (arg0 == 0) {
        return;
    }
    reg_s0 = ((struct gd_8_s32 *)arg0)->v;
    while (!(reg_s0 == 0)) {
        if ((*(s32 *)reg_s0) == arg1) {
            if (((struct gd_48_s32 *)reg_s0)->v != 0) {
                ((s32 (*)())((struct gd_48_s32 *)reg_s0)->v)(((struct gd_14_s32 *)reg_s0)->v);
            }
        }
        reg_s0 = ((struct gd_18_s32 *)reg_s0)->v;
    }
}
