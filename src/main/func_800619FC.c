/* __scExec: start tasks on the RSP and RDP */
typedef void *OSMesg;
typedef s32 OSPri;
typedef u32 OSIntMask;
typedef u64 OSTime;
typedef struct { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct {
    u32 type; u32 flags;
    u64 *ucode_boot; u32 ucode_boot_size;
    u64 *ucode; u32 ucode_size;
    u64 *ucode_data; u32 ucode_data_size;
    u64 *dram_stack; u32 dram_stack_size;
    u64 *output_buff; u64 *output_buff_size;
    u64 *data_ptr; u32 data_size;
    u64 *yield_data_ptr; u32 yield_data_size;
} OSTask_t;
typedef union { OSTask_t t; long long force_structure_alignment; } OSTask;
typedef struct { s16 type; s8 misc[30]; } OSScMsg;
typedef struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask list;
    OSMesgQueue *msgQ;
    OSMesg msg;
    OSTime startTime;
    OSTime totalTime;
} OSScTask;
typedef struct SCClient_s { struct SCClient_s *next; OSMesgQueue *msgQ; } OSScClient;
typedef struct {
    OSScMsg retraceMsg;
    OSScMsg prenmiMsg;
    OSMesgQueue interruptQ;
    OSMesg intBuf[8];
    OSMesgQueue cmdQ;
    OSMesg cmdMsgBuf[8];
    u8 thread[0x1B0]; /* OSThread */
    OSScClient *clientList;
    OSScTask *audioListHead;
    OSScTask *gfxListHead;
    OSScTask *audioListTail;
    OSScTask *gfxListTail;
    OSScTask *curRSPTask;
    OSScTask *curRDPTask;
    u32 frameCount;
    s32 doAudio;
} OSSched;
void func_80058580(void); /* osWritebackDCacheAll */
OSTime func_800586B0(void); /* osGetTime */
void func_80063840(OSTask *); /* osSpTaskLoad */
void func_80063A4C(OSTask *); /* osSpTaskStartGo */
s32 func_80063750(void *, u64); /* osDpSetNextBuffer */
static int dp_busy = 0;
static int dpCount = 0;
void func_800619FC(OSSched *sc, OSScTask *sp, OSScTask *dp)
{
    int rv;
    if (sp) {
        if (sp->list.t.type == 2) {
            func_80058580();
        }
        sp->state &= ~(0x10 | 0x20);
        sp->startTime = func_800586B0();
        func_80063840(&sp->list);
        func_80063A4C(&sp->list);
        sc->curRSPTask = sp;
        if (sp == dp)
            sc->curRDPTask = dp;
    }
    if (dp && (dp != sp)) {
        rv = func_80063750(dp->list.t.output_buff, *dp->list.t.output_buff_size);
        dp_busy = 1;
        dpCount = 0;
        sc->curRDPTask = dp;
    }
}
