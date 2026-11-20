/* alSaveNew: init the save filter (AL_SAVE) */
typedef s32 (*ALCmdHandler)(void *, s16 *, s32, s32, s16 *);
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
    s32 dramout;
    s32 first;
} ALSave;
extern void func_8005B670(ALFilter *, ALCmdHandler, ALSetParam, s32); /* alFilterNew */
extern s32 func_8005C650(void *, s16 *, s32, s32, s16 *);            /* alSavePull */
extern s32 func_8005C6F8(void *, s32, void *);                       /* alSaveParam */

void func_80059D58(ALSave *f)
{
    func_8005B670((ALFilter *)f, func_8005C650, func_8005C6F8, 3);
    f->dramout = 0;
    f->first = 1;
}
