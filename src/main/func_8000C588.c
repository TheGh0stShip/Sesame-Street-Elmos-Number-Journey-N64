extern s32 D_80082FF8;
s32 func_800583D0();
s32 func_80058A70();
s32 func_80060780();

void f(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp20[6];
    s32 sp38[2];

    func_800583D0(arg1, arg2);
    func_80060780(((s32)&sp20), 0, 0, arg0, arg1, arg2, ((s32)&D_80082FF8));
    func_80058A70(((s32)&D_80082FF8), ((s32)&sp38), 1);
}
