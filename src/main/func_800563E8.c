/* __allocParam: pop an ALParam off the synth driver's free list */
typedef struct ALParam_s { struct ALParam_s *next; } ALParam;
typedef struct {
    u8 unk_00[0x2C];
    ALParam *paramList;
} ALSynth;
typedef struct { ALSynth drvr; } ALGlobals;
extern ALGlobals *D_80077BC4; /* alGlobals */
ALParam *func_8005CD1C(void)
{
    ALParam *update = 0;
    ALSynth *drvr = &D_80077BC4->drvr;

    if (drvr->paramList) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = 0;
    }
    return update;
}
