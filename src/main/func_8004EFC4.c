#ifndef N64DK_M2C_PRELUDE
#define N64DK_M2C_PRELUDE
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef signed long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
typedef s32 M2C_UNK; typedef s8 M2C_UNK8; typedef s16 M2C_UNK16; typedef s32 M2C_UNK32; typedef s64 M2C_UNK64;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
/* unhandled instructions become calls to an unprototyped function: any argument list compiles */
extern s32 m2c_unhandled();
#define M2C_ERROR m2c_unhandled
#define M2C_TRAP_IF m2c_unhandled
#define M2C_BREAK m2c_unhandled
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)
#define MULT_HI(a, b) ((s32)(((s64)(a) * (s64)(b)) >> 32))
#define MULTU_HI(a, b) ((u32)(((u64)(a) * (u64)(b)) >> 32))
#define M2C_CARRY 0
#ifndef NULL
#define NULL ((void *)0)
#endif
#endif

/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */
struct Struct_func_8004EFC4_arg0;
struct Struct_func_8004EFC4_arg0 {   /* provisional: 2 field(s) from 1 function(s) */
    /* 0x00 */ u8 unk_0;
    /* 0x01 */ char unk_1[0x3];
    /* 0x04 */ s32 unk_4;
};
void func_80050D7C(s32, s32, s32);
void func_80051FB0(s32);
void func_8004EFC4(struct Struct_func_8004EFC4_arg0 *);

M2C_UNK ChangeCustomEffect();                    /* extern */

void func_8004EFC4(struct Struct_func_8004EFC4_arg0 *arg0)
{
  s32 temp_v1_7;
  temp_v1_7 = arg0->unk_0;
  switch (temp_v1_7)
  {
    case 0:
      func_80050D7C(arg0->unk_4, -2, 1);
      return;

    case 1:
      func_80050D7C(arg0->unk_4, -2, 0);
      return;

    case 2:
      ChangeCustomEffect(arg0->unk_4);
      return;

  }

}


