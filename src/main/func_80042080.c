struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct n64dk_a801151BC { s32 m0; u8 pad4[0x4]; s32 m8; u8 padC[0x4]; };
extern struct n64dk_a801151BC D_8011536C[];
extern s32 D_8013C724;

s32 f(s32 arg0) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;

    sp8 = 0;
    arg0 = arg0 + 15;
    arg0 = arg0 & -16;
    sp10 = 0;
    while (!((u32)sp10 >= (u32)D_8013C724)) {
        sp0 = D_8011536C[sp10].m8;
        while (!(sp0 == 0)) {
            if ((*(s32 *)sp0) == 0) {
                spC = D_8011536C[sp10].m0 - (sp0 + 16);
            } else {
                spC = (*(s32 *)sp0) - (sp0 + 16);
            }
            if ((u32)spC < (u32)arg0) {
                sp0 = ((struct gd_8_s32 *)sp0)->v;
            } else {
                if ((u32)spC > (u32)arg0) {
                    sp4 = (sp0 + 16) + arg0;
                    ((struct gd_4_s32 *)sp4)->v = sp0;
                    (*(s32 *)sp4) = *(s32 *)sp0;
                    ((struct gd_8_s32 *)sp4)->v = ((struct gd_8_s32 *)sp0)->v;
                    ((struct gd_C_s32 *)sp4)->v = sp0;
                    (*(s32 *)sp0) = sp4;
                    ((struct gd_8_s32 *)sp0)->v = sp4;
                    if ((*(s32 *)sp4) != 0) {
                        ((struct gd_4_s32 *)(*(s32 *)sp4))->v = sp4;
                    }
                    if (((struct gd_8_s32 *)sp4)->v != 0) {
                        ((struct gd_C_s32 *)((struct gd_8_s32 *)sp4)->v)->v = sp4;
                    }
                }
                if (((struct gd_C_s32 *)sp0)->v != 0) {
                    ((struct gd_8_s32 *)((struct gd_C_s32 *)sp0)->v)->v = ((struct gd_8_s32 *)sp0)->v;
                } else {
                    D_8011536C[sp10].m8 = ((struct gd_8_s32 *)sp0)->v;
                }
                if (((struct gd_8_s32 *)sp0)->v != 0) {
                    ((struct gd_C_s32 *)((struct gd_8_s32 *)sp0)->v)->v = ((struct gd_C_s32 *)sp0)->v;
                }
                ((struct gd_8_s32 *)sp0)->v = sp10;
                ((struct gd_C_s32 *)sp0)->v = -1;
                sp8 = sp0 + 16;
                return sp8;
            }
        }
        sp10++;
    }
    return 0;
}
