extern s32 D_8006F6C8;
extern s32 D_80108D24;
extern s16 D_80120348;
extern s32 D_80120368;
extern s16 D_80128418;
extern s32 D_80128438;
extern s32 D_80200000;
extern s32 D_80225800;
s32 func_8005F420();

void f(void) {
    D_80120348 = 6;
    D_80120368 = (s32)&D_80200000;
    D_80128418 = 7;
    D_80128438 = (s32)&D_80225800;
    func_8005F420(66);
    D_80108D24 = 1;
    D_8006F6C8 = 0;
}
