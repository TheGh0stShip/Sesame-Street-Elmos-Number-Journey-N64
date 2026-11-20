/* libultra env.c _pullSubFrame: envelope mixer RSP commands for a subframe */
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
typedef s32 (*ALSetParam)(void *, s32, void *);
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
extern s16 D_80077DE0[]; /* eqpower */
#define eqpower D_80077DE0
s16 func_8005B36C(f64 vol, f64 tgt, s32 count, u16 *ratel); /* _getRate */
u32 func_80059460(void *); /* osVirtualToPhysical */

Acmd *func_8005AF94(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    ALFilter *source = e->filter.source;

    if (e->motion != AL_PLAYING || !outCount)
        return ptr;

    ptr = (*source->handler)(source, inp, outCount, sampleOffset, p);

    aSetBuffer(ptr++, A_MAIN, *inp, AL_MAIN_L_OUT + *outp, outCount << 1);
    aSetBuffer(ptr++, A_AUX, AL_MAIN_R_OUT + *outp, AL_AUX_L_OUT + *outp, AL_AUX_R_OUT + *outp);

    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * eqpower[e->pan]) >> 15;
        e->lratm = func_8005B36C((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &(e->lratl));
        e->rtgt = (e->volume * eqpower[EQPOWER_LENGTH - e->pan - 1]) >> 15;
        e->rratm = func_8005B36C((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &(e->rratl));

        aSetVolume(ptr++, A_LEFT | A_VOL, e->cvolL, 0, 0);
        aSetVolume(ptr++, A_RIGHT | A_VOL, e->cvolR, 0, 0);
        aSetVolume(ptr++, A_LEFT | A_RATE, e->ltgt, e->lratm, e->lratl);
        aSetVolume(ptr++, A_RIGHT | A_RATE, e->rtgt, e->rratm, e->rratl);
        aSetVolume(ptr++, A_AUX, e->dryamt, 0, e->wetamt);
        aEnvMixer(ptr++, A_INIT | A_AUX, func_80059460(e->state));
    } else
        aEnvMixer(ptr++, A_CONTINUE | A_AUX, func_80059460(e->state));

    *inp += (outCount << 1);
    e->delta += outCount;
    return ptr;
}
