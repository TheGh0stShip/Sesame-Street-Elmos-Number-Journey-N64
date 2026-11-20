typedef struct { u8 pad[4]; u8 unk_4; u8 pad5[4]; u8 unk_9; } P;
typedef struct { u8 pad[4]; u8 type; u8 pad5; u16 unk6; u8 pad8[4]; s32 unkC; u16 unk10; u8 pad12[2]; s32 unk14; } Msg;
void func_80060898(void *, s32, s32);
s32 func_80065E14(P *p) {
    Msg m;
    m.unkC = 0;
    m.unk10 = 1;
    m.unk14 = (p->unk_9 == 1) ? 0x3E9 : 0x3EA;
    m.type = p->unk_4;
    m.unk6 = 0;
    func_80060898(&m, 0x18, 1);
    return 0;
}
