extern s32 D_002E6FC0;
extern s32 D_002E9A90;
extern s32 D_003CB0C0;
extern s32 D_003CB880;
extern s32 D_8007A398;
extern s32 D_8007A39C;
extern s32 D_800A7A60;
extern s32 D_800AF048;
extern s32 D_800AF04C;
extern s32 D_800AF050;
extern s32 D_800AF054;
extern s32 D_800AF058;
extern s32 D_800AF05C;
extern s32 D_800AF060;
extern s32 D_800AF064;
extern s32 D_800AF068;
extern s32 D_800AF070;
extern s32 D_800AF074;
extern s32 D_800AF078;
extern s32 D_800AF07C;
extern s32 D_800AF080;
extern s32 D_800AF084;
extern s32 D_800B10A0;
extern s32 D_800BD880;
extern s32 D_80108D28;
extern s32 D_80115530;
s32 func_80017788();
s32 func_80017990();
s32 func_8004E06C();
s32 func_8004ED20();
s32 func_80052A30();
s32 func_80059540();

void f(void) {
    s32 sp10;

    if ((((s32)&D_002E9A90) - ((s32)&D_002E6FC0)) < 0x34D9) {
        goto L14;
    }
    func_80059540(((s32)&D_8007A398), ((s32)&D_8007A39C), 106);
L14: ;
    func_80017788(((s32)&D_002E6FC0), ((s32)&D_800B10A0), (((s32)&D_002E9A90) - ((s32)&D_002E6FC0)));
    if ((((s32)&D_003CB880) - ((s32)&D_003CB0C0)) < 3123) {
        goto L39;
    }
    func_80059540(((s32)&D_8007A398), ((s32)&D_8007A39C), 109);
L39: ;
    func_80017788(((s32)&D_003CB0C0), ((s32)&D_80115530), (((s32)&D_003CB880) - ((s32)&D_003CB0C0)));
    D_800AF048 = 0;
    D_800AF04C = 22;
    D_800AF050 = (s32)&D_80108D28;
    D_800AF054 = 239;
    D_800AF058 = (s32)&D_800BD880;
    D_800AF05C = 0x493E0;
    D_800AF060 = (s32)&D_800B10A0;
    D_800AF064 = (s32)&D_002E9A90;
    D_800AF068 = (s32)&D_80115530;
    D_800AF070 = 128;
    D_800AF074 = 0x5622;
    D_800AF078 = 0x1000;
    D_800AF07C = 1;
    D_800AF080 = 32;
    D_800AF084 = 2048;
    sp10 = func_8004E06C(((s32)&D_800AF048));
    func_80052A30(2, 0x5622, 0, 0);
    func_80017990(0x3FFF, 0x3FFF);
    D_800A7A60 = 5;
    func_8004ED20(2);
}
