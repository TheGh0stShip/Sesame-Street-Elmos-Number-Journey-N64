typedef struct {
    u8 unk_00[0x20]; f32 unk_20; u8 unk_24[0x68 - 0x24]; f32 unk_68;
    u8 unk_6C[0xAA - 0x6C]; u16 unk_AA; u8 unk_AC[0xB6 - 0xAC]; u8 unk_B6;
    u8 unk_B7[0xDC - 0xB7]; f32 unk_DC;
} Obj;
f32 func_80057E70(f32);
/* computes and stores unk_68 from remaining count (unk_AA - unk_B6) */
f32 func_8004FF80(Obj *o) {
    s32 d = o->unk_AA - o->unk_B6;
    if (d > 0) {
        return o->unk_68 = func_80057E70(d * o->unk_DC) * o->unk_20;
    }
    return 0.0f;
}
