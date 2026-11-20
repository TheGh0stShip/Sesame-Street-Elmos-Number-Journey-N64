extern s32 D_8007A510;
s32 func_80061178();

s32 f(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 sp10;
    s32 pad0;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    f32 sp2C;
    f32 sp30;

    sp10 = 0;
    sp18 = (((arg1 * (*(f32 *)arg0)) + (arg2 * (*(f32 *)(arg0 + 16)))) + (arg3 * (*(f32 *)(arg0 + 32)))) + (*(f32 *)(arg0 + 48));
    sp1C = (((arg1 * (*(f32 *)(arg0 + 4))) + (arg2 * (*(f32 *)(arg0 + 20)))) + (arg3 * (*(f32 *)(arg0 + 36)))) + (*(f32 *)(arg0 + 52));
    sp20 = (((arg1 * (*(f32 *)(arg0 + 8))) + (arg2 * (*(f32 *)(arg0 + 24)))) + (arg3 * (*(f32 *)(arg0 + 40)))) + (*(f32 *)(arg0 + 56));
    sp24 = (((arg1 * (*(f32 *)(arg0 + 12))) + (arg2 * (*(f32 *)(arg0 + 28)))) + (arg3 * (*(f32 *)(arg0 + 44)))) + (*(f32 *)(arg0 + 60));
    if (sp24 == 0.0f) {
        func_80061178(((s32)&D_8007A510));
        return sp10;
    }
    if (!(sp24 == 1.0f)) {
        sp28 = sp18 / sp24;
        sp2C = sp1C / sp24;
        sp30 = sp20 / sp24;
    }
    if (sp24 >= 0.0f) {
        if (sp28 > 1.0f) {
            sp10 = sp10 | 1;
        }
        if (sp28 < -1.0f) {
            sp10 = sp10 | 2;
        }
        if (sp2C > 1.0f) {
            sp10 = sp10 | 4;
        }
        if (sp2C < -1.0f) {
            sp10 = sp10 | 8;
        }
        if (sp30 > 1.0f) {
            sp10 = sp10 | 16;
        }
        if (sp30 < -1.0f) {
            sp10 = sp10 | 32;
        }
    } else {
        if (sp28 > -1.0f) {
            sp10 = sp10 | 2;
        }
        if (sp28 < 1.0f) {
            sp10 = sp10 | 1;
        }
        if (sp2C > 1.0f) {
            sp10 = sp10 | 8;
        }
        if (sp2C < -1.0f) {
            sp10 = sp10 | 4;
        }
        sp10 = sp10 | 32;
    }
    return sp10;
}
