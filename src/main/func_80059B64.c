/* alLoadNew (libultra audio/drvrnew.c) */
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    void *setParam;
    s16 *inp;
    s16 *outp;
} ALFilter;
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void *state);
typedef struct {
    ALFilter filter;
    void *state;      /* 0x14 */
    void *lstate;     /* 0x18 */
    void *table;      /* 0x1C */
    s32 bookSize;     /* 0x20 */
    void *dmaFunc;    /* 0x24 */
    void *loop;       /* 0x28 */
    s32 unk_2C;
    ALDMAproc dma;    /* 0x30 */
    void *dmaState;   /* 0x34 */
    s32 sample;       /* 0x38 */
    s32 lastsam;      /* 0x3C */
    s32 first;        /* 0x40 */
    s32 memin;        /* 0x44 */
} ALLoadFilter;
typedef struct ALHeap_s ALHeap;
void func_8005B670(ALFilter *f, void *h, void *s, s32 type);               /* alFilterNew */
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);    /* alHeapDBAlloc */
s32 func_80059DA0();   /* alAdpcmPull */
s32 func_8005A580();   /* alLoadParam */
void func_80059B64(ALLoadFilter *f, ALDMANew dmaNew, ALHeap *hp)
{
    func_8005B670(&f->filter, func_80059DA0, func_8005A580, 0);
    f->state = func_8005C810(0, 0, hp, 1, 32);
    f->lstate = func_8005C810(0, 0, hp, 1, 32);
    f->dma = dmaNew(&f->dmaState);
    f->lastsam = 0;
    f->first = 1;
    f->memin = 0;
}
