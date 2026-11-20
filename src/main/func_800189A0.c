struct gd_4_u16 { u8 pad[0x4]; u16 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
extern s32 D_8007A3F0;
extern s32 D_8007A3F4;
s32 func_80000EDC();
s32 func_80042080();
s32 func_80059540();
s32 func_80060E7C();

s32 f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;

    sp1C = ((arg1 * 16) + 40) + ((struct gd_20_s32 *)arg0)->v;
    sp10 = func_80042080(sp1C);
    switch (sp10) {
    case 0:
        func_80059540(((s32)&D_8007A3F0), ((s32)&D_8007A3F4), 51);
    }
    if (sp10 == 0) {
        return 0;
    }
    func_80000EDC(sp10, arg0, 10);
    sp14 = sp10 + 40;
    if (sp14 == 0) {
        return 0;
    }
    ((struct gd_C_s32 *)sp10)->v = sp14;
    func_80060E7C(((struct gd_C_s32 *)sp10)->v, ((struct gd_C_s32 *)arg0)->v, (arg1 * 16));
    if (((struct gd_20_s32 *)sp10)->v != 0) {
        sp18 = sp14 + (arg1 * 16);
        ((struct gd_10_s32 *)sp10)->v = sp18;
        ((struct gd_18_s32 *)sp10)->v = sp18 + (((struct gd_4_u16 *)sp10)->v << 4);
        func_80000EDC(((struct gd_10_s32 *)sp10)->v, ((struct gd_C_s32 *)sp10)->v, (((struct gd_4_u16 *)sp10)->v * 4));
        return sp10;
    }
    return 0;
}
