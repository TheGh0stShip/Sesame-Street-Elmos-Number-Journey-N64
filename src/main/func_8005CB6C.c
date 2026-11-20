/* libultra audio alAudioFrame */
typedef struct { u32 w0, w1; } Acmd;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    s32 (*handler)(void *);
    s32 callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    Acmd *(*handler)(void *, s16 *, s32, s32, Acmd *);
    s32 (*setParam)(void *, s32, void *);
    s32 inp, outp, type;
} ALFilter;
typedef struct {
    ALPlayer *head;
    u8 unk_04[0x18];
    s32 paramSamples;
    s32 curSamples;
    u8 unk_24[0x14];
    ALFilter *outputFilter;
    u8 unk_3C[0xC];
    s32 maxOutSamples;
} ALSynth;
typedef struct { ALSynth drvr; } ALGlobals;

extern ALGlobals *D_80077F00; /* alGlobals */
extern s32 func_8005CE88(ALSynth *, ALPlayer **); /* __nextSampleTime */
extern s32 func_8005CDF4(ALSynth *, s32); /* _timeToSamplesNoRound */
extern void func_8005CD60(ALSynth *); /* _collectPVoices */

#define MIN(a, b) ((a) < (b) ? (a) : (b))

Acmd *func_8005CB6C(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    ALFilter *output;
    ALSynth *drvr = &D_80077F00->drvr;
    s16 tmp = 0;
    s32 nOut;
    Acmd *cmdlEnd = cmdList;
    s16 *lOutBuf = outBuf;
    Acmd *cmdPtr;

    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }

    for (drvr->paramSamples = func_8005CE88(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = func_8005CE88(drvr, &client)) {
        drvr->paramSamples &= ~(16 - 1);
        client->samplesLeft += func_8005CDF4(drvr, (*client->handler)(client));
    }

    drvr->paramSamples &= ~(16 - 1);

    for (cmdPtr = cmdList; outLen > 0; outLen -= nOut) {
        nOut = MIN(drvr->maxOutSamples, outLen);
        cmdPtr = cmdlEnd;
        cmdPtr->w0 = 0x07000000; /* aSegment(cmdPtr++, 0, 0) */
        cmdPtr->w1 = 0;
        cmdPtr++;
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);
        drvr->curSamples += nOut;
        lOutBuf += nOut << 1;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);
    func_8005CD60(drvr);
    return cmdlEnd;
}
