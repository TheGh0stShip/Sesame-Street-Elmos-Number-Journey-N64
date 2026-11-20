/* hand-written assembly routine (SDK) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_800628AC\n"
    "func_800628AC:\n"
    "    nop\n"
    "    addiu $9, $0, 0x1\n"
    "    sw $9, 0x18($26)\n"
    "    lw $27, 0x118($26)\n"
    "    lui $1, 0x2000\n"
    "    or $27, $27, $1\n"
    "    beq $0, $0, . - 0x130\n"
    "    sw $27, 0x118($26)\n"
    ".set at\n"
    ".set reorder\n"
);
