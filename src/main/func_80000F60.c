extern s32 D_8006EE70;
extern s32 D_8007BF50;
extern s16 D_8007BF78;
extern s8 D_800A914A;
extern s32 D_8010E650;
s32 func_800585B0();
s32 func_80058CD0();
s32 func_8005F870();

void f(void) {
    s32 sp10[6];
    s32 sp28;
    s32 sp2C;

    D_800A914A = 3;
    func_800585B0(((s32)&sp10), ((s32)&sp28), 1);
    func_80058CD0(5, ((s32)&sp10), 1);
    sp2C = func_8005F870(((s32)&sp10), ((s32)&D_8006EE70), ((s32)&D_8007BF50));
    if (sp2C != 0) {
    }
    D_8007BF78 = 5;
    func_80058CD0(5, ((s32)&D_8010E650), ((s32)&D_8007BF78));
}
