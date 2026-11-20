/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80061DE0\n"
    "func_80061DE0:\n"
    "    lui $26, 0x8006\n"
    "    addiu $26, $26, 0x1EB4\n"
    "    jr $26\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
