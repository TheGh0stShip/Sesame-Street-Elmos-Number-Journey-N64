#include "common.h"


void func_8001DAC0(s32 arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;

    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE7000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xBA001402;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xB6000000;
    ((struct gd_4_s32 *)sp10)->v = 0x9F3207;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xBA001301;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xBA000C02;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xB900031D;
    ((struct gd_4_s32 *)sp10)->v = 0xF0A4000;
    sp0 = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xBA000E02;
    ((struct gd_4_s32 *)sp10)->v = 0x8000;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xFD100000;
    ((struct gd_4_s32 *)sp10)->v = arg1;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE8000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xF5000100;
    ((struct gd_4_s32 *)sp10)->v = 0x7000000;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE6000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xF0000000;
    ((struct gd_4_s32 *)sp10)->v = 0x73FC000;
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE7000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    while (!(sp0 >= 320)) {
        sp8 = sp0 + 32;
        sp4 = 0;
        while (!(sp4 >= 240)) {
            spC = sp4 + 48;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = 0xFD48009F;
            ((struct gd_4_s32 *)sp10)->v = arg0;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = (((((((sp8 - sp0) + 1) >> 1) + 7) >> 3) & 511) << 9) | 0xF5480000;
            ((struct gd_4_s32 *)sp10)->v = 0x7080200;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = 0xE6000000;
            ((struct gd_4_s32 *)sp10)->v = 0;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = ((((sp0 << 1) & 4095) << 12) | 0xF4000000) | ((sp4 << 2) & 4095);
            ((struct gd_4_s32 *)sp10)->v = ((((sp8 << 1) & 4095) << 12) | 0x7000000) | ((spC << 2) & 4095);
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = 0xE7000000;
            ((struct gd_4_s32 *)sp10)->v = 0;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = (((((((sp8 - sp0) + 1) >> 1) + 7) >> 3) & 511) << 9) | 0xF5400000;
            ((struct gd_4_s32 *)sp10)->v = 0x80200;
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = ((((sp0 << 2) & 4095) << 12) | 0xF2000000) | ((sp4 << 2) & 4095);
            ((struct gd_4_s32 *)sp10)->v = (((sp8 << 2) & 4095) << 12) | ((spC << 2) & 4095);
            sp10 = (s32)D_8013EC7C++;
            (*(s32 *)sp10) = ((((sp8 << 2) & 4095) << 12) | 0xE4000000) | ((spC << 2) & 4095);
            ((struct gd_4_s32 *)sp10)->v = (((sp0 << 2) & 4095) << 12) | ((sp4 << 2) & 4095);
            sp14 = (s32)D_8013EC7C++;
            (*(s32 *)sp14) = 0xB4000000;
            ((struct gd_4_s32 *)sp14)->v = (((sp0 << 5) & 0xFFFF) << 16) | ((sp4 << 5) & 0xFFFF);
            sp14 = (s32)D_8013EC7C++;
            (*(s32 *)sp14) = 0xB3000000;
            ((struct gd_4_s32 *)sp14)->v = 0x4000400;
            sp4 = sp4 + 48;
        }
        sp0 = sp0 + 32;
    }
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xE7000000;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBA001402;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBA001301;
    ((struct gd_4_s32 *)sp14)->v = 0x80000;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBA000C02;
    ((struct gd_4_s32 *)sp14)->v = 0x2000;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xB900031D;
    ((struct gd_4_s32 *)sp14)->v = 0x552078;
}
