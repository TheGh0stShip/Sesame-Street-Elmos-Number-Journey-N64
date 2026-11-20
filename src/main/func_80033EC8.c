extern s32 D_800AD018;
extern u8 D_80109D68[][30];
s32 func_800217C4();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    u8 sp30;

    sp20 = arg0;
    sp24 = arg1;
    sp28 = arg2;
    sp2C = arg3;
    sp10 = 0;
    while (!(sp10 >= 4)) {
        if ((*(s32 *)(((sp10 * 4) + ((s32)&sp10)) + 16)) < 3) {
            (*(s32 *)(((sp10 * 4) + ((s32)&sp10)) + 16)) = 3;
        }
        for (sp14 = 0; !(sp14 >= (*(s32 *)(((sp10 * 4) + ((s32)&sp10)) + 16))); sp14++) {
            D_80109D68[sp10][sp14] = sp14;
        }
        for (sp14 = 0; !(sp14 >= 120); sp14++) {
            sp18 = func_800217C4((*(s32 *)(((sp10 * 4) + ((s32)&sp10)) + 16)));
            sp1C = func_800217C4((*(s32 *)(((sp10 * 4) + ((s32)&sp10)) + 16)));
            sp30 = D_80109D68[sp10][sp18];
            D_80109D68[sp10][sp18] = D_80109D68[sp10][sp1C];
            D_80109D68[sp10][sp1C] = sp30;
        }
        (*(s32 *)((sp10 * 4) + ((s32)&D_800AD018))) = 0;
        sp10++;
    }
}
