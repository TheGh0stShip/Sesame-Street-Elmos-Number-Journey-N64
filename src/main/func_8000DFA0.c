s32 f(s32 arg0, u8 arg1) {
    while (((*(u8 *)arg0) != 0) && ((*(u8 *)arg0) != arg1)) {
        arg0++;
    }
    if ((*(u8 *)arg0) != 0) {
        return arg0;
    }
    return 0;
}
