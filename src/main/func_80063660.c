/* one-time creates a queue bound to OS event 15, stores two values, polls func_80063440 until it succeeds, then waits for the event */
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
extern s32 D_80077FF0;
extern OSMesgQueue D_800A2470;
extern OSMesg D_800A2488[];
extern s32 D_8012FEB4;
extern s32 D_80106C98;
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count);  /* osCreateMesgQueue */
void func_80058CD0(s32 event, OSMesgQueue *mq, OSMesg msg);   /* osSetEventMesg */
s32 func_80058A70(OSMesgQueue *mq, OSMesg *msg, s32 flag);    /* osRecvMesg */
s32 func_80063440(void *buf, s32 a, s32 b);
void func_80063660(s32 a, s32 b) {
    s32 buf[2];
    s32 done = 0;
    if (D_80077FF0 == 0) {
        func_800585B0(&D_800A2470, D_800A2488, 1);
        func_80058CD0(15, &D_800A2470, NULL);
        D_80077FF0 = 1;
    }
    D_8012FEB4 = a;
    D_80106C98 = b;
    do {
        done += func_80063440(buf, 1, 5);
    } while (done == 0);
    func_80058A70(&D_800A2470, NULL, 1);
}
