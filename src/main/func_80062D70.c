/* sets the FPU control/status register, returning the previous value */
u32 func_80062D70(u32 csr) {
    u32 old;
    __asm__ volatile("cfc1 %0, $31" : "=r"(old));
    __asm__ volatile("ctc1 %0, $31" : : "r"(csr));
    return old;
}
