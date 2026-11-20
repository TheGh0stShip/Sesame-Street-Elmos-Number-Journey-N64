/* alFxParamHdl: set a parameter of one reverb delay line */
typedef struct { s16 fc; } ALLowPass;
typedef struct {
    u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain;
    f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain;
    ALLowPass *lp; void *resampler;
} ALDelay;
typedef struct { u8 pad[0x20]; ALDelay *delay; } ALFx;
typedef struct { u8 pad[0x44]; s32 outputRate; } ALGlobals;
extern ALGlobals *D_80077F00; /* alGlobals */
void func_80059570(ALLowPass *lp); /* _init_lpfilter */

#define RANGE 2.0
#define CONVERT 173123.404906676

s32 func_8005BE38(void *filter, s32 paramID, void *param)
{
    ALFx *f = (ALFx *)filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
    case 0:
        f->delay[s].input = (u32)val & 0xFFFFFFF8;
        break;
    case 1:
        f->delay[s].output = (u32)val & 0xFFFFFFF8;
        break;
    case 3:
        f->delay[s].ffcoef = (s16)val;
        break;
    case 2:
        f->delay[s].fbcoef = (s16)val;
        break;
    case 4:
        f->delay[s].gain = (s16)val;
        break;
    case 5:
        f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) / D_80077F00->outputRate;
        break;
    case 6:
        f->delay[s].rsgain = (((f32)val) / CONVERT) * (f->delay[s].output - f->delay[s].input);
        break;
    case 7:
        if (f->delay[s].lp) {
            f->delay[s].lp->fc = (s16)val;
            func_80059570(f->delay[s].lp);
        }
        break;
    }
    return 0;
}
