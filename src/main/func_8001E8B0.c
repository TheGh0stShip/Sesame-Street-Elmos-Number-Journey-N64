void f(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12) {
    (*(f32 *)arg0) = -arg10;
    (*(f32 *)(arg0 + 4)) = -arg11;
    (*(f32 *)(arg0 + 8)) = -arg12;
    (*(s32 *)(arg0 + 12)) = 0;
    (*(f32 *)(arg0 + 16)) = arg7;
    (*(f32 *)(arg0 + 20)) = arg8;
    (*(f32 *)(arg0 + 24)) = arg9;
    (*(s32 *)(arg0 + 28)) = 0;
    (*(f32 *)(arg0 + 32)) = arg4;
    (*(f32 *)(arg0 + 36)) = arg5;
    (*(f32 *)(arg0 + 40)) = arg6;
    (*(s32 *)(arg0 + 44)) = 0;
    (*(f32 *)(arg0 + 48)) = arg1;
    (*(f32 *)(arg0 + 52)) = arg2;
    (*(f32 *)(arg0 + 56)) = arg3;
    (*(f32 *)(arg0 + 60)) = 1.0f;
}
