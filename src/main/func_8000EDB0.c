s32 func_8000EC10();

void f(s32 arg0) {
    for (;;) {
        if ((*(u8 *)arg0) == 0) {
            return;
        }
        func_8000EC10((*(u8 *)(arg0++)));
    }
}
