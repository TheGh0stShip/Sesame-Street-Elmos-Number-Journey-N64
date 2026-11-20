/* alSavePull */
typedef struct { u32 w0, w1; } Acmd;
typedef struct ALFilter_s ALFilter;
typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
struct ALFilter_s {
    ALFilter *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
typedef struct {
    ALFilter filter;
    s32 dramout;
    s32 first;
} ALSave;
#define AL_MAIN_L_OUT 0x440
#define AL_MAIN_R_OUT 0x580
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->w0 = (_SHIFTL(8, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->w1 = (_SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16)); }
#define aInterleave(pkt, l, r) { Acmd *_a = (Acmd *)pkt; \
    _a->w0 = _SHIFTL(13, 24, 8); _a->w1 = (_SHIFTL(l, 16, 16) | _SHIFTL(r, 0, 16)); }
#define aSaveBuffer(pkt, s) { Acmd *_a = (Acmd *)pkt; \
    _a->w0 = _SHIFTL(6, 24, 8); _a->w1 = (unsigned int)(s); }

Acmd *func_8005C650(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALSave *f = (ALSave *)filter;
    ALFilter *source = f->filter.source;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);
    aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
    aInterleave(ptr++, AL_MAIN_L_OUT, AL_MAIN_R_OUT);
    aSetBuffer(ptr++, 0, 0, 0, outCount << 2);
    aSaveBuffer(ptr++, f->dramout);
    return ptr;
}
