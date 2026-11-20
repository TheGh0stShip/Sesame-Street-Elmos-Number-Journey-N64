s32 func_8001E5F0();
s32 func_80056F00();
s32 func_80057180();
s32 func_80057790(s32, s32, s32, s32, f32);
s32 func_80057D90();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp18[16];
    s32 sp58[16];
    s32 sp98[16];
    s32 spD8[16];

    func_8001E5F0(((s32)&sp18), arg1);
    func_80057790(((s32)&sp98), arg2, arg3, arg4, arg5);
    func_80057180(((s32)&sp98), ((s32)&sp18), ((s32)&spD8));
    func_80057D90(((s32)&sp58), arg6, arg7, arg8);
    func_80057180(((s32)&sp58), ((s32)&spD8), ((s32)&spD8));
    func_80056F00(((s32)&spD8), arg0);
}
