typedef struct {
    u8 pad0[0x3C]; s32 unk_3C; s32 unk_40; u8 pad44[0x10]; s32 unk_54;
    u8 pad58[0x42]; u16 unk_9A; u8 pad9C[0x16]; u16 unk_B2; u16 unk_B4;
    u8 padB6[9]; u8 unk_BF; u8 unk_C0; u8 padC1[2]; u8 unk_C3; u8 unk_C4; u8 unk_C5;
} Obj;
void func_8004FBF0(Obj *o) {
    if (o->unk_9A != 0x7FFF) {
        if (o->unk_B2 != 0) {
            o->unk_54 = o->unk_40 + (o->unk_B2 << 8);
        } else {
            o->unk_54 = o->unk_3C - (o->unk_B4 << 8);
        }
    } else {
        o->unk_54 = o->unk_40 + 0x7FFFFFFF;
    }
    o->unk_C3 = 1;
    o->unk_C4 = o->unk_C0;
    o->unk_C5 = o->unk_BF;
}
