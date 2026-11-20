typedef struct {
    u8 pad[0x166]; s16 unk_166; u8 pad1[0x190 - 0x168];
    s32 unk_190, unk_194, unk_198, unk_19C;
    u8 pad2[0x11A0 - 0x1A0];
    u16 unk_11A0; s16 unk_11A2; u32 unk_11A4; s32 unk_11A8;
    u8 unk_11AC[0x18]; u8 unk_11C4[4];
    s32 unk_11C8, unk_11CC, unk_11D0, unk_11D4, unk_11D8;
    u8 unk_11DC, unk_11DD, unk_11DE;
} State;
void func_80060D80(State *, s32);
void func_800585B0(void *, void *, s32);
void func_8006ADB0(State *s, s32 a, s32 b, s32 c, s32 d) {
    func_80060D80(s, 0x190);
    s->unk_166 = 0x28;
    s->unk_190 = d;
    s->unk_194 = a;
    s->unk_198 = b;
    s->unk_19C = c;
    s->unk_11A8 = 0;
    func_800585B0(s->unk_11AC, s->unk_11C4, 1);
    if (a == 0 || d == 0) s->unk_11DD = 0; else s->unk_11DD = 1;
    s->unk_11DC = 0;
    s->unk_11A2 = 0;
    s->unk_11A0 = s->unk_11A4 = (u16)s->unk_11A2;
    s->unk_11C8 = 0;
    s->unk_11D0 = 0;
    s->unk_11CC = 0;
    s->unk_11D8 = 0;
    s->unk_11D4 = 0;
    s->unk_11DE = 2;
}
