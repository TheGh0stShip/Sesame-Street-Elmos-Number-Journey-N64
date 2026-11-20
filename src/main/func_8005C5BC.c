#define RANGE 2.0
typedef struct {
    u32 input, output;
    u16 ffcoef, fbcoef;
    u16 gain;
    f32 rsinc;   /* 0x10 */
    f32 rsval;   /* 0x14 */
    s32 rsdelta; /* 0x18 */
    f32 rsgain;  /* 0x1C */
} ALDelay;
/* _doModFunc */
f32 func_8005C5BC(ALDelay *d, s32 count) {
    f32 val;
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > RANGE) ? d->rsval - (RANGE * 2) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= RANGE / 2;
    return (d->rsgain * val);
}
