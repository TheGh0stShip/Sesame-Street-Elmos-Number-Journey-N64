/* thread loop: create two queues bound to events 0x12/0x13, then forever wait on the first, poll until ready, wait on the second */
extern char D_800A1F50[];
extern char D_800A1F68[];
extern char D_800A1F70[];
extern char D_800A1F88[];
void func_800585B0(void *mq, void *msg, s32 count);  /* osCreateMesgQueue */
void func_80058CD0(s32 ev, void *mq, s32 msg);       /* osSetEventMesg */
s32 func_80058A70(void *mq, void *msg, s32 flag);    /* osRecvMesg */
void func_800606E4(void);
s32 func_80063440(void *buf, s32, s32);
void func_80060750(void);
void func_8005FD9C(void)
{
    u8 buf[8];
    s32 n;

    func_800585B0(D_800A1F50, D_800A1F68, 1);
    func_800585B0(D_800A1F70, D_800A1F88, 1);
    func_80058CD0(0x12, D_800A1F50, 0);
    func_80058CD0(0x13, D_800A1F70, 0);
    for (;;) {
        func_80058A70(D_800A1F50, 0, 1);
        func_800606E4();
        n = 0;
        do {
            n += func_80063440(buf, 1, 9);
        } while (n == 0);
        func_80058A70(D_800A1F70, 0, 1);
        func_80060750();
    }
}
