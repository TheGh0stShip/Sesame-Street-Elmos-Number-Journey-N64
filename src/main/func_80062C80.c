/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80062C80\n"
    "func_80062C80:\n"
    "    mfc0 $8, $12\n"
    "    or $8, $8, $4\n"
    "    mtc0 $8, $12\n"
    "    nop\n"
    "    nop\n"
    "    jr $31\n"
    "    nop\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
