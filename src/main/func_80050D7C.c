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

extern s32 D_800A0544;
extern void *D_800A054C;
struct Fields_var_var_v1_9
{
  s32 unk_0;
  char pad_4[0x40];
  s32 unk_44;
};

void func_80050D7C(s32 arg0, s32 arg1, s32 arg2)
{
    s32 pad[2]; /* n64dk: frame padding, reconstruction scaffolding */
  s32 var_a3_11;
  struct Fields_var_var_v1_9 *var_v1_9;
  var_v1_9 = D_800A054C;
  var_a3_11 = 0;
  if (D_800A0544 > 0)
  {
    do
    {
      var_a3_11 += 1;
      if (var_v1_9->unk_44 == arg0)
      {
        var_v1_9->unk_0 = (s32) ((var_v1_9->unk_0 & arg1) | arg2);
      }
      var_v1_9 = (struct Fields_var_var_v1_9 *) (((s8 *) var_v1_9) + 0x13C);
    }
    while (var_a3_11 < D_800A0544);
  }
}


