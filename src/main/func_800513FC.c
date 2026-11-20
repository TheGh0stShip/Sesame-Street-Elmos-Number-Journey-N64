typedef struct { u8 b[6]; } E6;
typedef struct { u8 pad[0x1C]; E6 *unk_1C; } Bank;
typedef struct { u8 pad[0x74]; Bank *unk_74; u8 pad2[0xC]; E6 *unk_84; } S;
/* reads 1-2 byte index, points unk_84 at entry of 6-byte table */
u8 *func_800513FC(S *p, u8 *ptr) {
    s32 v = *ptr++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *ptr++;
    }
    p->unk_84 = &p->unk_74->unk_1C[v];
    return ptr;
}
