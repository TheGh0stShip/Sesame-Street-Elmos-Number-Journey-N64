/* hand-written: wait for SP DMA idle, then write SP_STATUS 0x82 and wait for halt */
__asm__(
".set noreorder\n.set noat\n"
".globl func_800685C4\n"
"func_800685C4:\n"
"lui $a0, 0xA404\n ori $a0, $a0, 0x18\n"
"1: lw $v0, 0($a0)\n bnez $v0, 1b\n nop\n"
"addiu $a1, $zero, 0x82\n lui $a0, 0xA404\n ori $a0, $a0, 0x10\n sw $a1, 0($a0)\n"
"2: lui $a0, 0xA404\n ori $a0, $a0, 0x10\n lw $v0, 0($a0)\n andi $v0, $v0, 0x3\n beqz $v0, 2b\n nop\n"
"jr $ra\n nop\n"
".set reorder\n.set at\n");
