s32 func_8001E5F0();
s32 func_80056F00();
s32 func_80057180();
s32 func_80057D90();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp10[16];
    s32 sp50[16];
    s32 sp90[16];

    func_8001E5F0(((s32)&sp10), arg1);
    func_80057D90(((s32)&sp50), arg2, arg3, arg4);
    func_80057180(((s32)&sp50), ((s32)&sp10), ((s32)&sp90));
    func_80056F00(((s32)&sp90), arg0);
}
