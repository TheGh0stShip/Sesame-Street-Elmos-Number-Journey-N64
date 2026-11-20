struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
struct n64dk_e64 { u8 b[64]; };
struct n64dk_v4 { u8 pad[0x4]; void *v; };
extern s32 D_80071760;
extern s32 D_800717E0;
extern u8 D_80076B60;
extern s32 D_8010E640;
extern s32 D_8012F154;
extern s32 D_8012FDE0;
extern s32 D_8012FEB0;
extern struct n64dk_e8 *D_8013EC7C;
extern s32 D_80167C10;
extern s32 D_80173AB8;
extern s32 D_80175738;
extern s32 D_80176BA8;
extern s32 D_8017C690;
extern s32 D_8018E4D8;
extern s32 D_801973F8;
extern s32 D_801A9E08;
s32 func_80025AD0();
s32 func_80025DF0();
s32 func_800271AC();
s32 func_80059460();

void f(s32 arg0) {
    s32 sp10;

    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0x1020040;
    ((struct n64dk_v4 *)sp10)->v = (void *)&((struct n64dk_e64 *)&D_80071760)[D_800717E0];
    if ((D_8012F154 == 15) || (D_8012F154 == 16)) {
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBA000C02;
        ((struct gd_4_s32 *)sp10)->v = 0x3000;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xB7000000;
        ((struct gd_4_s32 *)sp10)->v = 0x12204;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xFC121824;
        ((struct gd_4_s32 *)sp10)->v = 0xFF33FFFF;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBA001402;
        ((struct gd_4_s32 *)sp10)->v = 0x100000;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xF8000000;
        ((struct gd_4_s32 *)sp10)->v = 0x4F4FF6FF;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBC000008;
        ((struct gd_4_s32 *)sp10)->v = 0x2155DFAB;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xB900031D;
        ((struct gd_4_s32 *)sp10)->v = 0xC8112038;
    } else {
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBA000C02;
        ((struct gd_4_s32 *)sp10)->v = 0x3000;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xB7000000;
        ((struct gd_4_s32 *)sp10)->v = 0x2204;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xFC127E24;
        ((struct gd_4_s32 *)sp10)->v = -3079;
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xB900031D;
        ((struct gd_4_s32 *)sp10)->v = 0x553078;
    }
    sp10 = (s32)D_8013EC7C++;
    (*(s32 *)sp10) = 0xE7000000;
    ((struct gd_4_s32 *)sp10)->v = 0;
    switch (D_8012F154) {
    case 1:
        func_800271AC(((s32)&D_801A9E08));
        return;
    case 9:
        func_800271AC(((s32)&D_8017C690));
        return;
    case 10:
        func_800271AC(((s32)&D_80173AB8));
        return;
    case 8:
        func_800271AC(((s32)&D_80176BA8));
        return;
    case 7:
        if (D_800717E0 != 0) {
            func_80025DF0(D_8012FDE0);
            D_8012FEB0 = D_8012FDE0;
        } else {
            func_80025DF0(D_8010E640);
            D_8012FEB0 = D_8010E640;
        }
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBC003C06;
        ((struct gd_4_s32 *)sp10)->v = func_80059460(D_8012FEB0);
        func_800271AC(((s32)&D_8018E4D8));
        return;
    case 6:
        func_800271AC(((s32)&D_80175738));
        return;
    case 5:
        if (D_800717E0 != 0) {
            func_80025AD0(D_8012FDE0);
            D_8012FEB0 = D_8012FDE0;
        } else {
            func_80025AD0(D_8010E640);
            D_8012FEB0 = D_8010E640;
        }
        sp10 = (s32)D_8013EC7C++;
        (*(s32 *)sp10) = 0xBC003C06;
        ((struct gd_4_s32 *)sp10)->v = func_80059460(D_8012FEB0);
        func_800271AC(((s32)&D_801973F8));
        return;
    case 11:
        if (D_80076B60 != 1) {
            func_800271AC(((s32)&D_80167C10));
        }
        return;
    }
}
