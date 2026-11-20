/* __nextSampleTime: find the client with the earliest next sample time */
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    u8 unk_04[0xC];
    s32 samplesLeft;
} ALPlayer;
typedef struct {
    ALPlayer *head;
    u8 unk_04[0x1C];
    s32 curSamples;
} ALSynth;
s32 func_8005CE88(ALSynth *drvr, ALPlayer **client)
{
    s32 delta = 0x7fffffff;
    ALPlayer *cl;

    *client = 0;

    for (cl = drvr->head; cl != 0; cl = cl->next) {
        if ((cl->samplesLeft - drvr->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }

    return (*client)->samplesLeft;
}
