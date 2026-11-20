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

struct gd_2_u16 { u8 pad[0x2]; u16 v; };
struct gd_6_u16 { u8 pad[0x6]; u16 v; };
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
struct gd_38_s32 { u8 pad[0x38]; s32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
extern s32 func_80023408;
extern s32 func_80033528;
extern s32 D_80076800;
extern s32 D_8007A960;
extern s32 D_8007A964;
extern s32 D_80107CE0;
extern s32 D_80115408;
s32 func_80041A80();
s32 func_80042080();
s32 func_80059540();
s32 func_80069C70();

void func_80033A6C(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    if ((*(s32 *)sp10) == 1) {
        goto L13;
    }
    func_80059540(((s32)&D_8007A960), ((s32)&D_8007A964), 3679);
L13: ;
    D_80107CE0 = func_80042080(88);
    func_80069C70(D_80107CE0, 0, 88);
    D_80076800 = D_80107CE0;
    ((struct gd_8_s16 *)D_80107CE0)->v = ((struct gd_2_u16 *)sp10)->v;
    ((struct gd_A_s16 *)D_80107CE0)->v = ((struct gd_6_u16 *)sp10)->v;
    ((struct gd_C_s32 *)D_80107CE0)->v = ((struct gd_8_s32 *)sp10)->v;
    ((struct gd_14_s32 *)D_80107CE0)->v = (s32)&func_80023408;
    ((struct gd_18_s32 *)D_80107CE0)->v = 0;
    ((struct gd_1C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_20_s32 *)D_80107CE0)->v = 0;
    ((struct gd_10_s32 *)D_80107CE0)->v = (s32)&func_80033528;
    ((struct gd_24_f32 *)D_80107CE0)->v = ((struct gd_C_f32 *)sp10)->v;
    ((struct gd_28_f32 *)D_80107CE0)->v = ((struct gd_10_f32 *)sp10)->v;
    ((struct gd_2C_f32 *)D_80107CE0)->v = ((struct gd_14_f32 *)sp10)->v;
    ((struct gd_24_s32 *)D_80107CE0)->v = 0;
    ((struct gd_28_f32 *)D_80107CE0)->v = 5e+02f;
    ((struct gd_2C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_30_s32 *)D_80107CE0)->v = 0;
    ((struct gd_34_s32 *)D_80107CE0)->v = 0;
    ((struct gd_38_s32 *)D_80107CE0)->v = 0;
    ((struct gd_3C_s32 *)D_80107CE0)->v = 0;
    ((struct gd_40_f32 *)D_80107CE0)->v = 1.0f;
    ((struct gd_44_s32 *)D_80107CE0)->v = 0;
    func_80041A80(D_80107CE0, D_80115408, 3, 0);
}
