#include "common.h"

/* alFilterNew: initialise an ALFilter */
typedef s32 (*ALCmdHandler)(void *, s16 *, s32, s32, s16 *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
void func_8005B670(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type)
{
    f->source = 0;
    f->handler = h;
    f->setParam = s;
    f->inp = 0;
    f->outp = 0;
    f->type = type;
}
