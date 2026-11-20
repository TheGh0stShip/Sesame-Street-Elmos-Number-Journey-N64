struct gd_4_s32 { u8 pad[0x4]; s32 v; };
extern s32 D_8016DA28;
extern s32 D_80177360;
extern s32 D_801811A8;
extern s32 D_801A4304;
s32 func_800409A0();

void f(s32 arg0) {
    register s32 reg_s0;
    s32 sp10;

    reg_s0 = arg0;
    switch (((struct gd_4_s32 *)reg_s0)->v) {
    case 0x5001:
        sp10 = (s32)&D_801811A8;
        break;
    case 0x5003:
        sp10 = (s32)&D_80177360;
        break;
    case 0x5005:
        sp10 = (s32)&D_8016DA28;
        break;
    case 0x6000:
        sp10 = (s32)&D_801A4304;
        break;
    }
    func_800409A0(arg0, sp10);
}
