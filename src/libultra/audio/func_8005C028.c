#include "common.h"

/* _loadOutputBuffer (reverb, 2.0I form): load a delay line's output, resampled when chorus-modulated */
typedef union { struct { u32 w0; u32 w1; } words; long long force_structure_alignment; } Acmd;
typedef struct { u8 pad[0x14]; s16 *state; s32 unk_18; s32 unk_1C; f32 delta; s32 first; } ALResampler;
typedef struct {
    u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain;
    f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain;
    void *lp; ALResampler *rs;
} ALDelay;
typedef struct { u8 pad[0x18]; s16 *input; } ALFx;

f32 _doModFunc(ALDelay *d, s32 count); /* _doModFunc */
Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *cmdptr);/* _loadBuffer */
 /* osVirtualToPhysical */

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define aSetBuffer(pkt, f, i, o, c) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(8, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aResample(pkt, f, p, s) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(5, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(p, 0, 16)); \
    _a->words.w1 = (unsigned int)(s); }

#define AL_TEMP_1 0x280
#define MAX_RATIO 32768.0f
#define UNITY_PITCH 0x8000

Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *cmdptr)
{
    Acmd *ptr = cmdptr;
    s32 ratio, count;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;

    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc(d, incount);
        delta /= length;
        delta = (s32)(delta * MAX_RATIO);
        delta = delta / MAX_RATIO;
        fratio = 1.0 - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        out_ptr -= ramalign;
        ptr = _loadBuffer(r, out_ptr, AL_TEMP_1, count + ramalign, ptr);
        ratio = (s32)(fratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, AL_TEMP_1 + (ramalign << 1), buff, incount << 1);
        aResample(ptr++, d->rs->first, ratio, osVirtualToPhysical(d->rs->state));
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = _loadBuffer(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}
