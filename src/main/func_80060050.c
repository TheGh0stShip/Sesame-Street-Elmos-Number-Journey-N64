/* osEPiStartDma (libultra 2.0F src/io/epidma.c) */
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
typedef struct { u16 type; u8 pri; u8 status; OSMesgQueue *retQueue; } OSIoMesgHdr;
typedef struct OSPiHandle OSPiHandle;
typedef struct { OSIoMesgHdr hdr; void *dramAddr; u32 devAddr; u32 size; OSPiHandle *piHandle; } OSIoMesg;
typedef struct { s32 active; } OSDevMgr;
extern OSDevMgr D_80077F40; /* __osPiDevMgr */
OSMesgQueue *func_80064620(void); /* osPiGetCmdQueue */
s32 func_80062F60(OSMesgQueue *, OSMesg, s32); /* osJamMesg */
s32 func_80058BA0(OSMesgQueue *, OSMesg, s32); /* osSendMesg */

s32 func_80060050(OSPiHandle *pihandle, OSIoMesg *mb, s32 direction) {
    register s32 ret;

    if (!D_80077F40.active) {
        return -1;
    }
    mb->piHandle = pihandle;
    if (direction == 0) {
        mb->hdr.type = 15;
    } else {
        mb->hdr.type = 16;
    }
    if (mb->hdr.pri == 1) {
        ret = func_80062F60(func_80064620(), (OSMesg)mb, 0);
    } else {
        ret = func_80058BA0(func_80064620(), (OSMesg)mb, 0);
    }
    return ret;
}
