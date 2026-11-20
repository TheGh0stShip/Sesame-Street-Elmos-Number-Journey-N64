extern u16 D_800A7F68;
extern s32 D_800A7F6C;
extern s32 D_80108D24;
extern s32 D_80109D4C;
extern s32 D_80115330;
extern s32 D_8011533C;
extern s32 D_80115354;
extern s32 D_80115358;
extern s32 D_8011535C;
extern s32 D_80115360;
extern s32 D_80115408;
s32 func_8001791C();
s32 func_80041A80();
s32 func_80041CAC();
s32 func_8004200C();
s32 func_8004E884();
s32 func_80052D88();

void f(s32 arg0) {
    D_80108D24 = 0;
    func_8001791C();
    if (D_800A7F6C != 0) {
        func_8004E884(D_800A7F6C);
    }
    func_80052D88(D_800A7F68);
    D_80109D4C = 8;
    func_8004200C(D_80115408);
    D_80115358 = arg0;
    D_80115354 = 0;
    D_8011535C = 20;
    D_80115360 = 2;
    D_8011533C = 0;
    if (func_80041CAC((*(s32 *)D_80115408), 3, 0xFFFF) == 0) {
        func_80041A80(((s32)&D_80115330), D_80115408, 3, 0);
    }
}
