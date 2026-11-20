struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
extern s32 D_8007B3BC;
extern s32 D_800A0520;
s32 func_80049920();

void f(s32 arg0) {
    s32 sp10;

    if (((struct gd_1C_s32 *)arg0)->v == 0) {
        func_80049920(((s32)&D_8007B3BC));
        return;
    }
    sp10 = ((struct gd_1C_s32 *)arg0)->v;
    ((struct gd_1C_s32 *)arg0)->v = *(s32 *)sp10;
    (*(s32 *)sp10) = D_800A0520;
    D_800A0520 = sp10;
    return;
}
