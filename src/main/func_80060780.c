/* osPiStartDma */
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;
typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;
typedef struct {
    s32 active;
} OSDevMgr;
extern OSDevMgr D_80077F40; /* __osPiDevMgr */
OSMesgQueue *func_80064620(void); /* osPiGetCmdQueue */
s32 func_80062F60(OSMesgQueue *, OSMesg, s32); /* osJamMesg */
s32 func_80058BA0(OSMesgQueue *, OSMesg, s32); /* osSendMesg */

s32 func_80060780(OSIoMesg *mb, s32 priority, s32 direction, u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq)
{
    register s32 ret;

    if (!D_80077F40.active)
        return -1;
    if (direction == 0)
        mb->hdr.type = 11;
    else
        mb->hdr.type = 12;
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = devAddr;
    mb->size = size;
    mb->piHandle = NULL;
    if (priority == 1)
        ret = func_80062F60(func_80064620(), (OSMesg)mb, 0);
    else
        ret = func_80058BA0(func_80064620(), (OSMesg)mb, 0);
    return ret;
}
