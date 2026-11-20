/* alEnvmixerNew (libultra 2.0F audio/drvrnew.c) */
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct { s16 s[0x28]; } ENVMIX_STATE; /* 0x50 bytes */
typedef struct ALEnvMixer_s {
    ALFilter filter;
    ENVMIX_STATE *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;
void func_8005B670(ALFilter *f, void *h, void *s, s32 type);   /* alFilterNew */
void *func_8005C810(u8 *file, s32 line, void *hp, s32 num, s32 size); /* alHeapDBAlloc */
extern char D_8005A9C0[];  /* alEnvmixerPull */
extern char D_8005AEC0[];  /* alEnvmixerParam */
void func_80059AAC(ALEnvMixer *e, void *hp)
{
    func_8005B670((ALFilter *)e, D_8005A9C0, D_8005AEC0, 4);
    e->state = func_8005C810(0, 0, hp, 1, sizeof(ENVMIX_STATE));
    e->first = 1;
    e->motion = 0;
    e->volume = 1;
    e->ltgt = 1;
    e->rtgt = 1;
    e->cvolL = 1;
    e->cvolR = 1;
    e->dryamt = 0;
    e->wetamt = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->delta = 0;
    e->segEnd = 0;
    e->pan = 0;
    e->ctrlList = 0;
    e->ctrlTail = 0;
    e->sources = 0;
}
