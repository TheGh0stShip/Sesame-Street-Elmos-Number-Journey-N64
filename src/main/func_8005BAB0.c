/* alFxPull (libultra audio reverb.c) */
typedef struct { u32 w0; u32 w1; } Acmd;
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; _a->w0 = (_SHIFTL(8, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); _a->w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aMix(pkt, f, g, i, o) { Acmd *_a = (Acmd *)pkt; _a->w0 = (_SHIFTL(12, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16)); _a->w1 = _SHIFTL(i,16, 16) | _SHIFTL(o, 0, 16); }
#define aClearBuffer(pkt, d, c) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(2, 24, 8) | _SHIFTL(d, 0, 24); _a->w1 = (unsigned int)(c); }
#define aDMEMMove(pkt, i, o, c) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(10, 24, 8) | _SHIFTL(i, 0, 24); _a->w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define SWAP(in, out) { s16 t = out; out = in; in = t; }

struct ALFilter_s;
typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef struct ALFilter_s { struct ALFilter_s *source; ALCmdHandler handler; void *setParam; s16 inp; s16 outp; s32 type; } ALFilter;
typedef struct ALLowPass_s ALLowPass;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; ALLowPass *lp; void *rs; } ALDelay;
typedef struct { ALFilter filter; s16 *base; s16 *input; u32 length; ALDelay *delay; u8 section_count; } ALFx;

Acmd *func_8005C3B0(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p); /* _saveBuffer */
Acmd *func_8005C240(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p); /* _loadBuffer */
Acmd *func_8005C028(ALFx *r, ALDelay *d, s32 buff, s32 count, Acmd *p); /* _loadOutputBuffer */
Acmd *func_8005C520(ALLowPass *lp, s32 buff, s32 count, Acmd *p); /* _filterBuffer */

Acmd *func_8005BAB0(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALFx *r = (ALFx *)filter;
    ALFilter *source = r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, *prev_out_ptr = 0;
    ALDelay *d;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);

    input = 0x6c0;
    output = 0x800;
    buff1 = 0;
    buff2 = 0x140;

    aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
    aMix(ptr++, 0, 0xda83, 0x6c0, input);
    aMix(ptr++, 0, 0x5a82, 0x800, input);

    ptr = func_8005C3B0(r, r->input, input, outCount, ptr);
    aClearBuffer(ptr++, output, outCount << 1);

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        } else {
            ptr = func_8005C240(r, in_ptr, buff1, outCount, ptr);
        }
        ptr = func_8005C028(r, d, buff2, outCount, ptr);

        if (d->ffcoef) {
            aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp) {
                ptr = func_8005C3B0(r, out_ptr, buff2, outCount, ptr);
            }
        }

        if (d->fbcoef) {
            aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = func_8005C3B0(r, in_ptr, buff1, outCount, ptr);
        }

        if (d->lp)
            ptr = func_8005C520(d->lp, buff2, outCount, ptr);

        if (!d->rs)
            ptr = func_8005C3B0(r, out_ptr, buff2, outCount, ptr);

        if (d->gain)
            aMix(ptr++, 0, (u16)d->gain, buff2, output);

        prev_out_ptr = &r->input[d->output];
    }

    r->input += outCount;
    if (r->input > &r->base[r->length])
        r->input -= r->length;

    aDMEMMove(ptr++, output, 0x6c0, outCount << 1);

    return ptr;
}
