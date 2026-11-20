/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80062D60\n"
    "func_80062D60:\n"
    "    mtc0 $4, $11\n"
    "    jr $31\n"
    "    nop\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
