/* __rmonGetRegisterContents */
typedef struct { u8 pad[0x20]; u32 context[1]; } OSThread;
OSThread *func_80066D88(s32 threadNumber); /* __rmonGetTCB */
u32 func_80066540(s32 regNumber); /* __rmonGetRCPRegister */
u32 func_80066CA4(s32 method, s32 threadNumber, s32 regNumber) {
    if (method == 0) {
        u32 *regPointer;
        OSThread *tptr;
        if (regNumber >= 1 && regNumber <= 25) {
            regNumber -= 1;
        } else if (regNumber >= 28 && regNumber <= 31) {
            regNumber -= 3;
        } else {
            return 0;
        }
        tptr = func_80066D88(threadNumber);
        if (tptr == NULL) {
            return 0;
        }
        regPointer = (u32 *)&tptr->context;
        regPointer += regNumber;
        return *regPointer;
    } else {
        return func_80066540(regNumber);
    }
}
