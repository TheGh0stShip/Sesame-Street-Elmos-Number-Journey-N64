extern s32 D_80074328;
extern s16 D_800A7F18;
extern u8 D_800AD039;
extern s32 D_80115404;
extern s32 D_8011551C;
extern s32 D_8013E7F0;
extern s32 D_8013EEF8;
s32 func_80058BA0();

void f(s32 arg0, s32 arg1) {
    D_80115404 = arg0;
    D_8013E7F0 = arg1;
    D_8011551C = (s32)&D_80074328;
    func_80058BA0(((s32)&D_8013EEF8), 0, 1);
    D_800A7F18 = 0;
    D_800AD039++;
}
