#include "common.h"

typedef struct { unsigned int w0; unsigned int w1; } Awords;
typedef union { Awords words; long long force_union_align; } Acmd;
typedef struct {
    s32 input; s32 output; s16 ffcoef; s16 fbcoef; s16 gain; u8 unk_0E[0x20 - 0xE];
    void *lp; void *rs;
} Delay;
typedef struct {
    u8 unk_00[0x14]; s16 *base; s16 *input; s32 length; Delay *delay; u8 section_count;
} Fx;
typedef struct { u8 unk_00[0x20]; Fx *fx; } FxHolder;
typedef struct { u8 unk_00[0x34]; FxHolder *unk_34; } Synth;
extern Synth *D_80077BC4;
Acmd *func_80053750(void);
Acmd *_n_saveBuffer(Fx *r, s16 *buf, s32 dmem, Acmd *p);
Acmd *_n_loadBuffer(Fx *r, s16 *buf, s32 dmem, s32 count, Acmd *p);
Acmd *_n_loadOutputBuffer(Fx *r, Delay *d, s32 dmem, Acmd *p);
Acmd *_n_filterBuffer(void *lp, s32 dmem, Acmd *p);

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define CMD(pkt, a, b) { Acmd *_a = (Acmd *)(pkt); _a->words.w0 = (a); _a->words.w1 = (b); }
#define MIX(pkt, f, g, i, o) { Acmd *_a = (Acmd *)(pkt); \
    _a->words.w0 = _SHIFTL(12, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16); \
    _a->words.w1 = _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16); }

/* reverb effect pull: builds the audio command list for the aux bus (libultra alFxPull) */
Acmd *func_80054FE0(void) {
    Fx *r = D_80077BC4->unk_34->fx;
    Acmd *ptr;
    s16 i;
    s16 buff1 = 0;
    s16 buff2 = 0x170;
    s16 tmp;
    s16 *in_ptr;
    s16 *out_ptr;
    s16 *prev_out_ptr = 0;
    Delay *d;
    s16 auxOut = 0x930;

    i = 0;
    ptr = func_80053750();
    CMD(ptr++, 0x0C00DA83, 0x07C007C0);
    CMD(ptr++, 0x0C005A82, 0x093007C0);
    ptr = _n_saveBuffer(r, r->input, 0x7C0, ptr);
    CMD(ptr++, 0x02000930, 0x170);
    for (; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];
        if (in_ptr == prev_out_ptr) {
            tmp = buff2;
            buff2 = buff1;
            buff1 = tmp;
        } else {
            ptr = _n_loadBuffer(r, in_ptr, buff1, 0xB8, ptr);
        }
        ptr = _n_loadOutputBuffer(r, d, buff2, ptr);
        if (d->ffcoef) {
            MIX(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp) {
                ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
            }
        }
        if (d->fbcoef) {
            MIX(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = _n_saveBuffer(r, in_ptr, buff1, ptr);
        }
        if (d->lp) {
            ptr = _n_filterBuffer(d->lp, buff2, ptr);
        }
        if (!d->rs) {
            ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
        }
        if (d->gain) {
            MIX(ptr++, 0, (u16)d->gain, buff2, auxOut);
        }
        prev_out_ptr = &r->input[d->output];
    }
    r->input += 0xB8;
    if (r->input > &r->base[r->length]) {
        r->input -= r->length;
    }
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = _SHIFTL(10, 24, 8) | _SHIFTL(auxOut, 0, 24); _a->words.w1 = _SHIFTL(0x7C0, 16, 16) | _SHIFTL(0x170, 0, 16); }
    return ptr;
}
