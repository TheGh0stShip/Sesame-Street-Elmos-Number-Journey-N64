typedef struct { u32 w0; u32 w1; } Acmd;
typedef struct ALPlayer {
    struct ALPlayer *next;
    void *clientData;
    s32 (*handler)(void *);
    s32 callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct {
    ALPlayer *head;
    u8 unk_04[0x18];
    s32 paramSamples;
    s32 curSamples;
    u8 unk_24[0x20];
    s32 maxOutSamples;
    s16 *outBuf;
} ALSynth;
extern ALSynth *D_80077BC4;
s32 func_800564C4(s32);
Acmd *func_80056570(s32, Acmd *);
void func_8005642C(void);

/* alAudioFrame: run player handlers then pull samples into the command list */
Acmd *func_80056280(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    Acmd *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (D_80077BC4->head == NULL) {
        *cmdLen = 0;
        return cmdList;
    }
    for (client = D_80077BC4->head; client->samplesLeft - D_80077BC4->curSamples < outLen; ) {
        D_80077BC4->paramSamples = client->samplesLeft & ~0xF;
        client->samplesLeft += func_800564C4(client->handler(client));
    }
    D_80077BC4->paramSamples &= ~0xF;
    while (outLen > 0) {
        nOut = (D_80077BC4->maxOutSamples < outLen ? D_80077BC4->maxOutSamples : outLen);
        D_80077BC4->outBuf = lOutBuf;
        cmdlEnd = func_80056570(D_80077BC4->curSamples, cmdlEnd);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        D_80077BC4->curSamples += nOut;
    }
    *cmdLen = cmdlEnd - cmdList;
    func_8005642C();
    return cmdlEnd;
}
