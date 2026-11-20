#include "common.h"

/* libultra env.c alEnvmixerPull: process envelope mixer parameter updates and build RSP commands */
typedef union {
    struct { u32 w0; u32 w1; } words;
    long long int force_union_align;
} Acmd;
typedef short ENVMIX_STATE[40];
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_INIT 0x01
#define A_CONTINUE 0x00
#define A_LEFT 0x02
#define A_RIGHT 0x00
#define A_VOL 0x04
#define A_RATE 0x00
#define A_AUX 0x08
#define A_MAIN 0x00
#define A_ENVMIXER 3
#define A_SETBUFF 8
#define A_SETVOL 9
#define aEnvMixer(pkt, f, s) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_ENVMIXER, 24, 8) | _SHIFTL(f, 16, 8); \
    _a->words.w1 = (unsigned int)(s); }
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aSetVolume(pkt, f, v, t, r) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETVOL, 24, 8) | _SHIFTL(f, 16, 16) | _SHIFTL(v, 0, 16)); \
    _a->words.w1 = _SHIFTL(t, 16, 16) | _SHIFTL(r, 0, 16); }

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    union { f32 f; s32 i; } data;
    union { f32 f; s32 i; } moredata;
    union { f32 f; s32 i; } stillmoredata;
    union { f32 f; s32 i; } yetstillmoredata;
} ALParam;
typedef struct ALEnvMixer_s {
    ALFilter filter;
    ENVMIX_STATE *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;
#define AL_PLAYING 1
#define AL_STOPPED 0
#define EQPOWER_LENGTH 128
#define AL_MAIN_L_OUT 1088
#define AL_MAIN_R_OUT 1408
#define AL_AUX_L_OUT 1728
#define AL_AUX_R_OUT 2048
 /* eqpower */
#define eqpower D_80077DE0
s16 func_8005B36C(f64 vol, f64 tgt, s32 count, u16 *ratel);/* _getRate */
 /* osVirtualToPhysical */
enum {
    AL_FILTER_FREE_VOICE, AL_FILTER_SET_SOURCE, AL_FILTER_ADD_SOURCE, AL_FILTER_ADD_UPDATE,
    AL_FILTER_RESET, AL_FILTER_SET_WAVETABLE, AL_FILTER_SET_DRAM, AL_FILTER_SET_PITCH,
    AL_FILTER_SET_UNITY_PITCH, AL_FILTER_START, AL_FILTER_SET_STATE, AL_FILTER_SET_VOLUME,
    AL_FILTER_SET_PAN, AL_FILTER_START_VOICE_ALT, AL_FILTER_START_VOICE, AL_FILTER_STOP_VOICE,
    AL_FILTER_SET_FXAMT
};
#define AL_RESAMPLER_OUT 0

typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    f32 pitch;
    s16 volume;
    ALPan pan;
    u8 fxMix;
    s32 samples;
    struct ALWaveTable_s *wave;
} ALStartParamAlt;
typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 unity;
    struct ALWaveTable_s *wave;
} ALStartParam;
typedef struct PVoice_s {
    u8 unk_00[0xD8];
    s32 offset;
} PVoice;
typedef struct {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    struct PVoice_s *pvoice;
} ALFreeParam;
typedef struct ALSynth_s { u8 unk_00[4]; } ALSynth;
typedef struct ALGlobals_s { ALSynth drvr; } ALGlobals;
extern ALGlobals *D_80077F00; /* alGlobals */
Acmd *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);/* _pullSubFrame */
 /* _getVol */
void _freePVoice(ALSynth *drvr, PVoice *pvoice); /* _freePVoice */
void func_8005CD48(ALParam *param); /* __freeParam */

