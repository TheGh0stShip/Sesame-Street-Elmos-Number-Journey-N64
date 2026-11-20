/* alSynNew: initialise the synthesis driver from a config */
typedef struct ALLink_s { struct ALLink_s *next; struct ALLink_s *prev; } ALLink;
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap;
typedef struct ALFilter_s ALFilter;
typedef struct ALParam_s { struct ALParam_s *next; u8 unk_04[0x18]; } ALParam;
typedef struct { u8 unk_00[0x1C]; } ALSave;
typedef struct { u8 unk_00[0x4C]; } ALAuxBus;
typedef struct { u8 unk_00[0x20]; } ALMainBus;
typedef struct { u8 unk_00[0x48]; } ALLoadFilter;
typedef struct { u8 unk_00[0x34]; } ALResampler;
typedef struct { u8 unk_00[0x4C]; } ALEnvMixer;
typedef void *(*ALDMANew)(void *);
typedef struct ALPlayer_s ALPlayer;
typedef struct PVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    ALLoadFilter decoder;
    ALResampler resampler;
    ALEnvMixer envmixer;
    s32 offset;
} PVoice;
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType;
    s32 *params;
} ALSynConfig;
typedef struct {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    ALHeap *heap;
    ALParam *paramList;
    ALMainBus *mainBus;
    ALAuxBus *auxBus;
    ALFilter *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;

void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
#define alHeapAlloc(hp, n, s) func_8005C810(0, 0, hp, n, s)
void func_80059D58(ALSave *s);                                   /* alSaveNew */
void func_80059CA0(ALAuxBus *b, void *sources, s32 max);         /* alAuxBusNew */
void func_80059CFC(ALMainBus *b, void *sources, s32 max);        /* alMainBusNew */
s32 func_8005CEF0(ALSynth *drvr, s16 bus, ALSynConfig *c, ALHeap *hp); /* alSynAllocFX */
s32 func_8005B7D0(void *f, s32 id, void *p);                     /* alMainBusParam */
void func_8005C77C(ALLink *ln, ALLink *to);                      /* alLink */
void func_80059B64(ALLoadFilter *f, ALDMANew dma, ALHeap *hp);   /* alLoadNew */
s32 func_8005A580(void *f, s32 id, void *p);                     /* alLoadParam */
void func_80059C14(ALResampler *r, ALHeap *hp);                  /* alResampleNew */
s32 func_8005B9EC(void *f, s32 id, void *p);                     /* alResampleParam */
void func_80059AAC(ALEnvMixer *e, ALHeap *hp);                   /* alEnvmixerNew */
s32 func_8005AEC0(void *f, s32 id, void *p);                     /* alEnvmixerParam */
s32 func_8005A98C(void *f, s32 id, void *p);                     /* alAuxBusParam */
s32 func_8005C6F8(void *f, s32 id, void *p);                     /* alSaveParam */

void func_8005C8A0(ALSynth *drvr, ALSynConfig *c)
{
    s32 i;
    PVoice *pv;
    PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALSave *save;
    ALFilter *sources;
    ALParam *params;
    ALParam *paramPtr;

    drvr->head = NULL;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = 160;
    drvr->dma = (ALDMANew) c->dmaproc;

    save = alHeapAlloc(hp, 1, sizeof(ALSave));
    func_80059D58(save);
    drvr->outputFilter = (ALFilter *)save;

    drvr->auxBus = alHeapAlloc(hp, 1, sizeof(ALAuxBus));
    drvr->maxAuxBusses = 1;
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter *));
    func_80059CA0(drvr->auxBus, sources, c->maxPVoices);

    drvr->mainBus = alHeapAlloc(hp, 1, sizeof(ALMainBus));
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter *));
    func_80059CFC(drvr->mainBus, sources, c->maxPVoices);

    if (c->fxType != 0) {
        func_8005CEF0(drvr, 0, c, hp);
    } else
        func_8005B7D0(drvr->mainBus, 2, &drvr->auxBus[0]);

    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;

    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        func_8005C77C((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;

        func_80059B64(&pv->decoder, drvr->dma, hp);
        func_8005A580(&pv->decoder, 1, 0);

        func_80059C14(&pv->resampler, hp);
        func_8005B9EC(&pv->resampler, 1, &pv->decoder);

        func_80059AAC(&pv->envmixer, hp);
        func_8005AEC0(&pv->envmixer, 1, &pv->resampler);

        func_8005A98C(drvr->auxBus, 2, &pv->envmixer);

        pv->channelKnob = (ALFilter *)&pv->envmixer;
    }

    func_8005C6F8(save, 1, drvr->mainBus);

    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }

    drvr->heap = hp;
}
