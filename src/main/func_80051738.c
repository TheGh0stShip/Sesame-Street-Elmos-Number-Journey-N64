/* parses a note event (skipping an optional extra byte when bit 7 is set) and calls a hook in mode 2 */
typedef struct { u32 unk_0; u8 pad[0x40]; s32 unk_44; } S;
extern void (*D_800A0580)(s32, u8);
u8 *func_80051738(S *s, u8 *p) {
    u8 a;
    u8 b;
    a = *p++; b = *p++; if (b & 0x80) { b = *p++; }
    if ((s->unk_0 & 3) == 2 && D_800A0580 != NULL) {
        D_800A0580(s->unk_44, a);
    }
    return p;
}
