#include "common.h"

typedef struct Obj {
    u8 pad[0x1AC];
    f32 unk_1AC;
    f32 unk_1B0;
    f32 unk_1B4;
} Obj;
extern u16 D_80107CBA;

/* Normalises a horizontal vector from obj and sets a 0..128 magnitude level; handles button input. */
void func_800394FC(Obj *arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f64 sp28;

    sp10 = arg0->unk_1AC;
    sp14 = 0;
    sp18 = arg0->unk_1B4;
    sp20 = sqrtf((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
    if (sp20 >= 1e-07f) {
        sp28 = D_8007B000 / ((f64)sp20);
        sp10 = (f32)(((f64)sp10) * sp28);
        sp14 = (f32)(((f64)sp14) * sp28);
        sp18 = (f32)(((f64)sp18) * sp28);
    } else {
    }
    (*(u32 (*))&D_8012F138) = sp20 * D_8007B008;
    if ((*(u32 (*))&D_8012F138) >= 129) {
        (*(u32 (*))&D_8012F138) = 128;
    }
    D_80140F48 = D_8012F13B;
    ((void (*)(s32, u32))func_8004E990)(D_800A7F2C, (*(u32 (*))&D_8012F138));
    if (((D_80107CBA & 0xC00F) & 0xFFFF) != 0) {
        func_800375A8(arg0, 15);
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
