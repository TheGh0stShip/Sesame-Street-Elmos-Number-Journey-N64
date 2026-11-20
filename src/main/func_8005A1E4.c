/* alRaw16Pull: 16-bit raw loader filter command-list builder, with loop handling */
typedef struct { unsigned int w0; unsigned int w1; } Awords;
typedef union { Awords words; long long int force_union_align; } Acmd;
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_CLEARBUFF 2
#define A_LOADBUFF 4
#define A_SETBUFF 8
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define A_LOOP 0x02
#define ADPCMFSIZE 16
#define K0_TO_PHYS(x) ((u32)(x)&0x1FFFFFFF)
#define MIN(a,b) (((a)<(b))?(a):(b))
#define aClearBuffer(pkt, d, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
    _a->words.w1 = (unsigned int)(c); }
#define aLoadBuffer(pkt, s) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_LOADBUFF, 24, 8); \
    _a->words.w1 = (unsigned int)(s); }
#define aSetBuffer(pkt, f, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aDMEMMove(pkt, i, o, c) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); }
#define aLoadADPCM(pkt, c, d) { Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24); \
    _a->words.w1 = (unsigned int) d; }

typedef short ADPCM_STATE[ADPCMFSIZE];
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef struct { s32 order; s32 npredictors; s16 book[1]; } ALADPCMBook;
typedef struct { u32 start; u32 end; u32 count; ADPCM_STATE state; } ALADPCMloop;
typedef struct { u32 start; u32 end; u32 count; } ALRawLoop;
typedef struct { ALADPCMloop *loop; ALADPCMBook *book; } ALADPCMWaveInfo;
typedef struct { ALRawLoop *loop; } ALRAWWaveInfo;
typedef struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union { ALADPCMWaveInfo adpcmWave; ALRAWWaveInfo rawWave; } waveInfo;
} ALWaveTable;
typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    s32 (*setParam)(void *, s32, void *);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    ALFilter filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    ALWaveTable *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
} ALLoadFilter;

Acmd *func_8005A1E4(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) 
{
    Acmd        *ptr = p;
    s32         nbytes;
    s32         dramLoc;
    s32         dramAlign;
    s32         dmemAlign;
    s32         overFlow;
    s32         startZero;
    s32         nSam;
    s32         op;
    
    ALLoadFilter *f = (ALLoadFilter *)filter;
    ALFilter *a = (ALFilter *) filter;

    if (outCount == 0)
        return ptr;
    
    if ((outCount + f->sample > f->loop.end) && (f->loop.count != 0)){

        nSam = f->loop.end - f->sample;
        nbytes = nSam<<1;
        if (nSam > 0){
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else 
            dramAlign = 0; 
            
        /*
         * Fix up output pointer to allow for dram alignment
         */
        *outp += dramAlign;
        
        f->memin = (s32) f->table->base + (f->loop.start<<1);
        f->sample = f->loop.start;
        op = *outp;
        
        while (outCount > nSam){

            op += (nSam<<1);
            outCount -= nSam;
            /*
             * -1 is loop forever
             */
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            
            /*
             * What to compute.
             */
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            nbytes = nSam<<1;
        
            /*
             * Do the next section, same as last.
             */
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            if (op & 0x7)
                dmemAlign = 8 - (op & 0x7);
            else
                dmemAlign = 0;

            aSetBuffer(ptr++, 0, op + dmemAlign, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);

            /*
             * Merge the two sections in DMEM.
             */
            if (dramAlign || dmemAlign)
                aDMEMMove(ptr++, op+dramAlign+dmemAlign, op, nSam<<1);
            
        }
        
        f->sample += outCount;
        f->memin += (outCount<<1);
        
        return ptr;
    }

    /*
     * The unlooped case, which is executed most of the time
     *
     * overFlow is the number of bytes past the end
     * of the bitstream I try to generate
     */

    nbytes = outCount<<1;
    overFlow = f->memin + nbytes - ((s32) f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    if (overFlow > nbytes)
        overFlow = nbytes;
    
    if (overFlow < nbytes){
        if (outCount > 0){
            nbytes -= overFlow;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else      
            dramAlign = 0; 
        *outp += dramAlign;

        f->sample += outCount;
        f->memin += outCount<<1;    
    } else {        
        f->memin += outCount<<1;    
    }

    /*
     * Put zeros in if necessary
     */
    if (overFlow){
        startZero = (outCount<<1) - overFlow;
        if (startZero < 0)
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, overFlow);
    }
    return ptr;
}
