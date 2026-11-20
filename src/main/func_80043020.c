struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
s32 func_80042880();
s32 func_80046590();
s32 func_80046810();

void f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 sp14;

    if (arg1 == 0) {
        return;
    }
    sp10 = *(s32 *)arg1;
    while ((sp10 != 0) && (((struct gd_38_f32 *)arg0)->v > 0.0f)) {
        sp14 = ((struct gd_8_s32 *)sp10)->v;
        if ((((struct gd_30_s32 *)arg0)->v & ((struct gd_8_s32 *)sp14)->v) != 0) {
            switch (*(s32 *)sp14) {
            case 3:
                func_80042880(arg0, sp14);
                break;
            case 1:
                func_80046590(arg0, sp14);
                break;
            case 12:
                func_80046810(arg0, sp14);
                break;
            }
        }
        sp10 = *(s32 *)sp10;
    }
}