Acmd *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam *thisParam;

    inp = AL_RESAMPLER_OUT;

    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = thisOffset - lastOffset;
        if (samples > outCount)
            break;

        switch (e->ctrlList->type) {
        case (AL_FILTER_START_VOICE_ALT): {
            ALStartParamAlt *param = (ALStartParamAlt *)e->ctrlList;
            ALFilter *f = (ALFilter *)e;
            s32 tmp;

            if (param->unity) {
                (*e->filter.setParam)(&e->filter, AL_FILTER_SET_UNITY_PITCH, 0);
            }
            (*e->filter.setParam)(&e->filter, AL_FILTER_SET_WAVETABLE, param->wave);
            (*e->filter.setParam)(&e->filter, AL_FILTER_START, 0);

            e->first = 1;
            e->delta = 0;
            e->segEnd = param->samples;

            tmp = ((s32)param->volume * (s32)param->volume) >> 15;
            e->volume = (s16)tmp;
            e->pan = param->pan;
            e->dryamt = eqpower[param->fxMix];
            e->wetamt = eqpower[EQPOWER_LENGTH - param->fxMix - 1];

            if (param->samples) {
                e->cvolL = 1;
                e->cvolR = 1;
            } else {
                e->cvolL = (e->volume * eqpower[e->pan]) >> 15;
                e->cvolR = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
            }

            if (f->source) {
                union {
                    f32 f;
                    s32 i;
                } data;
                data.f = param->pitch;
                (*f->source->setParam)(f->source, AL_FILTER_SET_PITCH, (void *)data.i);
            }
        } break;

        case (AL_FILTER_SET_FXAMT):
        case (AL_FILTER_SET_PAN):
        case (AL_FILTER_SET_VOLUME):
            ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);

            if (e->delta >= e->segEnd) {
                e->ltgt = (e->volume * eqpower[e->pan]) >> 15;
                e->rtgt = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
                e->delta = e->segEnd;
                e->cvolL = e->ltgt;
                e->cvolR = e->rtgt;
            } else {
                e->cvolL = func_8005B5EC(e->cvolL, e->delta, e->lratm, e->lratl);
                e->cvolR = func_8005B5EC(e->cvolR, e->delta, e->rratm, e->rratl);
            }

            if (e->cvolL == 0) e->cvolL = 1;
            if (e->cvolR == 0) e->cvolR = 1;

            if (e->ctrlList->type == AL_FILTER_SET_PAN)
                e->pan = (s16)e->ctrlList->data.i;

            if (e->ctrlList->type == AL_FILTER_SET_VOLUME) {
                e->delta = 0;
                fVol = (e->ctrlList->data.i);
                fVol = (fVol * fVol) >> 15;
                e->volume = (s16)fVol;
                e->segEnd = e->ctrlList->moredata.i;
            }

            if (e->ctrlList->type == AL_FILTER_SET_FXAMT) {
                e->dryamt = eqpower[e->ctrlList->data.i];
                e->wetamt = eqpower[EQPOWER_LENGTH - e->ctrlList->data.i - 1];
            }

            e->first = 1;
            break;

        case (AL_FILTER_START_VOICE): {
            ALStartParam *p = (ALStartParam *)e->ctrlList;

            if (p->unity) {
                (*e->filter.setParam)(&e->filter, AL_FILTER_SET_UNITY_PITCH, 0);
            }
            (*e->filter.setParam)(&e->filter, AL_FILTER_SET_WAVETABLE, p->wave);
            (*e->filter.setParam)(&e->filter, AL_FILTER_START, 0);
        } break;

        case (AL_FILTER_STOP_VOICE): {
            ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
            (*e->filter.setParam)(&e->filter, AL_FILTER_RESET, 0);
        } break;

        case (AL_FILTER_FREE_VOICE): {
            ALSynth *drvr = &D_80077F00->drvr;
            ALFreeParam *param = (ALFreeParam *)e->ctrlList;
            param->pvoice->offset = 0;
            _freePVoice(drvr, (PVoice *)param->pvoice);
        } break;

        default:
            ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
            (*e->filter.setParam)(&e->filter, e->ctrlList->type, (void *)e->ctrlList->data.i);
            break;
        }
        loutp += (samples << 1);
        outCount -= samples;

        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0)
            e->ctrlTail = 0;

        func_8005CD48(thisParam);
    }

    ptr = _pullSubFrame(e, &inp, &loutp, outCount, sampleOffset, ptr);

    if (e->delta > e->segEnd)
        e->delta = e->segEnd;

    return ptr;
}
