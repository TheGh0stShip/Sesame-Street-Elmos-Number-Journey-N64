#include "common.h"

extern s32 D_80128578;

void func_800125D4(s32 arg0) {
    register s32 reg_s0;
    register s32 reg_s1;
    s32 sp20;

    switch ((*(u8 (*))&D_8012F150)) {
    case 0:
        if (D_80076800 == 0) {
            return;
        }
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xE7000000;
        ((struct gd_4_s32 *)sp20)->v = 0;
        guPosition(((D_800717E0 * 64) + ((s32)&D_800A9158)), 0, 0, 0, 0.25f, 0, 0, 0);
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0x1020040;
        ((struct gd_4_s32 *)sp20)->v = (D_800717E0 * 64) + ((s32)&D_000A9158);
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xBA001402;
        ((struct gd_4_s32 *)sp20)->v = 0;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xB6000000;
        ((struct gd_4_s32 *)sp20)->v = 0x9F3207;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xB7000000;
        ((struct gd_4_s32 *)sp20)->v = 1;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xB900031D;
        ((struct gd_4_s32 *)sp20)->v = 0x504A50;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xFC309661;
        ((struct gd_4_s32 *)sp20)->v = 0x552EFF7F;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xBA000C02;
        ((struct gd_4_s32 *)sp20)->v = 0x2000;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xBB000001;
        ((struct gd_4_s32 *)sp20)->v = 0x80008000;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xBA000E02;
        ((struct gd_4_s32 *)sp20)->v = 0;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xFD900000;
        ((struct gd_4_s32 *)sp20)->v = (s32)&D_800731E0;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5900000;
        ((struct gd_4_s32 *)sp20)->v = 0x7094250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xE6000000;
        ((struct gd_4_s32 *)sp20)->v = 0;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF3000000;
        ((struct gd_4_s32 *)sp20)->v = 0x77FF400;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xE7000000;
        ((struct gd_4_s32 *)sp20)->v = 0;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800400;
        ((struct gd_4_s32 *)sp20)->v = 0xF94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x7C3FC;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800400;
        ((struct gd_4_s32 *)sp20)->v = 0xF94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x7C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800440;
        ((struct gd_4_s32 *)sp20)->v = 0x1F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x107C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800480;
        ((struct gd_4_s32 *)sp20)->v = 0x2F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x207C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF58004C0;
        ((struct gd_4_s32 *)sp20)->v = 0x3F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x307C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800500;
        ((struct gd_4_s32 *)sp20)->v = 0x4F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x407C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800540;
        ((struct gd_4_s32 *)sp20)->v = 0x5F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x507C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF5800580;
        ((struct gd_4_s32 *)sp20)->v = 0x6F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x607C07C;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF58005C0;
        ((struct gd_4_s32 *)sp20)->v = 0x7F94250;
        sp20 = (s32)D_8013EC7C++;
        (*(s32 *)sp20) = 0xF2000000;
        ((struct gd_4_s32 *)sp20)->v = 0x707C07C;
        if (D_800717E0 == 0) {
            D_80128578 = (s32)&D_80109E40;
        } else {
            D_80128578 = (s32)&D_8010E690;
        }
        reg_s0 = 0x800A0000;
        reg_s0 = ((struct gd_7F74_s32 *)reg_s0)->v;
        D_800BD87C = (s32)((struct gd_24_f32 *)D_80076800)->v;
        D_80106C6E = (s32)((struct gd_2C_f32 *)D_80076800)->v;
        while (reg_s0 != 0) {
            reg_s1 = *(s32 *)reg_s0;
            if (((struct gd_8_s32 *)reg_s0)->v != 0) {
                ((s32 (*)())((struct gd_8_s32 *)reg_s0)->v)(reg_s0);
            }
            reg_s0 = reg_s1;
        }
    }
}
