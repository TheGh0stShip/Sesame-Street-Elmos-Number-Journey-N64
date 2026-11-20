/* func_80060D80 (libultra func_80060D80.s, hand-written assembly) */
#include "asm.h"

.text
.set noreorder
.set noat
.globl func_80060D80
func_80060D80:
  negu $v1, $a0
  slti $at, $a1, 0xC
  bnez $at, .Lbytes
  nop
  andi $v1, $v1, 0x3
  beqz $v1, .Lblk
  subu $a1, $a1, $v1
  swl $zero, 0($a0)
  addu $a0, $a0, $v1
.Lblk:
  addiu $at, $zero, -0x20
  and $a3, $a1, $at
  beqz $a3, .Lwords
  subu $a1, $a1, $a3
  addu $a3, $a3, $a0
.Lblkloop:
  sw $zero, 0($a0)
  sw $zero, 4($a0)
  sw $zero, 8($a0)
  sw $zero, 12($a0)
  addiu $a0, $a0, 0x20
  sw $zero, -16($a0)
  sw $zero, -12($a0)
  sw $zero, -8($a0)
  bne $a0, $a3, .Lblkloop
  sw $zero, -4($a0)
.Lwords:
  addiu $at, $zero, -4
  and $a3, $a1, $at
  beqz $a3, .Lbytes
  subu $a1, $a1, $a3
  addu $a3, $a3, $a0
.Lwloop:
  addiu $a0, $a0, 4
  bne $a0, $a3, .Lwloop
  sw $zero, -4($a0)
.Lbytes:
  blez $a1, .Ldone
  nop
  addu $a1, $a1, $a0
.Lbloop:
  addiu $a0, $a0, 1
  bne $a0, $a1, .Lbloop
  sb $zero, -1($a0)
.Ldone:
  jr $ra
  nop
.set at
.set reorder

/* known names (symbols.number.tsv) */
#undef bzero
.globl bzero
bzero = func_80060D80
