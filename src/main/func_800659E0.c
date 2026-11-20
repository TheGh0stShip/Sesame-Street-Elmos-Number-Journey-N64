/* reads a word from a PI-domain address via func_800637F0; returns 0 when out of range */
s32 func_800637F0(u32 addr, u32 *data);
u32 func_800659E0(u32 addr) {
    u32 data;
    if (addr - 0x04000000 < 0x1000000) {
        func_800637F0(addr, &data);
        return data;
    }
    return 0;
}
