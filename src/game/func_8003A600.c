#include "common.h"
#include "game_structs.h"

extern u16 D_80107CBA;

/* Normalises a horizontal vector from obj and derives speed-based values; handles button input. */
void func_8003A600(Actor *arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    u32 sp24;
    f64 sp28;

    if (arg0->u.walker.turning == 0) {
        func_800375A8(arg0, 22);
        return;
    }
    if (D_800A9148 >= 161) {
        func_800375A8(arg0, 24);
        arg0->u.walker.unk_1E4 = 22.0f;
        return;
    }
    sp10 = arg0->u.walker.unk_1A8.l0.scale.y;
    sp14 = 0;
    sp18 = arg0->u.walker.unk_1A8.l1.unk_1AC.z;
    sp20 = sqrtf((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
    if (sp20 >= 1e-07f) {
        sp28 = D_8007B040 / ((f64)sp20);
        sp10 = (f32)(((f64)sp10) * sp28);
        sp14 = (f32)(((f64)sp14) * sp28);
        sp18 = (f32)(((f64)sp18) * sp28);
    } else {
    }
    arg0->u.walker.unk_226 = (u8)(u32)sp20 / 4 + 8;
    D_800A93A3 = (s32)sp20;
    sp24 = sp20 * D_8007B048 + D_8007B050;
    if (sp24 >= 97) {
        sp24 = 96;
    }
    D_80140F48 = sp24;
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
