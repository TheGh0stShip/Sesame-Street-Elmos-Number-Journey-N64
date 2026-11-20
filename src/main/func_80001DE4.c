struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
struct n64dk_e64 { u8 b[64]; };
extern s32 D_8006F6CC;
extern s32 D_80075580;
extern s32 D_80133FF0;
extern struct n64dk_e8 *D_8013EC7C;
s32 func_8000E030();
s32 func_8000E16C();
s32 func_8000E1EC();
s32 func_8000E264();
s32 func_8000E2D0();
s32 func_8000E32C();

void f(s32 arg0) {
    void *sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;

    sp18 = 16;
    sp14 = func_8000E030();
    func_8000E2D0(1);
    func_8000E264(255, 255, 255, 255);
    sp1C = 0;
    while (!((u32)sp1C >= (u32)D_8006F6CC)) {
        sp10 = (void *)&((struct n64dk_e64 *)&D_80133FF0)[sp1C];
        func_8000E1EC(72, sp18);
        sp18 = sp18 + 16;
        func_8000E32C(((s32)sp10));
        sp1C++;
    }
    sp20 = (s32)D_8013EC7C++;
    (*(s32 *)sp20) = 0x6000000;
    ((struct gd_4_s32 *)sp20)->v = (s32)&D_80075580;
    func_8000E16C(sp14);
}
