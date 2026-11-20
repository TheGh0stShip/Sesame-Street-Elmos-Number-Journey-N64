#include "common.h"

/* alAdpcmPull (libultra audio/load.c): ADPCM decoder pull, with loop handling */
typedef union { struct { unsigned int w0; unsigned int w1; } words; long long force_structure_alignment; } Acmd;
typedef short ADPCM_STATE[16];
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);

typedef struct { u32 start; u32 end; u32 count; } ALRawLoop;
typedef struct { s32 order; s32 npredictors; s16 book[1]; } ALADPCMBook;
typedef struct { ALRawLoop *loop; ALADPCMBook *book; } ALADPCMWaveInfo;
typedef struct ALWaveTable_s {
    u8 *base; s32 len; u8 type; u8 flags;
    union { ALADPCMWaveInfo adpcmWave; } waveInfo;
} ALWaveTable;

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct {
    ALFilter filter;            /* 0x00 */
    ADPCM_STATE *state;         /* 0x10 */
    ALRawLoop loop;             /* 0x14 */
    ALWaveTable *table;         /* 0x20 */
    s32 bookSize;               /* 0x24 */
    ALDMAproc dma;              /* 0x28 */
    void *dmaState;             /* 0x2C */
    s32 sample;                 /* 0x30 */
    s32 lastsam;                /* 0x34 */
    s32 first;                  /* 0x38 */
    s32 memin;                  /* 0x3C */
} ALLoadFilter;

#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define K0_TO_PHYS(x) ((u32)(x)&0x1FFFFFFF)
#define A_CLEARBUFF 2
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define A_LOOP 2
#define ADPCMFSIZE 16
#define ADPCMFBYTES 9
#define LFSAMPLES 4
#define AL_DECODER_IN 0
#define MIN(a,b) (((a)<(b))?(a):(b))

#define aClearBuffer(pkt, d, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); _a->words.w1 = (unsigned int)(c); }
#define aDMEMMove(pkt, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aLoadADPCM(pkt, c, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24); \
    _a->words.w1 = (unsigned int) d; }

/* _decodeChunk */
Acmd *_decodeChunk(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

Acmd *n_alAdpcmPull(void *filter, s16 *outp, s32 outCount, Acmd *p)
{
    Acmd        *ptr = p;
    s16         inp;
    s32         tsam;
    s32         nframes;
    s32         nbytes;
    s32         overFlow;
    s32         startZero;
    s32         nOver;
    s32         nSam;
    s32         op;
    s32         nLeft;
    s32         bEnd;
    s32         decoded = 0;
    s32         looped = 0;
    ALLoadFilter *f = (ALLoadFilter *)filter;

    if (outCount == 0)
        return ptr;

    inp = AL_DECODER_IN;
    aLoadADPCM(ptr++, f->bookSize,
               K0_TO_PHYS(f->table->waveInfo.adpcmWave.book->book));

    looped = (outCount + f->sample > f->loop.end) && (f->loop.count != 0);
    if (looped)
        nSam = f->loop.end - f->sample;
    else
        nSam = outCount;

    if (f->lastsam)
        nLeft = ADPCMFSIZE - f->lastsam;
    else
        nLeft = 0;
    tsam = nSam - nLeft;
    if (tsam<0) tsam = 0;

    nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
    nbytes =  nframes*ADPCMFBYTES;

    if (looped){
        ptr = _decodeChunk(ptr, f, tsam, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += (f->lastsam<<1);
        else
            *outp += (ADPCMFSIZE<<1);

        f->lastsam = f->loop.start &0xf;
        f->memin = (s32) f->table->base + ADPCMFBYTES *
            ((s32) (f->loop.start>>LFSAMPLES) + 1);
        f->sample = f->loop.start;

        bEnd = *outp;
        while (outCount > nSam){
            outCount -= nSam;
            op = (bEnd + ((nframes+1)<<(LFSAMPLES+1)) + 16) & ~0x1f;
            bEnd += (nSam<<1);
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            tsam = nSam - ADPCMFSIZE + f->lastsam;
            if (tsam<0) tsam = 0;
            nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
            nbytes =  nframes*ADPCMFBYTES;
            ptr = _decodeChunk(ptr, f, tsam, nbytes, op, inp, f->first | A_LOOP);
            aDMEMMove(ptr++, op+(f->lastsam<<1), bEnd, nSam<<1);
        }

        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES*nframes;
        return ptr;
    }

    nSam = nframes<<LFSAMPLES;
    overFlow = f->memin + nbytes - ((s32) f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    nOver = (overFlow/ADPCMFBYTES)<<LFSAMPLES;
    if (nOver > nSam + nLeft)
        nOver = nSam + nLeft;

    nbytes -= overFlow;

    if ((nOver - (nOver & 0xf))< outCount){
        decoded = 1;
        ptr = _decodeChunk(ptr, f, nSam - nOver, nbytes, *outp, inp, f->first);
        if (f->lastsam)
            *outp += (f->lastsam<<1);
        else
            *outp += (ADPCMFSIZE<<1);

        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES*nframes;
    } else {
        f->lastsam = 0;
        f->memin += ADPCMFBYTES*nframes;
    }

    if (nOver){
        f->lastsam = 0;
        if (decoded)
            startZero = (nLeft + nSam - nOver)<<1;
        else
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, nOver<<1);
    }
    return ptr;
}
