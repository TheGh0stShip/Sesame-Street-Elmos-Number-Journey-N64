/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80062D80\n"
    "func_80062D80:\n"
    "    mtc0 $4, $12\n"
    "    nop\n"
    "    jr $31\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
