extern s32 D_8011551C;
extern u8 D_80128443;
extern s32 D_8012FE68;
s32 func_8001E1E4();
s32 func_8005F590();

void f(s32 arg0) {
    if ((u32)D_80128443 < 240) {
        D_80128443 = D_80128443 + 16;
        return;
    }
    D_80128443 = 255;
    func_8005F590(1);
    if (D_8011551C == 0) {
        if (D_8012FE68 == 22) {
            func_8001E1E4(18);
            return;
        }
        func_8001E1E4(12);
    }
}
