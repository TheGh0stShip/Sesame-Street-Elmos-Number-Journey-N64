#include "common.h"

/* Physical voice init: alLoadNew + alResampleNew + alEnvmixerNew merged, without filter headers */
typedef struct ALHeap ALHeap;
typedef s32 (*ALDMAproc)(s32, s32, void *);
typedef ALDMAproc (*ALDMANew)(void *);
typedef struct {
    u8 unk_00[0xC];
    void *state;            /* 0x0C */
    void *lstate;           /* 0x10 */
    u8 unk_14[0x28 - 0x14];
    ALDMAproc dma;          /* 0x28 */
    void *dmaState;         /* 0x2C */
    s32 unk_30;
    s32 lastsam;            /* 0x34 */
    s32 first;              /* 0x38 */
    s32 memin;              /* 0x3C */
    void *rsState;          /* 0x40 */
    f32 ratio;              /* 0x44 */
    s32 delta;              /* 0x48 */
    s32 upitch;             /* 0x4C */
    s32 rsFirst;            /* 0x50 */
    void *emState;          /* 0x54 */
    s16 pan;                /* 0x58 */
    s16 volume;             /* 0x5A */
    s16 cvolL;              /* 0x5C */
    s16 cvolR;              /* 0x5E */
    s16 dryamt;             /* 0x60 */
    s16 wetamt;             /* 0x62 */
    u16 lratl;              /* 0x64 */
    s16 lratm;              /* 0x66 */
    s16 ltgt;               /* 0x68 */
    u16 rratl;              /* 0x6A */
    s16 rratm;              /* 0x6C */
    s16 rtgt;               /* 0x6E */
    s32 emDelta;            /* 0x70 */
    s32 segEnd;             /* 0x74 */
    s32 emFirst;            /* 0x78 */
    void *ctrlList;         /* 0x7C */
    void *ctrlTail;         /* 0x80 */
    s32 motion;             /* 0x84 */
} PVoiceFilters;
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */

void alN_PVoiceNew(PVoiceFilters *f, ALDMANew dmaNew, ALHeap *hp) {
    f->state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    f->lstate = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    f->dma = dmaNew(&f->dmaState);
    f->lastsam = 0;
    f->first = 1;
    f->memin = 0;

    f->rsState = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    f->upitch = 0;
    f->rsFirst = 1;
    f->delta = 0;
    f->ratio = 1.0f;

    f->emState = alHeapDBAlloc(0, 0, hp, 1, 0x50);
    f->emFirst = 1;
    f->motion = 0;
    f->volume = 1;
    f->ltgt = 1;
    f->rtgt = 1;
    f->cvolL = 1;
    f->cvolR = 1;
    f->dryamt = 0;
    f->wetamt = 0;
    f->lratm = 1;
    f->lratl = 0;
    f->lratm = 1;
    f->lratl = 0;
    f->emDelta = 0;
    f->segEnd = 0;
    f->pan = 0;
    f->ctrlList = 0;
    f->ctrlTail = 0;
}
