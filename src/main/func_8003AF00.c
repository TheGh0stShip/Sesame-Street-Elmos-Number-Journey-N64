typedef struct Obj { u8 pad0[0x1AC]; f32 unk_1AC; f32 pad1B0; f32 unk_1B4; u8 pad1B8[0x1EC-0x1B8]; s8 unk_1EC; u8 pad1ED; s8 unk_1EE; s8 unk_1EF; u16 unk_1F0; u8 pad1F2[0x226-0x1F2]; u16 unk_226; } Obj;
extern s16 D_800A9148;
extern u32 D_8012F138;
extern void *D_800A7F2C;
extern u8 D_80140F48;
extern u16 D_80107CBA;
extern u8 D_80107CF0;
extern u8 D_8012F150;
extern u16 D_800A7F6A;
f32 func_80056690(f32);
void func_800375A8(Obj *, s32);
void func_8004E990(void *, u32);
void *func_8004E5A0(s32);
/* movement sound/animation update: speed drives a step counter and a fading volume */
void func_8003AF00(Obj *o) {
    f32 v[3];
    f32 len;
    s32 unused1[21];
    f64 inv;
    s32 unused2[8];
    if (o->unk_1EC != 0) {
        o->unk_1EC += 8;
    }
    v[0] = o->unk_1AC;
    v[1] = 0;
    v[2] = o->unk_1B4;
    len = func_80056690(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        v[0] = v[0] * inv;
        v[1] = v[1] * inv;
        v[2] = v[2] * inv;
    } else {
    }
    o->unk_1F0 += (u8)(len + 0.5);
    o->unk_226 = (u8)((u8)len >> 1) + 2;
    if (D_800A9148 >= 0xAB) {
        func_800375A8(o, 0x1C);
    } else if (D_800A9148 >= 0x8D) {
        D_8012F138 = 0;
        func_8004E990(D_800A7F2C, D_8012F138);
        o->unk_1EF = 2;
        if (++o->unk_1EE > 32) {
            o->unk_1EE = 32;
        }
    } else {
        D_8012F138 = len * 8.0;
        if (D_8012F138 >= 0x80) {
            D_8012F138 = 0x7F;
        }
        D_80140F48 = D_8012F138;
        if (o->unk_1EF == 0) {
            if (D_8012F138 > 0x50) {
                o->unk_1EF = 1;
                o->unk_1EE = 0;
            }
        } else if (o->unk_1EF > 0) {
            if (o->unk_1EF >= 2) {
                if (--o->unk_1EE < 0) {
                    o->unk_1EF = 1;
                    o->unk_1EE = 0;
                }
                D_8012F138 = D_8012F138 + o->unk_1EE;
            } else {
                if (D_8012F138 < 0x40) {
                    o->unk_1EF = -1;
                    o->unk_1EE = D_8012F138;
                    func_8004E990(func_8004E5A0(0x21), D_8012F138);
                }
            }
        } else {
            D_8012F138 = o->unk_1EE;
            o->unk_1EE -= 8;
            if (o->unk_1EE < 0) {
                o->unk_1EF = 0;
                D_8012F138 = 0;
            }
        }
        func_8004E990(D_800A7F2C, D_8012F138);
    }
    if (D_80107CBA & 0x1000) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
            return;
        }
    }
}
