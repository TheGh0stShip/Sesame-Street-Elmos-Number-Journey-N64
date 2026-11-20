/* __osSiCreateAccessQueue */
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
extern u32 D_80078DB0;           /* __osSiAccessQueueEnabled */
extern OSMesgQueue D_8013EF10;   /* __osSiAccessQueue */
extern OSMesg D_800A36F0[];      /* siAccessBuf */
void func_800585B0(OSMesgQueue *mq, OSMesg *msg, s32 count); /* osCreateMesgQueue */
s32 func_80058BA0(OSMesgQueue *mq, OSMesg msg, s32 flag);    /* osSendMesg */
void func_80064460(void) {
    D_80078DB0 = 1;
    func_800585B0(&D_8013EF10, D_800A36F0, 1);
    func_80058BA0(&D_8013EF10, NULL, 0);
}
