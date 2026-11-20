s32 func_80063440(void *, u32, s32);
s32 func_80061224(void *unused, u8 *buf, u32 len) {
    u32 n = 0;
    while (n < len) {
        n += func_80063440(buf + n, len - n, 1);
    }
    return 1;
}
