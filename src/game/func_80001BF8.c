#include "common.h"


void func_80001BF8(s32 arg0) {
    s32 sp0;

    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0xE9000000;
    ((struct gd_4_s32 *)sp0)->v = 0;
    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0xB8000000;
    ((struct gd_4_s32 *)sp0)->v = 0;
}
