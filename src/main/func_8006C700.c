/* range check (a0 > 7 exits) then calls into the following routine with the return address kept in at */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_8006C700\n"
    "func_8006C700:\n"
    "    addiu $1, $4, -7\n"
    "    bgtz $1, . + 0x38\n"
    "    nop\n"
    "    addu $1, $31, $0\n"
    "    bltzal $0, . + 0xDC\n"
    ".set at\n"
    ".set reorder\n"
);
