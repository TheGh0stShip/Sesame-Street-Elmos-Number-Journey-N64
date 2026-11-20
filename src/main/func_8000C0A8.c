/* Boot thread body: start PI manager, rdb, rmon and main threads, then idle forever. */
typedef struct { s32 unk[0x1B0 / 4]; } OSThread;
void func_8005FBD0(s32 pri, void *cmdQ, void *cmdBuf, s32 cmdMsgCnt); /* osCreatePiManager */
void func_800594C0(void *buf, u32 size); /* osInitRdb */
void func_800585E0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri); /* osCreateThread */
void func_80058EA0(OSThread *t); /* osStartThread */
void func_80058D30(OSThread *t, s32 pri); /* osSetThreadPri */
void func_80060A40(void *); /* rmonMain */
void func_8000C188(void *);
extern u8 D_80082FE0[];
extern u8 D_80082CC0[];
extern u8 D_80128448[];
extern OSThread D_80081B10;
extern OSThread D_8007D7B0;
extern u8 D_8007FB10[];
void func_8000C0A8(void *arg) {
    func_8005FBD0(150, D_80082FE0, D_80082CC0, 200);
    func_800594C0(D_80128448, 0x100);
    func_800585E0(&D_80081B10, 0, func_80060A40, 0, D_80082CC0, 250);
    func_80058EA0(&D_80081B10);
    func_800585E0(&D_8007D7B0, 6, func_8000C188, arg, D_8007FB10, 10);
    func_80058EA0(&D_8007D7B0);
    func_80058D30(0, 0);
    for (;;) {
    }
}
