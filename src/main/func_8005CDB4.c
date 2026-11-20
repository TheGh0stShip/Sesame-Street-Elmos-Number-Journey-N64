/* _freePVoice: move a voice from the allocated list to the lame list */
typedef struct ALLink_s { struct ALLink_s *next; struct ALLink_s *prev; } ALLink;
typedef struct {
    u8 unk_00[0x14];
    ALLink pLameList;
} ALSynth;
typedef struct { ALLink node; } PVoice;
void func_8005C79C(ALLink *ln);                 /* alUnlink */
void func_8005C77C(ALLink *ln, ALLink *to);     /* alLink */
void func_8005CDB4(ALSynth *drvr, PVoice *pvoice)
{
    func_8005C79C((ALLink *)pvoice);
    func_8005C77C((ALLink *)pvoice, &drvr->pLameList);
}
