/* Loop: streams data from D_800A7A50 in 0x4000-byte chunks via func_80060780. */


extern s32 D_8007A8C0;
extern s32 D_8007A8C4;
extern s32 D_800A7A50;
extern s32 D_80106C74;
extern s32 D_8010E668;
extern s32 D_80118244;
extern s32 D_80140F30;
s32 func_80042080();
s32 func_800583D0();
s32 func_800585B0();
s32 func_80058A70();
s32 func_80059540();
s32 func_80060780();

void f(s32 arg0) {
    s32 sp20[6];
    s32 sp38;
    s32 sp3C;
    s8 pad0;

    func_800585B0(((s32)&D_80140F30), ((s32)&D_8010E668), 1);
    while (1) {
        func_80058A70(((s32)&D_80140F30), 0, 1);
        sp38 = func_80042080(D_800A7A50);
        D_80118244 = sp38;
        switch (D_80118244) {
        case 0:
            func_80059540(((s32)&D_8007A8C0), ((s32)&D_8007A8C4), 216);
        }
        for (; !(D_800A7A50 == 0); D_800A7A50 = D_800A7A50 - sp3C) {
            if ((u32)D_800A7A50 >= 0x4001) {
                sp3C = 0x4000;
            } else {
                sp3C = D_800A7A50;
            }
            func_800583D0(sp38, sp3C);
            func_80060780(((s32)&sp20), 0, 0, D_80106C74, sp38, sp3C, ((s32)&D_80140F30));
            func_80058A70(((s32)&D_80140F30), 0, 1);
            D_80106C74 = D_80106C74 + sp3C;
            sp38 = sp38 + sp3C;
        }
        D_80106C74 = 0;
    }
}
