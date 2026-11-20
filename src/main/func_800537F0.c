/* alFxNew variant (libultra drvrnew.c style): allocates the ALFx itself and builds its delay sections */
typedef struct ALHeap_s ALHeap;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        s64 force_aligned;
    } fcvec;
    s16 *fstate; /* POLEF_STATE * */
    s32 first;
} ALLowPass;
typedef struct {
    ALFilter filter;
    s16 *state; /* RESAMPLE_STATE * */
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
} ALResampler;
typedef struct {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    ALLowPass *lp;
    ALResampler *rs;
} ALDelay;
typedef struct {
    ALFilter filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    void *paramHdl;
} ALFx;
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

extern s32 D_80077920[]; /* SMALLROOM_PARAMS */
extern s32 D_80077988[]; /* BIGROOM_PARAMS */
extern s32 D_80077A10[]; /* ECHO (AL_FX_ECHO = 5)_PARAMS */
extern s32 D_80077A38[]; /* CHORUS_PARAMS */
extern s32 D_80077A60[]; /* FLANGE_PARAMS */
extern s32 D_80077A88[]; /* NULL_PARAMS */
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
void func_80059570(ALLowPass *lp); /* _init_lpfilter */
#define alHeapAlloc(hp, elem, size) func_8005C810(0, 0, (hp), (elem), (size))

#define RANGE 2.0
#define CONVERT 173123.404906676
#define LENGTH (d->output - d->input)

void func_800537F0(ALFx **fx, ALSynConfig *c, ALHeap *hp) {
    u16 i, j, k;
    s32 *param = 0;
    ALFx *r;
    ALDelay *d;

    r = alHeapAlloc(hp, 1, sizeof(ALFx));
    *fx = r;

    switch (c->fxType) {
        case 1: param = D_80077920; break;
        case 2: param = D_80077988; break;
        case 5: param = D_80077A10; break;
        case 3: param = D_80077A38; break;
        case 4: param = D_80077A60; break;
        case 6: param = c->params; break;
        default: param = D_80077A88; break;
    }

    j = 0;

    r->section_count = param[j++];
    r->length = param[j++];

    r->delay = alHeapAlloc(hp, r->section_count, sizeof(ALDelay));
    r->base = alHeapAlloc(hp, r->length, sizeof(s16));
    r->input = r->base;

    for (k = 0; k < r->length; k++)
        r->base[k] = 0;

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * RANGE) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / CONVERT) * LENGTH;
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = alHeapAlloc(hp, 1, sizeof(ALResampler));
            d->rs->state = alHeapAlloc(hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
            d->lp->fstate = alHeapAlloc(hp, 1, 8);
            d->lp->fc = param[j++];
            func_80059570(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}
