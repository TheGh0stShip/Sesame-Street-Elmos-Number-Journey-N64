/* n_alSynNew: build the n_audio synthesizer driver from its config */
typedef struct ALLink_s { struct ALLink_s *next, *prev; } ALLink;
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap;
typedef struct ALParam_s { struct ALParam_s *next; s32 delta; s16 type; s16 pad; s32 data[4]; } ALParam; /* 0x1C */
typedef void *(*ALDMANew)(void *);
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType;
} ALSynConfig;
typedef struct { void *source; s32 (*handler)(); } N_ALFilter;
typedef struct { N_ALFilter filter; s32 unk_08[3]; } N_ALMainBus; /* 0x14 */
typedef struct N_PVoice_s N_PVoice;
typedef struct {
    N_ALFilter filter;
    s32 unk_08[3];
    s32 sourceCount;
    s32 maxSources;
    N_PVoice **sources;
    void *fx;
    s32 unk_24[8];
} N_ALAuxBus; /* 0x44 */
struct N_PVoice_s { ALLink node; void *vvoice; u8 unk_0C[0x80]; }; /* 0x8C */
typedef struct {
    void *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    ALHeap *heap;
    ALParam *paramList;
    N_ALMainBus *mainBus;
    N_ALAuxBus *auxBus;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
    s32 sv_dramout;
    s32 sv_first;
} N_ALSynth;

extern N_ALSynth *D_80077BC4; /* n_syn */
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
void func_8005C77C(ALLink *ln, ALLink *to); /* alLink */
void func_80053BDC(N_PVoice *pv, ALDMANew dma, ALHeap *hp); /* n_alPVoiceNew */
void *func_800565C0(s16 bus, ALSynConfig *c, ALHeap *hp); /* n_alSynAllocFX */
s32 func_80054FE0(); /* n_alFxPull */
s32 func_80053750(); /* n_alAuxBusPull */

#define alHeapAlloc(hp, n, s) func_8005C810(0, 0, hp, n, s)

void func_80056010(ALSynConfig *c) {
    s32 i;
    N_PVoice *pv;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;

    D_80077BC4->head = NULL;
    D_80077BC4->numPVoices = c->maxPVoices;
    D_80077BC4->curSamples = 0;
    D_80077BC4->paramSamples = 0;
    D_80077BC4->outputRate = c->outputRate;
    D_80077BC4->maxOutSamples = 184;
    D_80077BC4->dma = (ALDMANew)c->dmaproc;
    D_80077BC4->sv_dramout = 0;
    D_80077BC4->sv_first = 1;

    D_80077BC4->auxBus = alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));
    D_80077BC4->auxBus->sourceCount = 0;
    D_80077BC4->auxBus->maxSources = c->maxPVoices;
    D_80077BC4->auxBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

    D_80077BC4->mainBus = alHeapAlloc(hp, 1, sizeof(N_ALMainBus));
    if (c->fxType != 0) {
        D_80077BC4->auxBus->fx = func_800565C0(0, c, hp);
        D_80077BC4->mainBus->filter.handler = func_80054FE0;
    } else {
        D_80077BC4->mainBus->filter.handler = func_80053750;
    }

    D_80077BC4->pFreeList.next = 0;
    D_80077BC4->pFreeList.prev = 0;
    D_80077BC4->pLameList.next = 0;
    D_80077BC4->pLameList.prev = 0;
    D_80077BC4->pAllocList.next = 0;
    D_80077BC4->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        func_8005C77C((ALLink *)pv, &D_80077BC4->pFreeList);
        pv->vvoice = 0;
        func_80053BDC(pv, D_80077BC4->dma, hp);
        D_80077BC4->auxBus->sources[D_80077BC4->auxBus->sourceCount++] = pv;
    }

    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    D_80077BC4->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = D_80077BC4->paramList;
        D_80077BC4->paramList = paramPtr;
    }
    D_80077BC4->heap = hp;
}
