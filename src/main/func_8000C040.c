extern s32 D_8000C0A8;
extern s32 D_8007D960;
extern s32 D_80081B10;
s32 func_800585E0();
s32 func_80058740();
s32 func_80058EA0();

void f(s32 arg0) {
    s32 pad0[16];
    s8 pad1;

    func_80058740();
    func_800585E0(((s32)&D_8007D960), 1, ((s32)&D_8000C0A8), arg0, ((s32)&D_80081B10), 10);
    func_80058EA0(((s32)&D_8007D960));
}
