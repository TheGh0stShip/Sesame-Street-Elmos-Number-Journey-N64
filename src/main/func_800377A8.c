extern s32 D_800A7F2C;
extern s32 D_8012F138;
extern s32 D_8012F154;
s32 func_800179D8();
s32 func_8004E938();

void f(void) {
    if (func_8004E938(D_800A7F2C) == 0) {
        switch (D_8012F154) {
        case 1:
            D_800A7F2C = func_800179D8(16);
            D_8012F138 = 256;
            return;
        case 9:
            D_800A7F2C = func_800179D8(24);
            D_8012F138 = 256;
            return;
        case 7:
            D_800A7F2C = func_800179D8(54);
            D_8012F138 = 256;
            return;
        case 11:
            D_800A7F2C = func_800179D8(53);
            D_8012F138 = 256;
            return;
        }
    }
}
