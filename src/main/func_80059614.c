/* alFxNew: build the reverb/chorus effect from a preset or custom parameter list */
typedef struct ALHeap_s ALHeap;
typedef s32 (*ALCmdHandler)(void *, s16 *, s32, s32, s16 *);
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef s32 (*ALSetFXParam)(void *, s32, void *);
typedef short POLEF_STATE[4];
typedef short RESAMPLE_STATE[16];

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct ALParam_s ALParam;

typedef struct ALResampler_s {
    ALFilter filter;
    RESAMPLE_STATE *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 motion;
} ALResampler;

typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        s64 force_aligned;
    } fcvec;
    POLEF_STATE *fstate;
    s32 first;
} ALLowPass;

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
    ALSetFXParam paramHdl;
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

extern void func_8005B670(ALFilter *, ALCmdHandler, ALSetParam, s32); /* alFilterNew */
extern void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
extern s32 func_8005BAB0(void *, s16 *, s32, s32, s16 *); /* alFxPull */
extern s32 func_8005BE24(void *, s32, void *);            /* alFxParam */
extern s32 func_8005BE38(void *, s32, void *);            /* alFxParamHdl */
extern s32 D_80077C50[]; /* SMALLROOM_PARAMS */
extern s32 D_80077CB8[]; /* BIGROOM_PARAMS */
extern s32 D_80077D40[]; /* ECHO_PARAMS */
extern s32 D_80077D68[]; /* CHORUS_PARAMS */
extern s32 D_80077D90[]; /* FLANGE_PARAMS */
extern s32 D_80077DB8[]; /* NULL_PARAMS */

#define alHeapAlloc(hp, elem, size) func_8005C810(0, 0, (hp), (elem), (size))
#define SCALE 16384

static void _init_lpfilter(ALLowPass *lp)
{
    s32 i, temp;
    s16 fc;
    f64 ffc, fcoef;

    temp = lp->fc * SCALE;
    fc = temp >> 15;
    lp->fgain = SCALE - fc;

    lp->first = 1;
    for (i = 0; i < 8; i++)
        lp->fcvec.fccoef[i] = 0;

    lp->fcvec.fccoef[i++] = fc;
    fcoef = ffc = (f64)fc / SCALE;

    for (; i < 16; i++) {
        fcoef *= ffc;
        lp->fcvec.fccoef[i] = (s16)(fcoef * SCALE);
    }
}

void func_80059614(ALFx *r, ALSynConfig *c, ALHeap *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALFilter *f = (ALFilter *)r;
    ALDelay *d;

    func_8005B670(f, 0, func_8005BE24, 5);
    f->handler = func_8005BAB0;
    r->paramHdl = (ALSetFXParam)func_8005BE38;

    switch (c->fxType) {
    case 1: param = D_80077C50; break;
    case 2: param = D_80077CB8; break;
    case 5: param = D_80077D40; break;
    case 3: param = D_80077D68; break;
    case 4: param = D_80077D90; break;
    case 6: param = c->params; break;
    default: param = D_80077DB8; break;
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
            d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = alHeapAlloc(hp, 1, sizeof(ALResampler));
            d->rs->state = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
            d->lp->fstate = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
            d->lp->fc = param[j++];
            _init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}
