typedef struct { u8 pad[0xAC]; s16 unk_AC; } S;
u8 *func_80051184(S *p, u8 *ptr) {
    s32 v = *ptr++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *ptr++;
    }
    p->unk_AC = v;
    return ptr;
}
