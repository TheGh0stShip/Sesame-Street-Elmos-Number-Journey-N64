typedef struct {
    u8 unk_0[0x1A];
} Elem;

void func_8006C700(s16 a, s16 b, Elem *e, void *out);
void func_8006C7F4(void *ctx, s16 a, s16 b, void *m);
void func_8006B290(void *ctx, s32 arg1, void *ctx2, s32 arg3);

/* runs four transform steps over parallel s16 arrays, then finishes with func_8006B290 */
void func_8006B488(void *ctx, s32 arg1, s16 *a2, s16 *a3, s16 *a4, s16 *a5, Elem *e, s32 arg7) {
    u8 buf[0x54];
    s32 i;

    for (i = 0; i < 4; i++) {
        func_8006C700(*a5, *a4, e, buf);
        func_8006C7F4(ctx, *a2, *a3, buf);
        a5++; a3++; a2++; a4++; e++;
    }
    func_8006B290(ctx, arg1, ctx, arg7);
}
