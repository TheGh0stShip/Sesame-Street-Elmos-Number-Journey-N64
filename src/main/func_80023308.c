extern s32 D_8004D4E8;
extern s32 D_8004D9D0;
extern s32 D_800AF040;
extern s32 D_80112E90;
s32 func_8004B208(s32, s32, s32, s32, s32, f32);

void f(void) {
    s32 sp18;

    sp18 = func_8004B208(D_80112E90, D_800AF040, ((s32)&D_8004D4E8), ((s32)&D_8004D9D0), 48, 1.0f);
}
