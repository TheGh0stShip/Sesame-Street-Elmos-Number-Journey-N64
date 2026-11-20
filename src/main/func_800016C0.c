struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct n64dk_a8006DD18 { s32 m0; s32 m4; };
extern struct n64dk_a8006DD18 D_8006F688[];

void f(void) {
    s32 sp10;
    s32 sp14;

    for (sp10 = 0; !(sp10 >= 8); sp10++) {
        if ((sp14 = D_8006F688[sp10].m0) != 0) {
            if (D_8006F688[sp10].m4 != 0) {
                ((s32 (*)())D_8006F688[sp10].m4)(0);
            }
            for (; !(sp14 == 0); sp14 = ((struct gd_14_s32 *)sp14)->v) {
                if (((struct gd_8_s32 *)sp14)->v != 0) {
                    ((s32 (*)())((struct gd_8_s32 *)sp14)->v)(((struct gd_C_s32 *)sp14)->v);
                }
                if (((*(s32 *)sp14) & 1) != 0) {
                    if (D_8006F688[sp10].m4 != 0) {
                        ((s32 (*)())D_8006F688[sp10].m4)(0);
                    }
                }
            }
        }
    }
}
