s32 func_80042080();
s32 func_8004DE00();

s32 f(s32 arg0) {
    register s32 reg_s0;

    reg_s0 = func_80042080(8);
    func_8004DE00(reg_s0, 8);
    (*(s32 *)reg_s0) = arg0;
    return reg_s0;
}
