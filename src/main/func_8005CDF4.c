/* converts ticks to microseconds: ticks * uspt / 1e6, rounded */
typedef struct {
    char pad[0x44];
    s32 unk_44;
} S;
s32 func_8005CDF4(S *s, s32 ticks) {
    f32 t = (f32)ticks * (f32)s->unk_44;
    return (f32)(t / 1000000.0 + 0.5);
}
