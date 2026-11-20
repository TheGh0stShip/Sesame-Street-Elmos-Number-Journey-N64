/* asm.h: assembler macros and register numbers after the SDK's sys/asm.h and sys/regdef.h, for the .s files in
   asm/ (run through the C preprocessor before KMC GNU as). Registers are written $name: the name expands to
   its number. */
#ifndef ASM_H
#define ASM_H
#define zero 0
#define v0 2
#define v1 3
#define a0 4
#define a1 5
#define a2 6
#define a3 7
#define t0 8
#define t1 9
#define t2 10
#define t3 11
#define t4 12
#define t5 13
#define t6 14
#define t7 15
#define s0 16
#define s1 17
#define s2 18
#define s3 19
#define s4 20
#define s5 21
#define s6 22
#define s7 23
#define t8 24
#define t9 25
#define k0 26
#define k1 27
#define gp 28
#define sp 29
#define fp 30
#define s8 30
#define ra 31
#define LEAF(x) .globl x; .align 2; .ent x,0; x:
#define END(x) .end x
#define NOP .set noreorder; nop; .set reorder
#define MFC0(reg, op) .set noreorder; mfc0 reg, op; .set reorder
#define MTC0(reg, op) .set noreorder; mtc0 reg, op; .set reorder
#define CFC1(reg, op) .set noreorder; cfc1 reg, op; .set reorder
#define CTC1(reg, op) .set noreorder; ctc1 reg, op; .set reorder
#endif
