/* reverb _loadBuffer: DMA a ring-buffer span, split at the wrap */
typedef struct { u32 w0; u32 w1; } Acmd;
typedef struct {
    u8 unk0[0x14];
    s16 *base;      /* 0x14 */
    s16 *input;     /* 0x18 */
    s32 length;     /* 0x1C */
} ALFx;
u32 func_80059460(void *p); /* osVirtualToPhysical */
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->w0 = (_SHIFTL(8, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->w1 = (_SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16)); }
#define aLoadBuffer(pkt, s) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(4, 24, 8); _a->w1 = (u32)(s); }
#define aSaveBuffer(pkt, s) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(6, 24, 8); _a->w1 = (u32)(s); }
Acmd *func_8005C240(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        aSetBuffer(ptr++, 0, buff, 0, before_end << 1);
        aLoadBuffer(ptr++, func_80059460(curr_ptr));
        aSetBuffer(ptr++, 0, buff + (before_end << 1), 0, after_end << 1);
        aLoadBuffer(ptr++, func_80059460(r->base));
    } else {
        aSetBuffer(ptr++, 0, buff, 0, count << 1);
        aLoadBuffer(ptr++, func_80059460(curr_ptr));
    }
    aSetBuffer(ptr++, 0, 0, 0, count << 1);
    return ptr;
}
