/* Returns whether bit 0x100 of the DP status register (0xA410000C) is set. */
s32 func_80068420(void) {
    u32 s = *(volatile u32 *)0xA410000C & 0x100;
    return s != 0;
}
