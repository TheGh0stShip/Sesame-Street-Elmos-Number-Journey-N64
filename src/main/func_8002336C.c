extern s32 D_800233C8;
extern u8 D_800AD03A;
s32 func_80001424();

void f(s32 arg0) {
    if (D_800AD03A == 0) {
        return;
    }
    func_80001424(((s32)&D_800233C8), arg0, 1, 0, 0);
}
