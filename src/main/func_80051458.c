typedef struct { u8 pad[0x34]; s32 unk_34; s32 unk_38; u8 pad3c[0x44]; s32 unk_80; u8 pad84[4]; s32 unk_88; s32 unk_8C; u8 pad90[0x12]; s16 unk_A2; s16 unk_A4; } S;
s32 func_80051458(S *p, u8 *ptr) {
    s32 a, b;
    a = *ptr++ << 8; a += *ptr++;
    b = *ptr++ << 8; b += *ptr++;
    p->unk_A2 = 1;
    p->unk_38 = p->unk_8C + b;
    b = *ptr++ << 8; b += *ptr++;
    p->unk_A4 = 1;
    p->unk_34 = p->unk_88 + b;
    return p->unk_80 + a;
}
