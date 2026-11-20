/* alAuxBusNew / alMainBusNew (libultra audio/drvrnew.c): init a bus filter */
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    void *setParam;
    s16 *inp;
    s16 *outp;
} ALFilter;
typedef struct {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALBus;
void func_8005B670(ALFilter *f, void *h, void *s, s32 type); /* alFilterNew */
s32 func_8005A8B0();
s32 func_8005A98C();
void func_80059CA0(ALBus *m, void *sources, s32 maxSources)
{
    func_8005B670(&m->filter, func_8005A8B0, func_8005A98C, 6);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = (ALFilter **)sources;
}
