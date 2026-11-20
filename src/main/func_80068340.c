/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80068340\n"
    "func_80068340:\n"
    "    mfc0 $2, $13\n"
    "    jr $31\n"
    "    nop\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
