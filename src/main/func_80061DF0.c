/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80061DF0\n"
    "func_80061DF0:\n"
    "    sw $26, -0x10($29)\n"
    "    beq $0, $0, .L80061DF0_14\n"
    "    lui $26, 0x8006\n"
    "    nop\n"
    "    nop\n"
    ".L80061DF0_14:\n"
    "    addiu $26, $26, 0x1E10\n"
    "    jr $26\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
