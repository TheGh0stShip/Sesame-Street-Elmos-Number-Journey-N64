s32 func_80041E10();

void f(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = *(s32 *)arg0;
    while (!(sp10 == 0)) {
        sp14 = *(s32 *)sp10;
        func_80041E10(sp10, arg0);
        sp10 = sp14;
    }
    return;
}
