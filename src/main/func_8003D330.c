struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1FC_s32 { u8 pad[0x1FC]; s32 v; };
struct gd_204_s32 { u8 pad[0x204]; s32 v; };
s32 func_800375A8();

void f(s32 arg0) {
    if (((struct gd_204_s32 *)arg0)->v != 0) {
        func_800375A8(arg0, 1);
        return;
    }
    if ((((struct gd_1FC_s32 *)arg0)->v & 0x80000000) == 0) {
        func_800375A8(arg0, 2);
        return;
    }
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        func_800375A8(arg0, 0);
    }
}
