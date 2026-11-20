struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
s32 func_800375A8();

void f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;

    sp10 = arg0;
    sp14 = ((struct gd_8_s32 *)arg1)->v;
    sp18 = ((struct gd_C_s32 *)arg1)->v;
    sp1C = ((struct gd_4_s32 *)arg1)->v;
    switch ((u32)sp14) {
    case 1:
        ((struct gd_1F0_s32 *)sp10)->v = sp1C;
        func_800375A8(sp10, 36);
        return;
    case 2:
        ((struct gd_1F0_s32 *)sp10)->v = sp1C;
        func_800375A8(sp10, 37);
        return;
    case 3:
        ((struct gd_1F0_s32 *)sp10)->v = sp1C;
        func_800375A8(sp10, 38);
        return;
    }
    return;
}
