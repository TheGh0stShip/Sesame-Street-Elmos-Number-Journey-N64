#include "common.h"


/* calls 8004E9E0, then latches arg into D_800A056C if it is unset */
void MusPtrBankInitialize(s32 arg0) {
    ((void (*)(void))func_80050350)();
    if (D_800A056C == 0) {
        D_800A056C = arg0;
    }
}
