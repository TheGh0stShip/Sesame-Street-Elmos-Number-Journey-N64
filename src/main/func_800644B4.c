/* __osPiGetAccess (with __osPiCreateAccessQueue inlined) */
typedef void *OSMesg;
typedef struct { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
extern s32 D_80078DB0;          /* __osPiAccessQueueEnabled */
extern OSMesgQueue D_8013EF10;  /* __osPiAccessQueue */
extern OSMesg D_800A36F0[1];    /* piAccessBuf */
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count); /* osCreateMesgQueue */
s32 func_80058BA0(OSMesgQueue *mq, OSMesg msg, s32 flag);    /* osSendMesg */
s32 func_80058A70(OSMesgQueue *mq, OSMesg *msg, s32 flag);   /* osRecvMesg */
static void __osPiCreateAccessQueue(void) {
    D_80078DB0 = 1;
    func_800585B0(&D_8013EF10, D_800A36F0, 1);
    func_80058BA0(&D_8013EF10, NULL, 0);
}
void func_800644B4(void) {
    OSMesg dummyMesg;
    if (!D_80078DB0) {
        __osPiCreateAccessQueue();
    }
    func_80058A70(&D_8013EF10, &dummyMesg, 1);
}
