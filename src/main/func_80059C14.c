/* alResampleNew */
typedef s32 (*ALCmdHandler)(void *, s16 *, s32, s32, void *);
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct { s16 v[16]; } RESAMPLE_STATE;
typedef struct ALParam_s ALParam;
typedef struct ALHeap_s ALHeap;
typedef struct ALResampler_s {
    ALFilter filter;
    RESAMPLE_STATE *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 motion;
} ALResampler;
void func_8005B670(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type); /* alFilterNew */
s32 func_8005B800(void *, s16 *, s32, s32, void *);  /* alResamplePull */
s32 func_8005B9EC(void *, s32, void *);              /* alResampleParam */
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size); /* alHeapDBAlloc */
void func_80059C14(ALResampler *r, ALHeap *hp) {
    func_8005B670((ALFilter *)r, func_8005B800, func_8005B9EC, 1);
    r->state = func_8005C810(0, 0, hp, 1, sizeof(RESAMPLE_STATE));
    r->delta = 0.0;
    r->first = 1;
    r->motion = 0;
    r->ratio = 1.0;
    r->upitch = 0;
    r->ctrlList = 0;
    r->ctrlTail = 0;
}
