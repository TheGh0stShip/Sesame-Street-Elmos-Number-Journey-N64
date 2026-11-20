#include "common.h"
/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */



void MusSetMasterVolume(s32 arg0, s32 arg1) {
    if (arg0 & 1) {
        D_800A055C = (u16) arg1;
    }
    if (arg0 & 2) {
        D_800A055E = (u16) arg1;
    }
}
