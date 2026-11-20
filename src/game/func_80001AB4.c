#include "common.h"


void func_80001AB4(s32 arg0) {
    s32 sp10;

    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xF7000000;
    ((struct gd_4_s32 *)sp10)->v = D_80107D10;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xF64FC3BC;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE7000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xBA001402;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0x6000000;
    ((struct gd_4_s32 *)sp10)->v = (s32)&D_80075580;
    if (D_80076800 != 0) {
        if (D_80108FB0 != 0) {
            func_80002EA8();
        }
    }
}
