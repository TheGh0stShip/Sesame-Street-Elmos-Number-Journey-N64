struct gd_10_s32 { u8 pad[0x10]; s32 v; };

void f(s32 arg0) {
    s32 sp10;

    sp10 = *(s32 *)arg0;
    while (!(sp10 == 0)) {
        if (((struct gd_10_s32 *)sp10)->v != 0) {
            ((s32 (*)())((struct gd_10_s32 *)sp10)->v)(sp10);
        }
        sp10 = *(s32 *)sp10;
    }
    return;
}
