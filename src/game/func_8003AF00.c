#include "common.h"
#include "game_structs.h"

extern u16 D_80107CBA;
/* movement sound/animation update: speed drives a step counter and a fading volume */
void func_8003AF00(Actor *o) {
    f32 v[3];
    f32 len;
    s32 unused1[21];
    f64 inv;
    s32 unused2[8];
    if (o->u.walker.unk_1EC != 0) {
        o->u.walker.unk_1EC += 8;
    }
    v[0] = o->u.walker.unk_1A8.l0.scale.y;
    v[1] = 0;
    v[2] = o->u.walker.unk_1A8.l1.unk_1AC.z;
    len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        v[0] = v[0] * inv;
        v[1] = v[1] * inv;
        v[2] = v[2] * inv;
    } else {
    }
    o->u.walker.unk_1F0.l1.unk_1F0 += (u8)(len + 0.5);
    o->u.walker.unk_226 = (u8)((u8)len >> 1) + 2;
    if (D_800A9148 >= 0xAB) {
        func_800375A8(o, 0x1C);
    } else if (D_800A9148 >= 0x8D) {
        (*(u32 (*))&D_8012F138) = 0;
        ((void (*)(void *, u32))func_8004E990)(D_800A7F2C, (*(u32 (*))&D_8012F138));
        o->u.walker.unk_1EF = 2;
        if (++o->u.walker.unk_1EE > 32) {
            o->u.walker.unk_1EE = 32;
        }
    } else {
        (*(u32 (*))&D_8012F138) = len * 8.0;
        if ((*(u32 (*))&D_8012F138) >= 0x80) {
            (*(u32 (*))&D_8012F138) = 0x7F;
        }
        D_80140F48 = (*(u32 (*))&D_8012F138);
        if (o->u.walker.unk_1EF == 0) {
            if ((*(u32 (*))&D_8012F138) > 0x50) {
                o->u.walker.unk_1EF = 1;
                o->u.walker.unk_1EE = 0;
            }
        } else if (o->u.walker.unk_1EF > 0) {
            if (o->u.walker.unk_1EF >= 2) {
                if (--o->u.walker.unk_1EE < 0) {
                    o->u.walker.unk_1EF = 1;
                    o->u.walker.unk_1EE = 0;
                }
                (*(u32 (*))&D_8012F138) = (*(u32 (*))&D_8012F138) + o->u.walker.unk_1EE;
            } else {
                if ((*(u32 (*))&D_8012F138) < 0x40) {
                    o->u.walker.unk_1EF = -1;
                    o->u.walker.unk_1EE = (*(u32 (*))&D_8012F138);
                    ((void (*)(void *, u32))func_8004E990)(func_8004E5A0(0x21), (*(u32 (*))&D_8012F138));
                }
            }
        } else {
            (*(u32 (*))&D_8012F138) = o->u.walker.unk_1EE;
            o->u.walker.unk_1EE -= 8;
            if (o->u.walker.unk_1EE < 0) {
                o->u.walker.unk_1EF = 0;
                (*(u32 (*))&D_8012F138) = 0;
            }
        }
        ((void (*)(void *, u32))func_8004E990)(D_800A7F2C, (*(u32 (*))&D_8012F138));
    }
    if (D_80107CBA & 0x1000) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
            return;
        }
    }
}
