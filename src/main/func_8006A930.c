/* 64-bit unsigned divide/remainder dispatch: tail-calls the 32-bit-divisor path when the divisor high word is zero */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_8006A930\n"
    "func_8006A930:\n"
    "    bnez $6, 1f\n"
    "    nop\n"
    "    j func_8006A6F4\n"
    "    nop\n"
    "    1: j func_8006A5F8\n"
    "    nop\n"
    "    nop\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
