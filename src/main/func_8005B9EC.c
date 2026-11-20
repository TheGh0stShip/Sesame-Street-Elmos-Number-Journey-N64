/* alResampleParam: resampler filter parameter handler */
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    s32 (*setParam)(void *, s32, void *);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
} ALResampler;

s32 func_8005B9EC(void *filter, s32 paramID, void *param)
{
    ALFilter *f = (ALFilter *) filter;
    ALResampler *r = (ALResampler *) filter;
    union {
        f32 f;
        s32 i;
    } data;

    switch (paramID) {
        case 1: /* AL_FILTER_SET_SOURCE */
            f->source = (ALFilter *) param;
            break;
        case 4: /* AL_FILTER_RESET */
            r->delta = 0.0;
            r->first = 1;
            r->motion = 0;
            r->upitch = 0;
            if (f->source)
                (*f->source->setParam)(f->source, 4, 0);
            break;
        case 9: /* AL_FILTER_START */
            r->motion = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 9, 0);
            break;
        case 7: /* AL_FILTER_SET_PITCH */
            data.i = (s32) param;
            r->ratio = data.f;
            break;
        case 8: /* AL_FILTER_SET_UNITY_PITCH */
            r->upitch = 1;
            break;
        default:
            if (f->source)
                (*f->source->setParam)(f->source, paramID, param);
            break;
    }
    return 0;
}
