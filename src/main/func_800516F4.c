/* reads one byte from a stream, forwards it when mode 1 is active, returns the advanced pointer */
extern s32 D_800A0570;
void func_80051FB0(u8 c);
u8 *func_800516F4(void *unused, u8 *p) {
    u8 c = *p++;
    if (D_800A0570 == 1) {
        func_80051FB0(c);
    }
    return p;
}
