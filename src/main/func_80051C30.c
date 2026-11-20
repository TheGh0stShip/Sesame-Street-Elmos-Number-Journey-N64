typedef struct {
    u8 pad[0x14];
    void *state;
    u8 pad18[8];
    f32 delta;
    s32 first;
} Resampler;
typedef struct {
    s16 fc;
    u8 pad2[0x28 - 2];
    void *fstate;
} LowPass;
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
    LowPass *lp;
    Resampler *rs;
} Delay;
typedef struct {
    u8 pad[0x14];
    s16 *base;
    s16 *input;
    u32 length;
    Delay *delay;
    u8 section_count;
} Fx;
typedef struct { u8 pad[0x24]; Fx *fx; } Synth34;
typedef struct { u8 pad[0x34]; Synth34 *unk_34; u8 pad38[8]; s32 outputRate; } Synth;
typedef struct { void *obj; void *state; } Prealloc;
extern Synth *D_8013C6F8;
typedef struct {
    Delay *delay;
    s16 *base;
    Prealloc pre[2];
} FxBufs;
extern FxBufs D_8013C708;
void func_80059570(LowPass *lp);

/* libultra alFxNew variant: builds reverb delay sections from a parameter list, using static buffers */
void func_80051C30(s32 *param) {
    u16 i, j;
    Fx *r;
    Delay *d;

    j = 0;
    r = D_8013C6F8->unk_34->fx;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = D_8013C708.delay;
    r->base = D_8013C708.base;
    r->input = r->base;
    for (i = 0; i < r->length; i++) {
        r->base[i] = 0;
    }
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];
        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / D_8013C6F8->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = D_8013C708.pre[0].obj;
            d->rs->state = D_8013C708.pre[0].state;
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = D_8013C708.pre[1].obj;
            d->lp->fstate = D_8013C708.pre[1].state;
            d->lp->fc = param[j++];
            func_80059570(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}
