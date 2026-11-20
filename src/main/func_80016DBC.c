extern s32 D_8007A15C;
extern s32 D_8007A1D8;
extern s32 D_8007A254;
extern s32 D_8007A2CC;
extern s32 D_800A7A80;
s32 func_8001649C();

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;

    sp2C = 0;
    sp28 = func_8001649C(arg2, arg0, 257, ((s32)&D_8007A15C), ((s32)&D_8007A1D8), arg5, arg3, arg7, ((s32)&sp2C), ((s32)&D_800A7A80));
    if ((sp28 != 0) || ((*(s32 *)arg3) == 0)) {
        return sp28;
    }
    sp28 = func_8001649C(((arg0 * 4) + arg2), arg1, 0, ((s32)&D_8007A254), ((s32)&D_8007A2CC), arg6, arg4, arg7, ((s32)&sp2C), ((s32)&D_800A7A80));
    return sp28;
}
