typedef struct { char pad0[0xB2]; u16 unk_B2; u16 unk_B4; } S;
u8 *func_8005106C(S *p, u8 *s)
{
    u16 hi = *s++;
    u16 lo = *s++;
    p->unk_B4 = 0;
    p->unk_B2 = (hi << 8) | lo;
    return s;
}
