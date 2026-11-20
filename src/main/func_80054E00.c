/* n_audio n_alResamplePull: resample one fixed 184-sample frame of a voice */
typedef struct { u32 w0, w1; } Acmd;
typedef struct {
    u8 unk_00[0x40];
    void *rs_state;
    f32 rs_ratio;
    s32 rs_upitch;
    f32 rs_delta;
    s32 rs_first;
} N_PVoice;

extern Acmd *func_800546E0(N_PVoice *e, s16 *outp, s32 outCount, Acmd *p); /* n_alAdpcmPull */
extern u32 func_80059460(void *); /* osVirtualToPhysical */

#define FIXED_SAMPLE 184
#define MAX_RATIO 1.99996
#define UNITY_PITCH 0x8000
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))

#define aDMEMMove(pkt, i, o, c)                                      \
    {                                                                \
        Acmd *_a = (Acmd *)pkt;                                      \
        _a->w0 = _SHIFTL(0x0A, 24, 8) | _SHIFTL(i, 0, 24);           \
        _a->w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);             \
    }
#define n_aResample(pkt, s, f, p, i, o)                              \
    {                                                                \
        Acmd *_a = (Acmd *)pkt;                                      \
        _a->w0 = (_SHIFTL(0x05, 24, 8) | _SHIFTL(s, 0, 24));         \
        _a->w1 = (_SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) |           \
                  _SHIFTL(i, 2, 12) | _SHIFTL(o, 0, 2));             \
    }

Acmd *func_80054E00(N_PVoice *e, s16 *outp, Acmd *p) {
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    s32 incr;
    f32 finCount;

    inp = 0x170;
    if (e->rs_upitch) {
        ptr = func_800546E0(e, &inp, FIXED_SAMPLE, p);
        aDMEMMove(ptr++, inp, *outp, FIXED_SAMPLE << 1);
    } else {
        if (e->rs_ratio > MAX_RATIO) {
            e->rs_ratio = MAX_RATIO;
        }
        e->rs_ratio = (s32)(e->rs_ratio * UNITY_PITCH);
        e->rs_ratio = e->rs_ratio / UNITY_PITCH;
        finCount = e->rs_delta + (e->rs_ratio * (f32)FIXED_SAMPLE);
        inCount = (s32)finCount;
        e->rs_delta = finCount - (f32)inCount;
        ptr = func_800546E0(e, &inp, inCount, p);
        incr = (s32)(e->rs_ratio * UNITY_PITCH);
        n_aResample(ptr++, func_80059460(e->rs_state), e->rs_first, incr, inp, 0);
        e->rs_first = 0;
    }
    return ptr;
}
