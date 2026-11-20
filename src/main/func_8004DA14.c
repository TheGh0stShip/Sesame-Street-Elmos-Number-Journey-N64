s32 func_80042080();
s32 func_80069C70();

s32 f(s32 arg0) {
    register s32 reg_s0;

    reg_s0 = func_80042080(16);
    func_80069C70(reg_s0, 0, 16);
    (*(s32 *)reg_s0) = arg0;
    return reg_s0;
}
