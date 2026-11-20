
extern s32 D_80000300;
extern s32 D_80082FF8;
extern s32 D_80083010;
extern s8 D_800A7A5C;
extern s8 D_800A7A5D;
extern s32 D_800A7F14;
extern s32 D_800A91E8;
extern s32 D_800ACFB4;
extern s8 D_800B1090;
extern s16 D_800B1092;
extern s8 D_800B1098;
extern s8 D_80106C6C;
extern s32 D_80106C7C;
extern s32 D_80106C84;
extern s32 D_80106C88;
extern s8 D_80107CD8;
extern s32 D_80107CF8;
extern s32 D_80108D28;
extern f32 D_80108FB4;
extern s32 D_80109D08;
extern s8 D_80109DFC;
extern s32 D_80109E08;
extern s8 D_80109E30;
extern s32 D_8010E650;
extern s32 D_80115400;
extern s32 D_80115408;
extern s32 D_80128440;
extern s32 D_8012F154;
extern s16 D_8013C7C0;
extern s32 D_80140F30;
s32 func_80000F60();
s32 func_80001EE8();
s32 func_80017520();
s32 func_80019258();
s32 func_80041A50();
s32 func_80042620();
s32 func_800585B0();
s32 func_80061290();
s32 func_800613D8();
s32 func_800614C0();

/* Initialises game state at boot (video mode by TV type at 0x80000300). */
void func_8000C60C(void) {
    func_800585B0(((s32)&D_80082FF8), ((s32)&D_80083010), 1);
    func_800585B0(((s32)&D_8010E650), ((s32)&D_80109D08), 16);
    func_800585B0(((s32)&D_80107CF8), ((s32)&D_80109E08), 8);
    if (D_80000300 == 1) {
        func_80061290(((s32)&D_80108D28), ((s32)&D_80140F30), 240, 2, 1);
        goto L48;
    }
    if (D_80000300 == 2) {
        func_80061290(((s32)&D_80108D28), ((s32)&D_80140F30), 240, 30, 1);
        goto L48;
    }
    loop: goto loop;
L48: ;
    func_800613D8(((s32)&D_80108D28), ((s32)&D_800ACFB4), ((s32)&D_8010E650));
    D_800A7F14 = func_800614C0(((s32)&D_80108D28));
    func_80042620();
    D_80115408 = (s32)&D_800A91E8;
    func_80041A50(D_80115408);
    D_8012F154 = 1;
    D_80128440 = 255;
    D_80109DFC = 0;
    D_80115400 = 0;
    D_80106C7C = 0;
    D_80106C84 = 0;
    D_80106C6C = 0;
    D_8013C7C0 = 0;
    D_800A7A5D = 0;
    D_80109E30 = 0;
    D_800A7A5C = 0;
    D_800B1098 = 0;
    D_800B1090 = 0;
    D_80107CD8 = 0;
    D_800B1092 = 240;
    D_80108FB4 = 4e+01f;
    func_80000F60();
    func_80019258();
    func_80001EE8();
    func_80017520();
    D_80106C88 = 0;
}
