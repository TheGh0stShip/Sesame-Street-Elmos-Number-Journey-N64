/* _filterBuffer (reverb.c): emits SETBUFF, LOADADPCM (coefs) and POLEF commands for a low-pass */
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_LOADADPCM 11
#define A_POLEF 14
#define A_SETBUFF 8
typedef struct { unsigned int w0; unsigned int w1; } Awords;
typedef union { Awords words; long long force_union_align; } Acmd;
typedef short POLEF_STATE[4];
typedef struct {
    s16 fc;
    s16 fgain;
    union { s16 fccoef[16]; s64 force_aligned; } fcvec;
    POLEF_STATE *fstate;
    s32 first;
} ALLowPass;
u32 func_80059460(void *addr); /* osVirtualToPhysical */
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aLoadADPCM(pkt, c, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24); \
    _a->words.w1 = (unsigned int)d; }
#define aPoleFilter(pkt, f, g, s) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_POLEF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16)); \
    _a->words.w1 = (unsigned int)(s); }
Acmd *func_8005C520(ALLowPass *lp, s32 buff, s32 count, Acmd *p) {
    Acmd *ptr = p;
    aSetBuffer(ptr++, 0, buff, buff, count << 1);
    aLoadADPCM(ptr++, 32, func_80059460(lp->fcvec.fccoef));
    aPoleFilter(ptr++, lp->first, lp->fgain, func_80059460(lp->fstate));
    lp->first = 0;
    return ptr;
}
