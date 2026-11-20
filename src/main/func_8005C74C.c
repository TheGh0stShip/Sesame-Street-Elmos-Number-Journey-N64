/* if D_80077F00 is set, call func_8005CEE0 and clear it */
extern s32 D_80077F00;
void func_8005CEE0(void);
void func_8005C74C(void)
{
    if (D_80077F00 != 0) {
        func_8005CEE0();
        D_80077F00 = 0;
    }
}
