/* n_alSynNew (n_audio synthesizer): set up the global synthesizer, main bus, voices and update list */
typedef struct ALLink_s { struct ALLink_s *next; struct ALLink_s *prev; } ALLink;
typedef struct ALHeap_s ALHeap;
typedef void *(*ALDMANew)(void *state);
typedef struct {
    ALLink node;
    s32 unk_08; /* vvoice */
    u8 pad[0x8C - 0xC];
} N_PVoice;
typedef struct ALParam_s { struct ALParam_s *next; u8 pad[0x1C - 4]; } ALParam;
typedef struct {
    u8 pad[0x14];
    s32 sourceCount;   /* 0x14 */
    s32 maxSources;    /* 0x18 */
    N_PVoice **sources; /* 0x1C */
    void *filter;      /* 0x20 */
    u8 pad2[0x44 - 0x24];
} N_ALMainBus;
typedef struct {
    s32 unk_00;
    void *handler;     /* 0x04 */
    u8 pad[0x14 - 8];
} N_ALSave;
typedef struct {
    void *head;              /* 0x00 */
    ALLink pFreeList;        /* 0x04 */
    ALLink pAllocList;       /* 0x0C */
    ALLink pLameList;        /* 0x14 */
    s32 paramSamples;        /* 0x1C */
    s32 curSamples;          /* 0x20 */
    ALDMANew dma;            /* 0x24 */
    ALHeap *heap;            /* 0x28 */
    ALParam *paramList;      /* 0x2C */
    N_ALSave *outputFilter;  /* 0x30 */
    N_ALMainBus *mainBus;    /* 0x34 */
    s32 numPVoices;          /* 0x38 */
    s32 unk_3C;
    s32 outputRate;          /* 0x40 */
    s32 maxOutSamples;       /* 0x44 */
    s32 unk_48;
    s32 unk_4C;
} N_ALSynth;
typedef struct {
    s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses;
    void *dmaproc; ALHeap *heap; s32 outputRate; u8 fxType;
} ALSynConfig;

extern N_ALSynth *D_80077BC4; /* n_syn */
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
void *func_80051A98(s16 bus, ALSynConfig *c, ALHeap *hp); /* n_alSynAllocFX */
void func_80054FE0(void); /* main bus pull with fx */
void func_80053750(void); /* main bus pull without fx */
void func_8005C77C(ALLink *ln, ALLink *to); /* alLink */
void func_80053BDC(N_PVoice *pv, ALDMANew dma, ALHeap *hp); /* n_alLoadNew-style voice init */

#define alHeapAlloc(hp, elem, size) func_8005C810(0, 0, (hp), (elem), (size))

void func_80051828(ALSynConfig *c)
{
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
    D_80077BC4->maxOutSamples = 0xB8;
    D_80077BC4->dma = (ALDMANew)c->dmaproc;
    D_80077BC4->unk_4C = 1;
    D_80077BC4->unk_48 = 0;

    D_80077BC4->mainBus = alHeapAlloc(hp, 1, sizeof(N_ALMainBus));
    D_80077BC4->mainBus->sourceCount = 0;
    D_80077BC4->mainBus->maxSources = c->maxPVoices;
    D_80077BC4->mainBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

    D_80077BC4->outputFilter = alHeapAlloc(hp, 1, sizeof(N_ALSave));

    if (c->fxType) {
        D_80077BC4->mainBus->filter = func_80051A98(0, c, hp);
        D_80077BC4->outputFilter->handler = func_80054FE0;
    } else {
        D_80077BC4->outputFilter->handler = func_80053750;
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
        pv->unk_08 = 0;
        func_80053BDC(pv, D_80077BC4->dma, hp);
        D_80077BC4->mainBus->sources[D_80077BC4->mainBus->sourceCount++] = pv;
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
