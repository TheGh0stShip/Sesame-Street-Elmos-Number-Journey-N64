/* __osEnqueueThread (hand-written assembly): insert thread a1 into priority queue a0 */
__asm__(
".set noreorder\n.set noat\n"
".globl func_800629D4\n"
"func_800629D4:\n"
"addu $t9, $a0, $zero\n lw $t8, 0($a0)\n lw $t7, 4($a1)\n lw $t6, 4($t8)\n slt $at, $t6, $t7\n bnez $at, 2f\n nop\n"
"1: addu $t9, $t8, $zero\n lw $t8, 0($t8)\n lw $t6, 4($t8)\n slt $at, $t6, $t7\n beqz $at, 1b\n nop\n"
"2: lw $t8, 0($t9)\n sw $t8, 0($a1)\n sw $a1, 0($t9)\n jr $ra\n sw $a0, 8($a1)\n"
".set reorder\n.set at\n");
