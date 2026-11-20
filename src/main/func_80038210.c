extern u16 D_80107CBA;
extern s32 D_8012FE68;
s32 func_800375A8();

void f(s32 arg0) {
    if (((D_80107CBA & 0x8000) & 0xFFFF) != 0) {
        D_8012FE68 = 1;
        func_800375A8(arg0, 35);
    }
}
