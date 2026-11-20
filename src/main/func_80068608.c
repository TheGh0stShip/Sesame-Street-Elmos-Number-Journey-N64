/* writes 0xC5 to PI register 0xA4040010 via the shared tail of the previous routine */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80068608\n"
    "func_80068608:\n"
    "    lui $4, 0xA404\n"
    "    ori $4, $4, 0x10\n"
    "    addiu $5, $0, 0xC5\n"
    "    beq $0, $0, . - 0x2C\n"
    "    sw $5, 0($4)\n"
    ".set at\n"
    ".set reorder\n"
);
