#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
typedef union { struct { u32 w0, w1; } words; long long force; } Acmd;
typedef struct {
    s16 fc;
    s16 fgain;
    union { s16 fccoef[16]; s64 force_aligned; } fcvec; /* 0x08 */
    s16 *fstate; /* 0x28 */
    s32 first;   /* 0x2C */
} ALLowPass;
u32 func_80059460(void *); /* osVirtualToPhysical */
/* _filterBuffer (newer ABI: buffer in pole-filter w1, no setbuffer) */
Acmd *func_80055930(ALLowPass *lp, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s32 t = buff >> 8;
    {
        Acmd *_a = ptr++;
        _a->words.w0 = _SHIFTL(11, 24, 8) | _SHIFTL(32, 0, 24);
        _a->words.w1 = func_80059460(lp->fcvec.fccoef);
    }
    {
        Acmd *_a = ptr++;
        _a->words.w0 = _SHIFTL(14, 24, 8) | _SHIFTL(lp->first, 16, 8) | _SHIFTL(lp->fgain, 0, 16);
        _a->words.w1 = _SHIFTL(t, 24, 8) | _SHIFTL(func_80059460(lp->fstate), 0, 24);
    }
    lp->first = 0;
    return ptr;
}
