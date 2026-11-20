extern s8 D_800A7F24;
extern s32 D_800AD018;
extern s32 D_80106C88;
extern s32 D_80106C9C;
extern s32 D_80108D20;
extern u8 D_80109D68[][30];
extern s32 D_8012FE90;
s32 func_80033EC8();
s32 func_80060E58();
s32 func_80060E7C();
s32 func_80069C70();

void f(s32 arg0, s32 arg1) {
    if (D_80106C88 == 0) {
        func_80033EC8(9, 8, 9, 26);
        D_80106C88 = 1;
    }
    D_80106C9C = func_80060E58(arg0);
    func_80069C70(((s32)&D_8012FE90), 0, 30);
    func_80060E7C(((s32)&D_8012FE90), arg0, D_80106C9C);
    if ((*(s32 *)((arg1 * 4) + ((s32)&D_800AD018))) >= D_80106C9C) {
        (*(s32 *)((arg1 * 4) + ((s32)&D_800AD018))) = 0;
    }
    D_80108D20 = D_80109D68[arg1][(*(s32 *)((arg1 * 4) + ((s32)&D_800AD018)))++];
    D_800A7F24 = *(u8 *)(D_80108D20 + ((s32)&D_8012FE90));
}
