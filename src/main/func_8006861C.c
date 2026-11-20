/* hand-written: write 2 to MI 0xA430000C (interrupt mask) and 0x125 to SP_STATUS (0xA4040010) */
__asm__(".set noreorder\n.set noat\n.globl func_8006861C\nfunc_80066CAC:\n"
"lui $a0, 0xA430\nori $a0, $a0, 0xC\naddiu $a1, $zero, 0x2\nsw $a1, 0x0($a0)\n"
"lui $a0, 0xA404\nori $a0, $a0, 0x10\naddiu $a1, $zero, 0x125\njr $ra\nsw $a1, 0x0($a0)\n.set at\n.set reorder\n");
