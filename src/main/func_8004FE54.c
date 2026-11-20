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
struct Struct_anon_29;
struct Struct_anon_31;
struct Struct_func_8004F3CC_arg0;
struct Struct_anon_29 {   /* provisional: 2 field(s) from 1 function(s) */
    /* 0x00 */ char unk_0[0x18];
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ char unk_1C[0x4];
    /* 0x20 */ s32 unk_20;
};
struct Struct_anon_31 {   /* provisional: 2 field(s) from 1 function(s) */
    /* 0x00 */ char unk_0[0x28];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
};
struct Struct_func_8004F3CC_arg0 {   /* provisional: 55 field(s) from 4 function(s) */
    /* 0x00 */ char unk_0[0x4];
    /* 0x04 */ s32 unk_4;
    /* 0x08 */ s32 unk_8;
    /* 0x0C */ s32 unk_C;
    /* 0x10 */ char unk_10[0x1C];
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ char unk_30[0xC];
    /* 0x3C */ s32 unk_3C;
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ char unk_44[0x8];
    /* 0x4C */ f32 unk_4C;
    /* 0x50 */ f32 unk_50;
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ f32 unk_58;
    /* 0x5C */ f32 unk_5C;
    /* 0x60 */ f32 unk_60;
    /* 0x64 */ s32 unk_64;
    /* 0x68 */ char unk_68[0xC];
    /* 0x74 */ struct Struct_anon_29 *unk_74;
    /* 0x78 */ struct Struct_anon_30 *unk_78;
    /* 0x7C */ struct Struct_anon_31 *unk_7C;
    /* 0x80 */ char unk_80[0x4];
    /* 0x84 */ s32 unk_84;
    /* 0x88 */ char unk_88[0xC];
    /* 0x94 */ s32 unk_94;
    /* 0x98 */ char unk_98[0x2];
    /* 0x9A */ u16 unk_9A;
    /* 0x9C */ char unk_9C[0x4];
    /* 0xA0 */ u16 unk_A0;
    /* 0xA2 */ char unk_A2[0x8];
    /* 0xAA */ u16 unk_AA;
    /* 0xAC */ u16 unk_AC;
    /* 0xAE */ u16 unk_AE;
    /* 0xB0 */ char unk_B0[0x2];
    /* 0xB2 */ u16 unk_B2;
    /* 0xB4 */ u16 unk_B4;
    /* 0xB6 */ char unk_B6[0x1];
    /* 0xB7 */ u8 unk_B7;
    /* 0xB8 */ char unk_B8[0x1];
    /* 0xB9 */ u8 unk_B9;
    /* 0xBA */ u8 unk_BA;
    /* 0xBB */ u8 unk_BB;
    /* 0xBC */ char unk_BC[0x1];
    /* 0xBD */ u8 unk_BD;
    /* 0xBE */ char unk_BE[0x1];
    /* 0xBF */ u8 unk_BF;
    /* 0xC0 */ u8 unk_C0;
    /* 0xC1 */ u8 unk_C1;
    /* 0xC2 */ u8 unk_C2;
    /* 0xC3 */ u8 unk_C3;
    /* 0xC4 */ u8 unk_C4;
    /* 0xC5 */ u8 unk_C5;
    /* 0xC6 */ u8 unk_C6;
    /* 0xC7 */ u8 unk_C7;
    /* 0xC8 */ u8 unk_C8;
    /* 0xC9 */ u8 unk_C9;
    /* 0xCA */ u8 unk_CA;
    /* 0xCB */ u8 unk_CB;
    /* 0xCC */ u8 unk_CC;
    /* 0xCD */ u8 unk_CD;
    /* 0xCE */ char unk_CE[0x1];
    /* 0xCF */ u8 unk_CF;
    /* 0xD0 */ u8 unk_D0;
    /* 0xD1 */ u8 unk_D1;
    /* 0xD2 */ u8 unk_D2;
    /* 0xD3 */ u8 unk_D3;
    /* 0xD4 */ u8 unk_D4;
    /* 0xD5 */ char unk_D5[0x1];
    /* 0xD6 */ u8 unk_D6;
    /* 0xD7 */ u8 unk_D7;
    /* 0xD8 */ char unk_D8[0x1];
    /* 0xD9 */ u8 unk_D9;
    /* 0xDA */ u8 unk_DA;
};
void func_8004FE54(struct Struct_func_8004F3CC_arg0 *);

void func_8004FE54(struct Struct_func_8004F3CC_arg0 *arg0) {
    arg0->unk_D9 = 0;
    arg0->unk_94 = arg0->unk_40;
    arg0->unk_DA = arg0->unk_BD & 0x40;
}
/* Warning: struct Struct_anon_30 is not defined (only forward-declared) */
