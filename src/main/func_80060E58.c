/* strlen */
u32 func_80060E58(const u8 *s) {
    const u8 *sc = s;
    while (*sc != 0) sc++;
    return sc - s;
}
