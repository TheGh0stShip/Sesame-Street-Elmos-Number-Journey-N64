#include "common.h"

/* _pullSubFrame (n_audio n_env.c): envelope mixer command list for one fixed-size subframe */

typedef union { Acwords words; s64 force_align; } Acmd;

typedef struct {
    u8 unk00[0x54];
    void *em_state;     /* 0x54 */
    s16 em_pan;         /* 0x58 */
    s16 em_volume;      /* 0x5A */
    s16 em_cvolL;       /* 0x5C */
    s16 em_cvolR;       /* 0x5E */
    s16 em_dryamt;      /* 0x60 */
    s16 em_wetamt;      /* 0x62 */
    u16 em_lratl;       /* 0x64 */
    s16 em_lratm;       /* 0x66 */
    s16 em_ltgt;        /* 0x68 */
    u16 em_rratl;       /* 0x6A */
    s16 em_rratm;       /* 0x6C */
    s16 em_rtgt;        /* 0x6E */
    s32 em_delta;       /* 0x70 */
    s32 em_segEnd;      /* 0x74 */
    s32 em_first;       /* 0x78 */
    u8 unk7C[8];
    s32 em_motion;      /* 0x84 */
} N_PVoice;    /* eqpower */

Acmd *n_alResamplePull(N_PVoice *f, s16 *outp, Acmd *p);/* n_alResamplePull */
    /* _getRate */
                                 /* osVirtualToPhysical */

#define EQPOWER_LENGTH 128
#define FIXED_SAMPLE 184
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))

#define n_aSetVolume(pkt, f, v, t, r)                                       \
    {                                                                       \
        Acmd *_a = (Acmd *)pkt;                                             \
        _a->words.w0 = (_SHIFTL(9, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(v, 0, 16)); \
        _a->words.w1 = (_SHIFTL(t, 16, 16) | _SHIFTL(r, 0, 16));            \
    }
#define n_aEnvMixer(pkt, f, t, s)                                           \
    {                                                                       \
        Acmd *_a = (Acmd *)pkt;                                             \
        _a->words.w0 = (_SHIFTL(3, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(t, 0, 16)); \
        _a->words.w1 = (u32)(s);                                            \
    }

Acmd *func_8005432C(N_PVoice *filter, s16 *inp, s16 *outp, s32 outCount, Acmd *p) {
    Acmd *ptr = p;
    N_PVoice *e = filter;

    if (e->em_motion != 1 || !outCount)
        return ptr;

    ptr = n_alResamplePull(e, inp, p);

    if (e->em_first) {
        e->em_first = 0;

        e->em_ltgt = (e->em_volume * D_80077AB0[e->em_pan]) >> 15;
        e->em_lratm = _getRate((f64)e->em_cvolL, (f64)e->em_ltgt, e->em_segEnd, &(e->em_lratl));
        e->em_rtgt = (e->em_volume * D_80077AB0[EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
        e->em_rratm = _getRate((f64)e->em_cvolR, (f64)e->em_rtgt, e->em_segEnd, &(e->em_rratl));

        n_aSetVolume(ptr++, 0, e->em_ltgt, e->em_lratm, e->em_lratl);
        n_aSetVolume(ptr++, 6, e->em_cvolL, e->em_dryamt, e->em_wetamt);
        n_aSetVolume(ptr++, 4, e->em_rtgt, e->em_rratm, e->em_rratl);
        n_aEnvMixer(ptr++, 1, e->em_cvolR, osVirtualToPhysical(e->em_state));
    } else
        n_aEnvMixer(ptr++, 0, 0, osVirtualToPhysical(e->em_state));

    *inp += FIXED_SAMPLE << 1;
    e->em_delta += FIXED_SAMPLE;

    return ptr;
}
