/* with the PI/SI access held, ensure the device is initialised once, run a call and return its result */
extern u8 D_80118241;
extern char D_800ACFC0[];
void func_800644B4(void);
void func_80064520(void);
void func_8005F7B4(void);
s32 func_800643C0(s32, void *);
s32 func_80058A70(void *, s32, s32);
s32 func_8005F690(void *mq)
{
    s32 ret;
    func_800644B4();
    if (D_80118241 != 1) {
        func_8005F7B4();
        func_800643C0(1, D_800ACFC0);
        func_80058A70(mq, 0, 1);
    }
    ret = func_800643C0(0, D_800ACFC0);
    D_80118241 = 1;
    func_80064520();
    return ret;
}
