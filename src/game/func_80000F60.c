#include "common.h"


void func_80000F60(void) {
    s32 sp10[6];
    s32 sp28;
    s32 sp2C;

    D_800A914A = 3;
    osCreateMesgQueue(((s32)&sp10), ((s32)&sp28), 1);
    osSetEventMesg(5, ((s32)&sp10), 1);
    sp2C = osContInit(((s32)&sp10), ((s32)&D_8006EE70), ((s32)&D_8007BF50));
    if (sp2C != 0) {
    }
    D_8007BF78 = 5;
    osSetEventMesg(5, ((s32)&D_8010E650), ((s32)&D_8007BF78));
}
