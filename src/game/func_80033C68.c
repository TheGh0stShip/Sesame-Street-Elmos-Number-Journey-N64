#include "common.h"

extern s32 D_8013E7F0;

void func_80033C68(s32 arg0, s32 arg1) {
    D_80115404 = arg0;
    D_8013E7F0 = arg1;
    D_8011551C = (s32)&D_80074328;
    osSendMesg(((s32)&D_8013EEF8), 0, 1);
    D_800A7F18 = 0;
    D_800AD039++;
}
