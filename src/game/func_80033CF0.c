#include "common.h"


void func_80033CF0(s32 arg0, s32 arg1) {
    D_80115404 = arg1;
    D_8011551C = arg0;
    osSendMesg(((s32)&D_8013EEF8), 0, 1);
    D_800A7F18 = 0;
    D_800AD039++;
}
