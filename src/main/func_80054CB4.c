/* _decodeChunk (load.c, older ABI): DMA ADPCM data and emit load/loop/decode commands */
typedef union { struct { u32 w0; u32 w1; } words; long long force_structure_alignment; } Acmd;
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef struct {
    u8 unk_00[0xC];
    void *state;
    void *lstate;
    u8 unk_14[0x28 - 0x14];
    ALDMAproc dma;
    void *dmaState;
    u8 unk_30[0x38 - 0x30];
    s32 first;
    s32 memin;
} ALLoadFilter;

#define A_ADPCM 1
#define A_LOADBUFF 4
#define A_SETLOOP 15
#define A_LOOP 2
#define K0_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define aLoadBuffer(pkt, s, c, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_LOADBUFF, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(s, 0, 12); \
    _a->words.w1 = (u32)(d); }
#define aSetLoop(pkt, a) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_SETLOOP, 24, 8); _a->words.w1 = (u32)(a); }
#define aADPCMdec(pkt, s, f, c, a, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_ADPCM, 24, 8) | _SHIFTL(s, 0, 24); \
    _a->words.w1 = _SHIFTL(f, 28, 4) | _SHIFTL(c, 16, 12) | _SHIFTL(a, 12, 4) | _SHIFTL(d, 0, 12); }

Acmd *func_80054CB4(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags)
{
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 0x7;
        nbytes += dramAlign;
        aLoadBuffer(ptr++, inp, nbytes + 8 - (nbytes & 0x7), dramLoc - dramAlign);
    } else
        dramAlign = 0;

    if (flags & A_LOOP) {
        aSetLoop(ptr++, K0_TO_PHYS(f->lstate));
    }
    aADPCMdec(ptr++, f->state, flags, tsam << 1, dramAlign, outp);
    f->first = 0;
    return ptr;
}
