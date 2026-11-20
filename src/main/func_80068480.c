/* __osSpSetPc */
s32 func_80068480(u32 pc) {
    if (!(*(volatile u32 *)0xA4040010 & 1)) {
        return -1;
    }
    *(volatile u32 *)0xA4080000 = pc;
    return 0;
}
