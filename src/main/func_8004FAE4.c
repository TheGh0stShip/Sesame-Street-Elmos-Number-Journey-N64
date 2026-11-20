typedef struct { u8 unk_00[0x1C]; } Chan;
typedef struct {
    u8 unk_00[0x24]; f32 unk_24; f32 unk_28; f32 unk_2C;
    u8 unk_30[0x4C - 0x30]; f32 unk_4C; f32 unk_50;
    u8 unk_54[0xAA - 0x54]; u16 unk_AA; u8 unk_AC[0xB8 - 0xAC]; u8 unk_B8;
    u8 unk_B9[2]; u8 unk_BB;
} Voice;
extern Chan *D_800A0548;
f32 func_800501BC(f32);
void func_80055DC0(Chan *, f32);
/* updates a voice's pitch (with glide) and sends the ratio to its channel */
void func_8004FAE4(Voice *v, s32 idx, f32 bend) {
    f32 p = v->unk_2C;
    if (v->unk_B8 != 0) {
        if (v->unk_B8 >= v->unk_AA) {
            p = v->unk_4C + (p - v->unk_4C) / v->unk_B8 * v->unk_AA;
        }
        v->unk_50 = p;
    }
    p += bend + v->unk_24;
    if (p != v->unk_28) {
        v->unk_28 = p;
        p = func_800501BC(p * 0.08333333333333333);
        if (p > 2.0) {
            p = 2.0f;
            v->unk_BB = 0;
        }
        func_80055DC0(&D_800A0548[idx], p);
    }
}
