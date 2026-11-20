/* __scHandleRDP: RDP task finished */
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
s32 func_800618D8(OSSched *, OSScTask *); /* __scTaskComplete */
s32 func_80061B10(OSSched *, OSScTask **, OSScTask **, s32); /* __scSchedule */
void func_800619FC(OSSched *, OSScTask *, OSScTask *); /* __scExec */
void func_800617F4(OSSched *sc)
{
    OSScTask *t, *sp = 0, *dp = 0;
    s32 state;
    t = sc->curRDPTask;
    sc->curRDPTask = 0;
    t->state &= ~1;
    func_800618D8(sc, t);
    state = (sc->curRSPTask == 0) << 1; if (sc->curRDPTask == 0) state |= 1;
    if ((func_80061B10(sc, &sp, &dp, state)) != state)
        func_800619FC(sc, sp, dp);
}
