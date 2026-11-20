#include "common.h"

/* _saveBuffer (reverb.c): save FIXED_SAMPLE samples to a circular delay line */
typedef union { struct { u32 w0; u32 w1; } words; long long force_structure_alignment; } Acmd;
typedef struct {
    u8 unk_00[0x14];
    s16 *base;
    s16 *input;
    s32 length;
} ALFx; /* osVirtualToPhysical */

#define FIXED_SAMPLE 184
#define A_SAVEBUFF 6
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define aSaveBuffer(pkt, s, c, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_SAVEBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(s, 0, 12); \
    _a->words.w1 = (u32)(d); }

Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + FIXED_SAMPLE;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        aSaveBuffer(ptr++, buff, before_end << 1, osVirtualToPhysical(curr_ptr));
        aSaveBuffer(ptr++, buff + (before_end << 1), after_end << 1, osVirtualToPhysical(r->base));
    } else {
        aSaveBuffer(ptr++, buff, FIXED_SAMPLE << 1, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}
