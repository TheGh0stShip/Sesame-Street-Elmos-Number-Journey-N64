/* libultra audio alLoadParam */
typedef s32 (*ALFilterHandler)(void *, s16 *, s32, s32, void *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALFilterHandler handler;
    void *setParam;
    s32 inp;
    s32 outp;
    s32 type;
} ALFilter;
typedef struct { u32 start, end, count; s16 state[16]; } ALADPCMloop;
typedef struct { u32 start, end, count; } ALRawLoop;
typedef struct { s32 order, npredictors; s16 book[1]; } ALADPCMBook;
typedef struct {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        struct { ALADPCMloop *loop; ALADPCMBook *book; } adpcmWave;
        struct { ALRawLoop *loop; } rawWave;
    } waveInfo;
} ALWaveTable;
typedef struct {
    ALFilter filter;
    s16 *lstate;
    ALRawLoop loop;
    ALWaveTable *table;
    s32 bookSize;
    void *dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
} ALLoadFilter;

extern s32 func_80059DA0(void *, s16 *, s32, s32, void *); /* alAdpcmPull */
extern s32 func_8005A1E4(void *, s16 *, s32, s32, void *); /* alRaw16Pull */
extern void func_8005C860(void *src, void *dst, s32 size); /* alCopy */

s32 func_8005A580(void *filter, s32 paramID, void *param) {
    ALLoadFilter *a = (ALLoadFilter *)filter;
    ALFilter *f = (ALFilter *)filter;

    switch (paramID) {
        case 5:
            a->table = (ALWaveTable *)param;
            a->memin = (s32)a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case 0:
                    f->handler = func_80059DA0;
                    a->table->len = 9 * ((s32)(a->table->len / 9));
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                                  a->table->waveInfo.adpcmWave.book->npredictors * 8;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        func_8005C860(a->table->waveInfo.adpcmWave.loop->state, a->lstate, 0x20);
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                case 1:
                    f->handler = func_8005A1E4;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 4:
            a->lastsam = 0;
            a->first = 1;
            a->sample = 0;
            if (a->table) {
                a->memin = (s32)a->table->base;
                if (a->table->type == 0) {
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                    }
                } else if (a->table->type == 1) {
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    }
                }
            }
            break;
        default:
            break;
    }
}
