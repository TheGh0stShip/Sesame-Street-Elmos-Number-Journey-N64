typedef struct {
    u8 pad0[0xC]; u32 unk_0C; u8 pad10[0x30]; u32 unk_40; u8 pad44[0x10]; u32 unk_54;
    f32 unk_58; f32 unk_5C; f32 unk_60; s32 unk_64; u8 pad68[0x57];
    u8 unk_BF; u8 unk_C0; u8 unk_C1; u8 unk_C2; u8 unk_C3; u8 unk_C4; u8 unk_C5;
    u8 unk_C6; u8 unk_C7; u8 unk_C8; u8 padC9[4]; u8 unk_CD;
} Env;
/* step an envelope: stage in unk_C3, level in unk_C4 */
void func_8004FC60(Env *e) {
    s32 t;
    if ((s32)(e->unk_54 - e->unk_0C) < 0 && e->unk_C3 < 4) {
        e->unk_C3 = 4;
        e->unk_C5 = 1;
        e->unk_CD = e->unk_C4;
    }
    if (--e->unk_C5 != 0) return;
    e->unk_C5 = e->unk_BF;
    switch (e->unk_C3) {
    case 1:
        t = (((e->unk_0C - e->unk_40) >> 8) * e->unk_64) >> 10;
        if (t < e->unk_C6) {
            e->unk_C4 = e->unk_C0 + (s32)(e->unk_58 * t);
        } else {
            e->unk_C3++;
            e->unk_C4 = e->unk_C1;
        }
        break;
    case 2:
        t = ((((e->unk_0C - e->unk_40) >> 8) - e->unk_C6) * e->unk_64) >> 10;
        if (t < e->unk_C7) {
            e->unk_C4 = e->unk_C1 + (s32)(e->unk_5C * t);
        } else {
            e->unk_C3++;
            e->unk_C4 = e->unk_C2;
        }
        break;
    case 3:
        break;
    case 4:
        t = (((e->unk_0C - e->unk_54) >> 8) * e->unk_64) >> 10;
        if (t < e->unk_C8) {
            e->unk_C4 = e->unk_CD - (s32)(e->unk_60 * t * e->unk_CD);
        } else {
            e->unk_C3++;
            e->unk_C4 = 0;
        }
        break;
    }
}
