/* alResamplePull: resampler filter command-list builder */
typedef struct { unsigned int w0; unsigned int w1; } Awords;
typedef union { Awords words; long long int force_union_align; } Acmd;
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_RESAMPLE 5
#define A_SETBUFF 8
#define A_DMEMMOVE 10
#define aResample(pkt, f, p, s) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_RESAMPLE, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(p, 0, 16)); \
    _a->words.w1 = (unsigned int)(s); }
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aDMEMMove(pkt, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define UNITY_PITCH 0x8000
#define MAX_RATIO 1.99996
#define AL_DECODER_OUT 320

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    s32 (*setParam)(void *, s32, void *);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
} ALResampler;
u32 func_80059460(void *); /* osVirtualToPhysical */

Acmd *func_8005B800(void *filter, s16 *outp, s32 outCnt, s32 sampleOffset, Acmd *p)
{
    ALResampler *f = (ALResampler *)filter;
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter *source = f->filter.source;
    s32 incr;
    f32 finCount;

    inp = AL_DECODER_OUT;

    if (!outCnt)
        return ptr;

    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        aDMEMMove(ptr++, inp, *outp, outCnt<<1);
    } else {
        if (f->ratio > MAX_RATIO) f->ratio = MAX_RATIO;
        f->ratio = (s32)(f->ratio * UNITY_PITCH);
        f->ratio = f->ratio / UNITY_PITCH;
        finCount = f->delta + (f->ratio * (f32) outCnt);
        inCount = (s32) finCount;
        f->delta = finCount - (f32)inCount;
        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);
        incr = (s32)(f->ratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, inp, *outp, outCnt<<1);
        aResample(ptr++, f->first, incr, func_80059460(f->state));
        f->first = 0;
    }
    return ptr;
}
