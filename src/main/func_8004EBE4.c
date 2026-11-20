void func_80050350(void);
extern s32 D_800A056C;

/* calls 8004E9E0, then latches arg into D_800A056C if it is unset */
void func_8004EBE4(s32 arg0) {
    func_80050350();
    if (D_800A056C == 0) {
        D_800A056C = arg0;
    }
}
