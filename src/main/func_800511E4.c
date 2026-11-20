typedef struct { u8 pad[0x30]; f32 unk_30; u8 pad2[0x5C]; f32 unk_90; } S;
u8 *func_800511E4(S *p, u8 *ptr) {
    s32 v = *ptr++;
    f32 f;
    if (v & 0x80) v |= 0xFFFFFF00;
    f = (f32)v / 100.0;
    p->unk_30 = p->unk_30 - p->unk_90 + f;
    p->unk_90 = f;
    return ptr;
}
