#include "common.h"

/* n_alEnvmixerPull: process queued envmixer parameter updates in 184-sample steps, then pull the rest */
typedef union { struct { u32 w0; u32 w1; } words; long long force_structure_alignment; } Acmd;

typedef struct ALParam_s {
    struct ALParam_s *next; /* 0x00 */
    s32 delta;              /* 0x04 */
    s16 type;               /* 0x08 */
    s16 unity;              /* 0x0A */
    union { f32 f; s32 i; void *p; } data;     /* 0x0C */
    union { f32 f; s32 i; } moredata;          /* 0x10 */
} ALParam;

typedef struct {
    struct ALParam_s *next; /* 0x00 */
    s32 delta;              /* 0x04 */
    s16 type;               /* 0x08 */
    s16 unity;              /* 0x0A */
    f32 pitch;              /* 0x0C */
    s16 volume;             /* 0x10 */
    u8 pan;                 /* 0x12 */
    u8 fxMix;               /* 0x13 */
    s32 samples;            /* 0x14 */
    void *wave;             /* 0x18 */
} ALStartParamAlt;

typedef struct {
    struct ALParam_s *next; /* 0x00 */
    s32 delta;              /* 0x04 */
    s16 type;               /* 0x08 */
    s16 unity;              /* 0x0A */
    void *wave;             /* 0x0C */
} ALStartParam;

typedef struct {
    u8 pad[0x88];
    s32 offset;             /* 0x88 */
} N_PVoice;

typedef struct {
    struct ALParam_s *next; /* 0x00 */
    s32 delta;              /* 0x04 */
    s16 type;               /* 0x08 */
    s16 pad;
    N_PVoice *pvoice;       /* 0x0C */
} ALFreeParam;

typedef struct {
    u8 unk_00[0x44];
    f32 rs_ratio;   /* 0x44 */
    s32 rs_upitch;  /* 0x48 */
    u8 unk_4C[0x08];
    s16 *state;     /* 0x54 */
    s16 pan;        /* 0x58 */
    s16 volume;     /* 0x5A */
    s16 cvolL;      /* 0x5C */
    s16 cvolR;      /* 0x5E */
    s16 dryamt;     /* 0x60 */
    s16 wetamt;     /* 0x62 */
    u16 lratl;      /* 0x64 */
    s16 lratm;      /* 0x66 */
    s16 ltgt;       /* 0x68 */
    u16 rratl;      /* 0x6A */
    s16 rratm;      /* 0x6C */
    s16 rtgt;       /* 0x6E */
    s32 delta;      /* 0x70 */
    s32 segEnd;     /* 0x74 */
    s32 first;      /* 0x78 */
    ALParam *ctrlList; /* 0x7C */
    ALParam *ctrlTail; /* 0x80 */
    s32 motion;     /* 0x84 */
} ALEnvMixer; /* eqpower */
Acmd *func_8005432C(ALEnvMixer *e, s16 *inp, s16 *outp, s32 outCount, Acmd *p);/* _pullSubFrame */
/* _getVol */
 /* n_alLoadParam */
s32 n_alEnvmixerParam(ALEnvMixer *e, s32 paramID, void *param); /* n_alEnvmixerParam */
void func_80056414(ALParam *p); /* __n_freeParam */
void _n_freePVoice(N_PVoice *pv); /* _n_freePVoice */

#define EQPOWER_LENGTH 128
#define FIXED_SAMPLE 184
#define SAMPLE184(delta) (((delta) + (FIXED_SAMPLE / 2)) / FIXED_SAMPLE * FIXED_SAMPLE)

Acmd *n_alEnvmixerPull(void *filter, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam *thisParam;
    s32 outCount = FIXED_SAMPLE;

    inp = 0;

    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;

        samples = SAMPLE184(thisOffset - lastOffset);
        if (samples == 0)
            thisOffset = lastOffset;
        if (samples > outCount)
            break;

        switch (e->ctrlList->type) {
        case 13: {
            ALStartParamAlt *param = (ALStartParamAlt *)e->ctrlList;

            if (param->unity) {
                e->rs_upitch = 1;
            }
            n_alLoadParam(e, 5, param->wave);
            e->motion = 1;
            e->first = 1;
            e->delta = 0;
            e->segEnd = SAMPLE184(param->samples);
            fVol = ((s32)param->volume * (s32)param->volume) >> 15;
            e->volume = (s16)fVol;
            e->pan = param->pan;
            e->dryamt = D_80077AB0[param->fxMix];
            e->wetamt = D_80077AB0[EQPOWER_LENGTH - param->fxMix - 1];
            if (param->samples) {
                e->cvolL = 1;
                e->cvolR = 1;
            } else {
                e->cvolL = (e->volume * D_80077AB0[e->pan]) >> 15;
                e->cvolR = (e->volume * D_80077AB0[EQPOWER_LENGTH - e->pan - 1]) >> 15;
            }
            e->rs_ratio = param->pitch;
        } break;

        case 16:
        case 12:
        case 11:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            if (e->delta >= e->segEnd) {
                e->ltgt = (e->volume * D_80077AB0[e->pan]) >> 15;
                e->rtgt = (e->volume * D_80077AB0[EQPOWER_LENGTH - e->pan - 1]) >> 15;
                e->delta = e->segEnd;
                e->cvolL = e->ltgt;
                e->cvolR = e->rtgt;
            } else {
                e->cvolL = _getVol(e->cvolL, e->delta, e->lratm, e->lratl);
                e->cvolR = _getVol(e->cvolR, e->delta, e->rratm, e->rratl);
            }
            if (e->cvolL == 0) e->cvolL = 1;
            if (e->cvolR == 0) e->cvolR = 1;

            if (e->ctrlList->type == 12)
                e->pan = (s16)e->ctrlList->data.i;

            if (e->ctrlList->type == 11) {
                e->delta = 0;
                fVol = e->ctrlList->data.i;
                fVol = (fVol * fVol) >> 15;
                e->volume = (s16)fVol;
                e->segEnd = SAMPLE184(e->ctrlList->moredata.i);
            }

            if (e->ctrlList->type == 16) {
                e->dryamt = D_80077AB0[e->ctrlList->data.i];
                e->wetamt = D_80077AB0[EQPOWER_LENGTH - e->ctrlList->data.i - 1];
            }
            e->first = 1;
            break;

        case 14: {
            ALStartParam *sp = (ALStartParam *)e->ctrlList;

            if (sp->unity) {
                e->rs_upitch = 1;
            }
            n_alLoadParam(e, 5, sp->wave);
            e->motion = 1;
        } break;

        case 15:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            n_alEnvmixerParam(e, 4, 0);
            break;

        case 0: {
            ALFreeParam *param = (ALFreeParam *)e->ctrlList;
            param->pvoice->offset = 0;
            _n_freePVoice(param->pvoice);
        } break;

        case 7:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            e->rs_ratio = e->ctrlList->data.f;
            break;

        case 8:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            e->rs_upitch = 1;
            break;

        case 5:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            n_alLoadParam(e, 5, e->ctrlList->data.p);
            break;

        default:
            ptr = func_8005432C(e, &inp, &loutp, samples, ptr);
            n_alEnvmixerParam(e, e->ctrlList->type, e->ctrlList->data.p);
            break;
        }
        loutp += (samples << 1);
        outCount -= samples;

        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0)
            e->ctrlTail = 0;

        func_80056414(thisParam);
    }

    ptr = func_8005432C(e, &inp, &loutp, outCount, ptr);

    if (e->delta > e->segEnd)
        e->delta = e->segEnd;

    return ptr;
}
