/* convert microseconds to output samples, rounded and aligned to 16 */
typedef struct { char pad0[0x44]; s32 outputRate; } S;
s32 func_8005CE3C(S *p, s32 us)
{
    return (s32)(f32)(((f32)us * (f32)p->outputRate) / 1000000.0 + 0.5) & ~0xF;
}
