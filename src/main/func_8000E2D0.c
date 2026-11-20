extern s32 D_80072D88;
s32 func_8005CFB0();
s32 func_8005D050();

void f(s32 arg0) {
    if (arg0 != 0) {
        func_8005D050(((s32)&D_80072D88), 3);
        return;
    }
    func_8005CFB0(((s32)&D_80072D88), 3);
}
