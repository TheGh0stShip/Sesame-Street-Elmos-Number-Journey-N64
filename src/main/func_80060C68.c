s32 func_80063440(void *, s32, s32);
/* writes the 4 bytes of value through a partial-write routine until done */
void func_80060C68(u32 value) {
    s32 n = 0;
    u8 *p = (u8 *)&value;
    do {
        n += func_80063440(p + n, 4 - n, 8);
    } while (n < 4);
}
