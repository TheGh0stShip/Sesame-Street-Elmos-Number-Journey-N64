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
struct Struct_func_80050DD0_arg0;
struct Struct_func_80050DD0_arg0 {   /* provisional: 6 field(s) from 1 function(s) */
    /* 0x00 */ char unk_0[0x8];
    /* 0x08 */ s32 unk_8;
    /* 0x0C */ char unk_C[0x28];
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ char unk_3C[0x8];
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ char unk_48[0x2C];
    /* 0x74 */ s32 unk_74;
    /* 0x78 */ s32 unk_78;
};
s32 func_80050DD0(struct Struct_func_80050DD0_arg0 *);

s32 func_80050DD0(struct Struct_func_80050DD0_arg0 *arg0) {
    arg0->unk_38 = 0;
    arg0->unk_34 = 0;
    arg0->unk_74 = 0;
    arg0->unk_78 = 0;
    arg0->unk_44 = 0;
    arg0->unk_8 = 0;
    return 0;
}
