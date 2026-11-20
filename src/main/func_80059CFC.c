/* initialises a type-7 audio filter (alFilterNew) and its three state words */
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
typedef struct {
    ALFilter filter;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
} S;
void func_8005B670(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type);
s32 func_8005B690(void *, s16 *, s32, s32, void *);
s32 func_8005B7D0(void *, s32, void *);
void func_80059CFC(S *f, s32 a, s32 b) {
    func_8005B670(&f->filter, func_8005B690, func_8005B7D0, 7);
    f->unk_14 = 0;
    f->unk_18 = b;
    f->unk_1C = a;
}
