/* pushes an 8-byte message onto a ring buffer; returns 0 when full */
typedef struct {
    u8 type;
    s32 arg;
} Msg;
extern s32 D_800A0584; /* head */
extern s32 D_800A0588; /* tail */
extern s32 D_800A058C; /* size */
extern Msg *D_800A0590; /* buffer */
void *func_800529BC(void *dst, const void *src, s32 n); /* memcpy */
s32 func_8004F044(Msg *m) {
    s32 next = (D_800A0588 + 1) % D_800A058C;
    if (next == D_800A0584) {
        return 0;
    }
    func_800529BC(&D_800A0590[D_800A0588], m, sizeof(Msg));
    D_800A0588 = next;
    return 1;
}
