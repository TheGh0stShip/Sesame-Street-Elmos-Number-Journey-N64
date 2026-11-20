s32 func_8001E5F0();
s32 func_80056F00();

void f(s32 arg0, s32 arg1) {
    s32 sp10[16];

    func_8001E5F0(((s32)&sp10), arg1);
    func_80056F00(((s32)&sp10), arg0);
}
