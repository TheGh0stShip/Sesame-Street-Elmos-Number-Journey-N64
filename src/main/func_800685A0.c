/* hand-written: returns 1 when the SP status register's halt/broke bits are clear (SP busy test) */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_800685A0\n"
    "func_800685A0:\n"
    "    addu $2, $0, $0\n"
    "    lui $8, 0xA404\n"
    "    lw $8, 0x10($8)\n"
    "    andi $8, $8, 0x3\n"
    "    bnez $8, 1f\n"
    "    nop\n"
    "    ori $2, $2, 0x1\n"
    "1:  jr $31\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
