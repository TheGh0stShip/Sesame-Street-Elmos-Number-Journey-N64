/* alEnvmixerParam */
typedef struct ALParam_s { struct ALParam_s *next; } ALParam;
struct ALFilter_s;
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    ALSetParam setParam;
    s32 inp, outp, type;
} ALFilter;
typedef struct {
    ALFilter filter;
    u8 pad18[2];
    s16 volume;      /* 0x1A */
    u8 pad1C[0x1C];
    s32 first;       /* 0x38 */
    ALParam *ctrlList; /* 0x3C */
    ALParam *ctrlTail; /* 0x40 */
    u8 pad44[4];
    s32 motion;      /* 0x48 */
} ALEnvMixer;

s32 func_8005AEC0(void *filter, s32 paramID, void *param) {
    ALFilter *f = (ALFilter *)filter;
    ALEnvMixer *e = (ALEnvMixer *)filter;

    switch (paramID) {
    case 3:
        if (e->ctrlTail) {
            e->ctrlTail->next = (ALParam *)param;
        } else {
            e->ctrlList = (ALParam *)param;
        }
        e->ctrlTail = (ALParam *)param;
        break;
    case 4:
        e->first = 1;
        e->motion = 0;
        e->volume = 1;
        if (f->source)
            (*f->source->setParam)(f->source, 4, param);
        break;
    case 9:
        e->motion = 1;
        if (f->source)
            (*f->source->setParam)(f->source, 9, param);
        break;
    case 1:
        f->source = (ALFilter *)param;
        break;
    default:
        if (f->source)
            (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}
