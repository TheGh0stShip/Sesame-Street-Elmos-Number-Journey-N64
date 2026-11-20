/* hand-written asm tail of a 64-bit unsigned compare: (a0:a1) vs (a2:a3); branches into the neighbouring
   less-than / greater-than exits at 0x80068C10 / 0x80068C18 and returns 1 when equal */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_8006A590\n"
    "func_8006A590:\n"
    "    sltu $25, $4, $6\n"
    "    bnez $25, .-20\n"
    "    nop\n"
    "    bne $6, $4, .-20\n"
    "    nop\n"
    "    sltu $25, $5, $7\n"
    "    bnez $25, .-40\n"
    "    nop\n"
    "    bne $7, $5, .-40\n"
    "    nop\n"
    "    jr $31\n"
    "    addiu $2, $0, 1\n"
    "    nop\n"
    ".set at\n"
    ".set reorder\n"
);
