#ifndef STRUCTS_H
#define STRUCTS_H
#include "ultra64.h"

struct gd_2_s16 { u8 pad[0x2]; s16 v; };
struct gd_2_u16 { u8 pad[0x2]; u16 v; };
struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_4_u16 { u8 pad[0x4]; u16 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct Pk { s32 z : 10; s32 y : 11; s32 x : 11; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s16 { u8 pad[0x20]; s16 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_2_u8 { u8 pad[0x2]; u8 v; };
struct gd_3_u8 { u8 pad[0x3]; u8 v; };
struct gd_4_u8 { u8 pad[0x4]; u8 v; };
struct n64dk_e6 { u8 b[6]; };
struct n64dk_a801078B8 { s16 m0; u16 m2; s8 m4; s8 m5; };
struct n64dk_a801078BA { u16 m0; u8 pad2[0x4]; };
typedef struct { u8 pad[0x80C8]; void *unk_80C8; } GfxCtx;
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
struct n64dk_e64 { u8 b[64]; };
struct n64dk_a8013EB38 { s32 m0; s32 m4; s32 m8; };
struct n64dk_a8006F760 { s32 m0; s32 m4; };
struct n64dk_a8006F798 { s32 m0; s32 m4; };
struct n64dk_a8006F7E0 { s32 m0; s32 m4; };
struct n64dk_a8006FC60 { s32 m0; s32 m4; };
struct n64dk_a8006FCA8 { s32 m0; s32 m4; };
struct n64dk_a8013ECA8 { s32 m0; s32 m4; s32 m8; };
typedef struct { s32 x; s32 y; char text[0x28]; } CreditLine;
typedef struct { u8 pad[0x60]; u64 unk_60; u8 pad68[0x80D0 - 0x68]; } Frame80D0;
struct gd_8_f64 { u8 pad[0x8]; f64 v; };
struct gd_10_f64 { u8 pad[0x10]; f64 v; };
struct gd_18_f64 { u8 pad[0x18]; f64 v; };
struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_20_f64 { u8 pad[0x20]; f64 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct n64dk_r16 { s32 w[4]; };
struct gd_28_s16 { u8 pad[0x28]; s16 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
typedef struct Effect { u8 pad0[0xC]; u16 unk_C; u8 padE[2]; u16 unk_10; u8 pad12[2]; u16 unk_14; u8 pad16[2]; s8 unk_18; u8 pad19[0x22 - 0x19]; u8 unk_22; u8 unk_23; u16 unk_24; } Effect;
struct gd_19_s8 { u8 pad[0x19]; s8 v; };
struct gd_1A_s8 { u8 pad[0x1A]; s8 v; };
struct gd_1A_u8 { u8 pad[0x1A]; u8 v; };
struct gd_1B_s8 { u8 pad[0x1B]; s8 v; };
struct gd_18_s8 { u8 pad[0x18]; s8 v; };
struct gd_18_u8 { u8 pad[0x18]; u8 v; };
struct gd_22_s8 { u8 pad[0x22]; s8 v; };
struct gd_23_s8 { u8 pad[0x23]; s8 v; };
struct gd_24_s16 { u8 pad[0x24]; s16 v; };
struct gd_21_u8 { u8 pad[0x21]; u8 v; };
struct gd_C_s16 { u8 pad[0xC]; s16 v; };
struct gd_10_s16 { u8 pad[0x10]; s16 v; };
struct gd_14_s16 { u8 pad[0x14]; s16 v; };
struct gd_21_s8 { u8 pad[0x21]; s8 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
struct gd_7F74_s32 { u8 pad[0x7F74]; s32 v; };
typedef struct SndCmd { s32 unk_00; s16 unk_04; } SndCmd;
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_28_s32 { u8 pad[0x28]; s32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
struct gd_1_u8 { u8 pad[0x1]; u8 v; };
struct IB { s32 f00, f04, f08; s32 *f0C; s32 f10, f14, f18, f1C, f20, f24, f28, f2C, f30, f34; };
struct ZS { s32 f00, f04, f08; };
struct gd_10_s8 { u8 pad[0x10]; s8 v; };
struct gd_11_s8 { u8 pad[0x11]; s8 v; };
struct gd_B_u8 { u8 pad[0xB]; u8 v; };
struct gd_10_u8 { u8 pad[0x10]; u8 v; };
struct gd_11_u8 { u8 pad[0x11]; u8 v; };
typedef unsigned char Byte;
typedef unsigned int uInt;
typedef struct inflate_huft_s inflate_huft;
struct inflate_huft_s { union { struct { Byte Exop; Byte Bits; } what; Byte *pad; } word; union { uInt Base; inflate_huft *Next; } more; };
typedef struct { s32 unk_0; s32 unk_4[3]; s32 unk_10; s32 unk_14[3]; s32 unk_20[3]; } Quiz;
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_8_u16 { u8 pad[0x8]; u16 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_54_f32 { u8 pad[0x54]; f32 v; };
struct gd_58_f32 { u8 pad[0x58]; f32 v; };
struct gd_5C_f32 { u8 pad[0x5C]; f32 v; };
struct gd_6C_s32 { u8 pad[0x6C]; s32 v; };
struct gd_70_s32 { u8 pad[0x70]; s32 v; };
struct gd_74_s32 { u8 pad[0x74]; s32 v; };
struct gd_60_s32 { u8 pad[0x60]; s32 v; };
struct gd_64_s32 { u8 pad[0x64]; s32 v; };
struct gd_68_s32 { u8 pad[0x68]; s32 v; };
typedef struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; } Sel;
typedef struct { u8 pad0[0xC]; s32 flags; u8 pad10[0x14]; s32 timer; u32 state; s32 duration; } Scene;
struct n64dk_a80074664 { s32 m0; s32 m4; };
struct n64dk_a80074714 { s32 m0; s32 m4; };
struct n64dk_a800747F4 { s32 m0; s32 m4; };
struct n64dk_a800748C4 { s32 m0; s32 m4; };
struct n64dk_a800748DC { s32 m0; s32 m4; };
struct n64dk_a800748F4 { s32 m0; s32 m4; };
struct n64dk_a8007490C { s32 m0; s32 m4; };
struct n64dk_a80074924 { s32 m0; s32 m4; };
struct n64dk_a8007494C { s32 m0; s32 m4; };
struct n64dk_a8007495C { s32 m0; s32 m4; };
struct n64dk_a8007496C { s32 m0; s32 m4; };
struct n64dk_a80074984 { s32 m0; s32 m4; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
typedef struct { s32 unk_0; f32 start[3]; f32 end[3]; f32 dir[3]; f32 len; f32 unk_2C[3]; f32 limitBase; } Seg;
typedef struct { u32 time; f32 v[3]; } VecKey;
typedef struct { u32 time; f32 q[4]; s32 unk_14; s32 unk_18; } RotKey;
typedef struct { u32 numTrans; VecKey *trans; u32 numRot; RotKey *rot; u32 numScale; VecKey *scale; } AnimTrack;
typedef struct { f32 m[4][4]; } MtxF;
typedef struct HitInfo { s32 unk_00; s32 unk_04; s32 unk_08; } HitInfo;
struct gd_6_u16 { u8 pad[0x6]; u16 v; };
typedef struct { f32 x0, unk_04, z0, x1, unk_10, z1; } Pts;
typedef struct Elem40 { u8 unk_00[0x40]; } Elem40;
struct gd_2_s8 { u8 pad[0x2]; s8 v; };
struct gd_3_s8 { u8 pad[0x3]; s8 v; };
typedef struct { s8 unk_0; s8 unk_1; union { u16 h; s8 b; } unk_2; } Sub1D4;
typedef struct { u8 unk_00[0x40]; } Sub40;
struct n64dk_mE8_40 { u8 pad[0xE8]; struct { u8 b[0x40]; } m[1]; };
typedef struct { s32 a, b, c; } Trip;
struct gd_1A8_t { u8 pad[0x1A8]; Trip v; };
struct gd_1B0_s32 { u8 pad[0x1B0]; s32 v; };
struct gd_1F0_s32 { u8 pad[0x1F0]; s32 v; };
struct gd_1E8_s32 { u8 pad[0x1E8]; s32 v; };
struct n64dk_v4 { u8 pad[0x4]; void *v; };
struct gd_E0_s32 { u8 pad[0xE0]; s32 v; };
struct gd_E4_s32 { u8 pad[0xE4]; s32 v; };
struct gd_40_s32 { u8 pad[0x40]; s32 v; };
struct gd_48_s32 { u8 pad[0x48]; s32 v; };
struct gd_4C_s16 { u8 pad[0x4C]; s16 v; };
struct gd_E_s16 { u8 pad[0xE]; s16 v; };
struct gd_9C_f32 { u8 pad[0x9C]; f32 v; };
struct gd_A0_f32 { u8 pad[0xA0]; f32 v; };
struct gd_A4_f32 { u8 pad[0xA4]; f32 v; };
struct gd_A8_f32 { u8 pad[0xA8]; f32 v; };
struct gd_AC_f32 { u8 pad[0xAC]; f32 v; };
struct gd_B0_f32 { u8 pad[0xB0]; f32 v; };
struct n64dk_e24 { u8 b[24]; };
typedef struct { s16 unk_0; u16 unk_2; s16 unk_4; u8 pad6[0xA]; } Vtx16;
typedef struct { u8 pad0[0x40]; } Mtx64;
typedef struct { u8 pad0[0xC]; Vtx16 *unk_C; s32 unk_10; Mtx64 *unk_14; s32 unk_18; u8 pad1C[4]; u16 unk_20; } DrawInfo;
typedef struct { s32 unk_0, unk_4, unk_8; } Unk1A8;
typedef struct { u8 unk_1D4; u8 unk_1D5; } TimerPair;
struct gd_60_f32 { u8 pad[0x60]; f32 v; };
struct gd_8C_s32 { u8 pad[0x8C]; s32 v; };
struct gd_90_s32 { u8 pad[0x90]; s32 v; };
struct gd_B8_s32 { u8 pad[0xB8]; s32 v; };
struct gd_BC_s32 { u8 pad[0xBC]; s32 v; };
struct gd_C0_s32 { u8 pad[0xC0]; s32 v; };
struct gd_C4_s32 { u8 pad[0xC4]; s32 v; };
struct gd_C8_s32 { u8 pad[0xC8]; s32 v; };
struct gd_CC_s32 { u8 pad[0xCC]; s32 v; };
struct gd_D0_s32 { u8 pad[0xD0]; s32 v; };
struct gd_D8_f32 { u8 pad[0xD8]; f32 v; };
struct gd_DC_f32 { u8 pad[0xDC]; f32 v; };
struct gd_E0_f32 { u8 pad[0xE0]; f32 v; };
struct gd_E4_f32 { u8 pad[0xE4]; f32 v; };
struct gd_EC_s32 { u8 pad[0xEC]; s32 v; };
struct gd_F0_s32 { u8 pad[0xF0]; s32 v; };
struct gd_1AC_s32 { u8 pad[0x1AC]; s32 v; };
struct gd_1B4_s32 { u8 pad[0x1B4]; s32 v; };
struct gd_1B8_f32 { u8 pad[0x1B8]; f32 v; };
struct gd_1BC_f32 { u8 pad[0x1BC]; f32 v; };
struct gd_1C0_f32 { u8 pad[0x1C0]; f32 v; };
struct gd_1C4_f32 { u8 pad[0x1C4]; f32 v; };
struct gd_1D4_s32 { u8 pad[0x1D4]; s32 v; };
struct gd_1D8_s32 { u8 pad[0x1D8]; s32 v; };
struct gd_1DC_s32 { u8 pad[0x1DC]; s32 v; };
struct gd_1E0_s32 { u8 pad[0x1E0]; s32 v; };
struct gd_1E4_s32 { u8 pad[0x1E4]; s32 v; };
struct gd_1EC_s32 { u8 pad[0x1EC]; s32 v; };
struct gd_1F0_s8 { u8 pad[0x1F0]; s8 v; };
struct gd_1F4_f32 { u8 pad[0x1F4]; f32 v; };
struct gd_1F8_s32 { u8 pad[0x1F8]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
struct gd_204_s32 { u8 pad[0x204]; s32 v; };
struct gd_214_s32 { u8 pad[0x214]; s32 v; };
struct gd_226_s16 { u8 pad[0x226]; s16 v; };
struct gd_228_s16 { u8 pad[0x228]; s16 v; };
typedef struct { u8 b[0x70]; } Pad70;
typedef struct { u8 b[0x68]; } Pad68;
typedef struct { u8 pad[0x60]; f32 unk_60; } S6888;
typedef struct { u8 pad0[0xFC]; f32 unk_FC; } Unk112E90;
typedef f32 Mtx44[4][4];
struct gd_38_s32 { u8 pad[0x38]; s32 v; };
struct gd_54_s8 { u8 pad[0x54]; s8 v; };
struct gd_54_u8 { u8 pad[0x54]; u8 v; };
typedef struct { u8 c[0x20]; s32 v[32]; } Row;
struct n64dk_f20_s32 { u8 pad[0x20]; s32 m[1]; };
struct gd_54_s32 { u8 pad[0x54]; s32 v; };
struct gd_1B8_s8 { u8 pad[0x1B8]; s8 v; };
struct gd_1B8_u8 { u8 pad[0x1B8]; u8 v; };
struct gd_1BC_s32 { u8 pad[0x1BC]; s32 v; };
struct gd_1C0_s32 { u8 pad[0x1C0]; s32 v; };
struct gd_1C8_s32 { u8 pad[0x1C8]; s32 v; };
struct gd_1CC_s32 { u8 pad[0x1CC]; s32 v; };
struct gd_1D0_f32 { u8 pad[0x1D0]; f32 v; };
struct n64dk_mF8_40 { u8 pad[0xF8]; struct { u8 b[0x40]; } m[1]; };
struct gd_58_f64 { u8 pad[0x58]; f64 v; };
struct gd_1C8_f32 { u8 pad[0x1C8]; f32 v; };
struct gd_1CC_f32 { u8 pad[0x1CC]; f32 v; };
struct V3 { f32 x, y, z; };
typedef struct Mtx40 { s32 m[4][4]; } Mtx40;
struct gd_5C_s32 { u8 pad[0x5C]; s32 v; };
struct gd_A_u16 { u8 pad[0xA]; u16 v; };
struct gd_224_s16 { u8 pad[0x224]; s16 v; };
struct gd_1AC_f32 { u8 pad[0x1AC]; f32 v; };
struct gd_1B4_f32 { u8 pad[0x1B4]; f32 v; };
struct gd_226_u16 { u8 pad[0x226]; u16 v; };
struct gd_B4_f32 { u8 pad[0xB4]; f32 v; };
struct gd_1FC_s32 { u8 pad[0x1FC]; s32 v; };
struct gd_1EE_s8 { u8 pad[0x1EE]; s8 v; };
struct gd_1EF_s8 { u8 pad[0x1EF]; s8 v; };
struct gd_1E4_f32 { u8 pad[0x1E4]; f32 v; };
struct gd_1ED_s8 { u8 pad[0x1ED]; s8 v; };
struct gd_1EC_s8 { u8 pad[0x1EC]; s8 v; };
struct gd_1EC_u8 { u8 pad[0x1EC]; u8 v; };
struct gd_224_u16 { u8 pad[0x224]; u16 v; };
struct gd_1DC_f32 { u8 pad[0x1DC]; f32 v; };
struct gd_1E0_f32 { u8 pad[0x1E0]; f32 v; };
struct gd_1E8_f32 { u8 pad[0x1E8]; f32 v; };
typedef struct { s32 v[4]; } Pad16;
struct gd_1A8_f32 { u8 pad[0x1A8]; f32 v; };
struct gd_1B0_f32 { u8 pad[0x1B0]; f32 v; };
struct gd_1B8_s32 { u8 pad[0x1B8]; s32 v; };
struct gd_1C4_s32 { u8 pad[0x1C4]; s32 v; };
struct gd_1D4_f32 { u8 pad[0x1D4]; f32 v; };
struct gd_1D4_s8 { u8 pad[0x1D4]; s8 v; };
struct gd_1D8_f32 { u8 pad[0x1D8]; f32 v; };
typedef struct { s32 a, b, c; } Triple;
struct gd_118_f32 { u8 pad[0x118]; f32 v; };
struct gd_11C_f32 { u8 pad[0x11C]; f32 v; };
struct gd_120_f32 { u8 pad[0x120]; f32 v; };
struct gd_124_s32 { u8 pad[0x124]; s32 v; };
struct gd_128_s32 { u8 pad[0x128]; s32 v; };
struct gd_134_s32 { u8 pad[0x134]; s32 v; };
struct gd_144_s8 { u8 pad[0x144]; s8 v; };
struct gd_145_s8 { u8 pad[0x145]; s8 v; };
struct gd_154_s32 { u8 pad[0x154]; s32 v; };
struct gd_158_s32 { u8 pad[0x158]; s32 v; };
struct gd_15C_s32 { u8 pad[0x15C]; s32 v; };
struct gd_160_s32 { u8 pad[0x160]; s32 v; };
struct gd_18C_f32 { u8 pad[0x18C]; f32 v; };
struct gd_198_f32 { u8 pad[0x198]; f32 v; };
struct gd_1A0_s32 { u8 pad[0x1A0]; s32 v; };
struct n64dk_m58_40 { u8 pad[0x58]; struct { u8 b[0x40]; } m[1]; };
struct gd_118_f64 { u8 pad[0x118]; f64 v; };
struct gd_120_s32 { u8 pad[0x120]; s32 v; };
struct gd_145_u8 { u8 pad[0x145]; u8 v; };
struct gd_194_f32 { u8 pad[0x194]; f32 v; };
struct gd_1E0_s8 { u8 pad[0x1E0]; s8 v; };
struct n64dk_a801151BC { s32 m0; u8 pad4[0x4]; s32 m8; u8 padC[0x4]; };
struct n64dk_a801151C4 { s32 m0; u8 pad4[0xC]; };
struct n64dk_m10_10 { u8 pad[0x10]; struct { u8 b[0x10]; } m[1]; };
struct n64dk_v0 { void *v; };
struct gd_F0_f32 { u8 pad[0xF0]; f32 v; };
struct gd_FC_f32 { u8 pad[0xFC]; f32 v; };
struct gd_64_f32 { u8 pad[0x64]; f32 v; };
struct gd_68_f32 { u8 pad[0x68]; f32 v; };
struct gd_6C_f32 { u8 pad[0x6C]; f32 v; };
struct gd_70_f32 { u8 pad[0x70]; f32 v; };
struct gd_74_f32 { u8 pad[0x74]; f32 v; };
struct gd_78_f32 { u8 pad[0x78]; f32 v; };
struct gd_7C_f32 { u8 pad[0x7C]; f32 v; };
struct gd_80_f32 { u8 pad[0x80]; f32 v; };
struct gd_84_f32 { u8 pad[0x84]; f32 v; };
struct gd_88_f32 { u8 pad[0x88]; f32 v; };
struct gd_8C_f32 { u8 pad[0x8C]; f32 v; };
struct gd_90_f32 { u8 pad[0x90]; f32 v; };
struct gd_94_f32 { u8 pad[0x94]; f32 v; };
struct gd_98_f32 { u8 pad[0x98]; f32 v; };
struct gd_B8_f32 { u8 pad[0xB8]; f32 v; };
struct gd_BC_f32 { u8 pad[0xBC]; f32 v; };
struct gd_C0_f32 { u8 pad[0xC0]; f32 v; };
struct gd_C4_f32 { u8 pad[0xC4]; f32 v; };
struct gd_C8_f32 { u8 pad[0xC8]; f32 v; };
struct gd_CC_f32 { u8 pad[0xCC]; f32 v; };
struct gd_D0_f32 { u8 pad[0xD0]; f32 v; };
struct gd_D4_f32 { u8 pad[0xD4]; f32 v; };
struct gd_E8_f32 { u8 pad[0xE8]; f32 v; };
struct gd_EC_f32 { u8 pad[0xEC]; f32 v; };
struct n64dk_e4 { u8 b[4]; };
typedef struct { f32 pos[3]; f32 yaw; f32 pitch; f32 unk_14; f32 roll; f32 unk_1C; f32 unk_20; f32 unk_24; } CamState;
typedef struct { u8 unk_00[0x38]; CamState st; } CamRef;
typedef struct { CamRef *ref; s32 unk_04; f32 posLimit; f32 angLimit; f32 rollLimit; } Clamp;
struct gd_78_s32 { u8 pad[0x78]; s32 v; };
struct gd_7C_s32 { u8 pad[0x7C]; s32 v; };
struct gd_80_s32 { u8 pad[0x80]; s32 v; };
struct gd_84_s32 { u8 pad[0x84]; s32 v; };
typedef struct { s16 unk_00; s16 unk_02; s8 unk_04; } ChanCfg;
typedef struct Evt { struct Evt *next; } Evt;
typedef struct { s32 unk_00; s32 unk_04; s32 unk_08; s32 unk_0C; s32 unk_10; s32 unk_14; s32 unk_18; s32 unk_1C; s32 unk_20; s32 unk_24; u8 pad28[0x18]; s32 unk_40; } Config;
typedef struct SeqPlayer { u8 unk00[4]; u8 *seq; u8 unk08[4]; s32 time; u8 unk10[0x24]; s32 unk34; s32 unk38; s32 unk3C; s32 unk40; s32 handle; u8 unk48[0x2C]; s32 id; u8 unk78[0x22]; u16 delta; u8 unk9C[0xE]; u16 unkAA; u16 fixedDelta; u8 unkAE[9]; u8 unkB7; u8 unkB8[3]; u8 unkBB; u8 unkBC[0x16]; u8 unkD2; u8 unkD3; u8 unkD4[0x68]; } SeqPlayer;
typedef u8 *(*SeqCmdFunc)(SeqPlayer *p, u8 *s);
typedef struct { u8 pad[0x10]; s32 unk_10; } Snd;
typedef struct { u8 pad[0x14]; s32 unk_14; } SubB;
typedef struct { u8 pad[0x20]; s32 unk_20; } SubA;
typedef struct { s32 off; s32 unk_4; } RelEnt;
typedef struct { s32 count; s32 unk_4; s32 unk_8; s32 flags; s32 unk_10; s32 off_14; RelEnt ent[1]; } RelHdr;
struct Struct_func_8004EFC4_arg0;
struct Struct_func_8004EFC4_arg0 { u8 unk_0; char unk_1[0x3]; s32 unk_4; };
typedef struct { u8 pad[8]; } Event;
typedef struct { u8 pad00[0x14]; u16 *unk_14; } Bank2;
typedef struct { u16 unk_00; u16 unk_02; u8 unk_04; u8 unk_05; } NoteMap;
typedef struct { u8 b[0x1C]; } Syn;
struct Struct_anon_29;
struct Struct_anon_31;
struct Struct_func_8004F3CC_arg0;
struct Struct_anon_29 { char unk_0[0x18]; s32 unk_18; char unk_1C[0x4]; s32 unk_20; };
struct Struct_anon_31 { char unk_0[0x28]; s32 unk_28; s32 unk_2C; };
struct Struct_func_8004F3CC_arg0 { char unk_0[0x4]; s32 unk_4; s32 unk_8; s32 unk_C; char unk_10[0x1C]; f32 unk_2C; char unk_30[0xC]; s32 unk_3C; s32 unk_40; char unk_44[0x8]; f32 unk_4C; f32 unk_50; s32 unk_54; f32 unk_58; f32 unk_5C; f32 unk_60; s32 unk_64; char unk_68[0xC]; struct Struct_anon_29 *unk_74; struct Struct_anon_30 *unk_78; struct Struct_anon_31 *unk_7C; char unk_80[0x4]; s32 unk_84; char unk_88[0xC]; s32 unk_94; char unk_98[0x2]; u16 unk_9A; char unk_9C[0x4]; u16 unk_A0; char unk_A2[0x8]; u16 unk_AA; u16 unk_AC; u16 unk_AE; char unk_B0[0x2]; u16 unk_B2; u16 unk_B4; char unk_B6[0x1]; u8 unk_B7; char unk_B8[0x1]; u8 unk_B9; u8 unk_BA; u8 unk_BB; char unk_BC[0x1]; u8 unk_BD; char unk_BE[0x1]; u8 unk_BF; u8 unk_C0; u8 unk_C1; u8 unk_C2; u8 unk_C3; u8 unk_C4; u8 unk_C5; u8 unk_C6; u8 unk_C7; u8 unk_C8; u8 unk_C9; u8 unk_CA; u8 unk_CB; u8 unk_CC; u8 unk_CD; char unk_CE[0x1]; u8 unk_CF; u8 unk_D0; u8 unk_D1; u8 unk_D2; u8 unk_D3; u8 unk_D4; char unk_D5[0x1]; u8 unk_D6; u8 unk_D7; char unk_D8[0x1]; u8 unk_D9; u8 unk_DA; };
typedef struct { u8 pad[0xCE]; u8 unk_CE; u8 unk_CF; u8 unk_D0; s8 unk_D1; u8 pad2[6]; u8 unk_D8; } Blink;
typedef struct { u8 unk_00[0xC]; s32 unk_0C; u8 unk_10[4]; s32 unk_14; u8 unk_18[0x38 - 0x18]; u8 *unk_38; u8 unk_3C[0xA2 - 0x3C]; u16 unk_A2; u8 unk_A4[0xBC - 0xA4]; u8 unk_BC; } Seq;
typedef union { u8 b; f32 f; } Num;
typedef struct { s32 unk_00; s32 unk_04; } Prog;
typedef struct { u8 pad0[4]; s32 unk_4; u8 pad8[4]; s32 *unk_C[7]; s32 unk_28; } Song;
struct Fields_var_var_v1_9 { s32 unk_0; char pad_4[0x40]; s32 unk_44; };
struct Struct_func_80050DD0_arg0;
struct Struct_func_80050DD0_arg0 { char unk_0[0x8]; s32 unk_8; char unk_C[0x28]; s32 unk_34; s32 unk_38; char unk_3C[0x8]; s32 unk_44; char unk_48[0x2C]; s32 unk_74; s32 unk_78; };
struct Struct_func_80051174_arg0;
struct Struct_func_80051174_arg0 { char unk_0[0x68]; s32 unk_68; char unk_6C[0x69]; u8 unk_D5; };
typedef struct { u8 b[6]; } E6;
typedef struct { s32 unk_00; void *handler; u8 pad[0x14 - 8]; } N_ALSave;
typedef struct { u8 unk_00[0x20]; s32 unk_20; } Arg1;
typedef struct { u8 pad[0x14]; void *state; u8 pad18[8]; f32 delta; s32 first; } Resampler;
typedef struct { s16 fc; u8 pad2[0x28 - 2]; void *fstate; } LowPass;
typedef struct { void *obj; void *state; } Prealloc;
typedef struct DmaBuf { struct DmaBuf *prev; struct DmaBuf *next; s32 lastFrame; u32 startAddr; void *ptr; } DmaBuf;
typedef struct { u16 type; u8 pri; u8 status; void *retQueue; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } IoMesg;
typedef struct { u8 pad[8]; u8 mq[1]; } Sys;
typedef struct { s32 unk_0, unk_4, unk_8, unk_C; } Arg;
typedef struct { s32 unk_00, unk_04, unk_08, unk_0C, unk_10, unk_14; u8 *unk_18; s32 unk_1C, unk_20, unk_24, unk_28, unk_2C; s32 unk_30, unk_34, unk_38, unk_3C, unk_40, unk_44, unk_48, unk_4C; s32 unk_50; void *unk_54; s32 unk_58, unk_5C, unk_60, unk_64; } Task;
typedef struct { s32 unk_0; s32 unk_4; s32 unk_8; s32 unk_C; s32 unk_10; s32 unk_14; s32 unk_18; u8 unk_1C; u8 pad1D[0xB]; } Cfg;
typedef struct { s32 unk_0; s32 unk_4; u8 pad8[0x4]; s32 unk_C; u8 pad10[0x18]; s32 unk_28; s32 unk_2C; s32 unk_30; s32 unk_34; s32 unk_38; s32 unk_3C; } Params;
typedef struct { u8 unk_00[0x18]; } Mtx18;
typedef struct { u8 unk_00[0x8]; } Buf8;
typedef struct { u8 pad0[0x14]; s32 unk_14; u8 pad18[4]; void **unk_1C; } S34;
typedef struct { u32 w0; u32 w1; } Acwords;
struct Fields_global_D_80077BC4 { void *unk_0; char pad_4[0x1c]; s32 unk_20; };
typedef struct Info { u8 unk_00[0x16]; s16 unk_16; } Info;
typedef struct { u8 pad[0x1C]; s32 unk_1C; } Glob;
typedef struct { s32 unk_00; s32 unk_04; s16 unk_08; s16 pad0A; s32 unk_0C; s32 unk_10; } Ev;
typedef struct { s32 unk_00; s32 unk_04; f32 unk_08_dummy; } Dummy;
typedef struct { u8 pad[0x88]; s32 unk_88; } Tgt;
typedef struct { u8 pad[0x88]; s32 unk_88; } Res;
typedef struct { u8 pad[0x1C]; s32 unk_1C; } Mgr;
typedef struct { void *source; s32 (*handler)(); } N_ALFilter;
typedef struct { u8 pad[0x40]; s32 unk_40; } S800773B4;
typedef float Matrix[4][4];
#ifndef _ULTRA64_H_
typedef long Mtx_t[4][4];
#endif
typedef struct { u32 inst1, inst2, inst3, inst4; } __osExceptionVector;
#ifndef _ULTRA64_H_
typedef void *ALFxRef;
#endif
#ifndef _ULTRA64_H_
typedef void *OSMesg;
#endif
#ifndef _ULTRA64_H_
typedef u64 OSTime;
#endif
#ifndef _ULTRA64_H_
typedef struct { u16 *histo_base; u32 histo_size; u32 *text_start; u32 *text_end; } OSProf;
#endif
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef s32 (*ALSetFXParam)(void *, s32, void *);
#ifndef _ULTRA64_H_
typedef short POLEF_STATE[4];
#endif
typedef s32 (*ALFilterHandler)(void *, s16 *, s32, s32, void *);
typedef s32 (*DmaFn)(s32 addr, s32 len, void *state);
#ifndef _ULTRA64_H_
typedef u8 ALPan;
#endif
struct ALFilter_s;
typedef struct { f64 v[8]; } Log2Table;
struct Struct_func_8005648C_arg0;
struct Struct_func_8005648C_arg0 { s32 unk_0; s32 unk_4; };
struct Struct_func_8005D068_arg0;
struct Struct_func_8005D068_arg0 { u16 unk_0; u16 unk_2; };
#ifndef _ULTRA64_H_
typedef struct { short ob[3]; unsigned short flag; short tc[2]; signed char n[3]; unsigned char a; } Vtx_tn;
#endif
#ifndef _ULTRA64_H_
typedef struct { void *SourceImagePointer; void *TlutPointer; short Stride; short SubImageWidth; short SubImageHeight; char SourceImageType; char SourceImageBitSize; short SourceImageOffsetS; short SourceImageOffsetT; char dummy[4]; } uSprite_t;
#endif
#ifndef _ULTRA64_H_
typedef union { uSprite_t s; long long int force_structure_allignment[3]; } uSprite;
#endif
#ifndef _ULTRA64_H_
typedef struct { unsigned char flag; unsigned char v[3]; } Tri;
#endif
#ifndef _ULTRA64_H_
typedef struct { unsigned char col[3]; char pad1; unsigned char colc[3]; char pad2; } Ambient_t;
#endif
#ifndef _ULTRA64_H_
typedef struct { int x1,y1,x2,y2; } Hilite_t;
#endif
#ifndef _ULTRA64_H_
typedef union { Ambient_t l; long long int force_structure_alignment[1]; } Ambient;
#endif
#ifndef _ULTRA64_H_
typedef union { Hilite_t h; long int force_structure_alignment[4]; } Hilite;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned int par:8; unsigned int len:16; unsigned int addr; } Gdma;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad:24; Tri tri; } Gtri;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad1:24; int pad2:24; unsigned char param:8; } Gpopmtx;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad0:8; int mw_index:8; int number:8; int pad1:8; int base:24; } Gsegment;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad0:8; int sft:8; int len:8; unsigned int data:32; } GsetothermodeL;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad0:8; int sft:8; int len:8; unsigned int data:32; } GsetothermodeH;
#endif
#ifndef _ULTRA64_H_
typedef struct { unsigned char cmd; unsigned char lodscale; unsigned char tile; unsigned char on; unsigned short s; unsigned short t; } Gtexture;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad:24; Tri line; } Gline3D;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int pad1:24; short int pad2; short int scale; } Gperspnorm;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned int fmt:3; unsigned int siz:2; unsigned int pad:7; unsigned int wd:12; unsigned int dram; } Gsetimg;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned int muxs0:24; unsigned int muxs1:32; } Gsetcombine;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned char pad; unsigned char prim_min_level; unsigned char prim_level; unsigned long color; } Gsetcolor;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; int x0:10; int x0frac:2; int y0:10; int y0frac:2; unsigned int pad:8; int x1:10; int x1frac:2; int y1:10; int y1frac:2; } Gfillrect;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned int fmt:3; unsigned int siz:2; unsigned int pad0:1; unsigned int line:9; unsigned int tmem:9; unsigned int pad1:5; unsigned int tile:3; unsigned int palette:4; unsigned int ct:1; unsigned int mt:1; unsigned int maskt:4; unsigned int shiftt:4; unsigned int cs:1; unsigned int ms:1; unsigned int masks:4; unsigned int shifts:4; } Gsettile;
#endif
#ifndef _ULTRA64_H_
typedef struct { int cmd:8; unsigned int sl:12; unsigned int tl:12; int pad:5; unsigned int tile:3; unsigned int sh:12; unsigned int th:12; } Gloadtile;
#endif
#ifndef _ULTRA64_H_
typedef Gloadtile Gloadblock;
#endif
#ifndef _ULTRA64_H_
typedef Gloadtile Gsettilesize;
#endif
#ifndef _ULTRA64_H_
typedef Gloadtile Gloadtlut;
#endif
#ifndef _ULTRA64_H_
typedef struct { unsigned int cmd:8; unsigned int xl:12; unsigned int yl:12; unsigned int pad1:5; unsigned int tile:3; unsigned int xh:12; unsigned int yh:12; unsigned int s:16; unsigned int t:16; unsigned int dsdx:16; unsigned int dtdy:16; } Gtexrect;
#endif
#ifndef _ULTRA64_H_
typedef struct { unsigned long w0; unsigned long w1; unsigned long w2; unsigned long w3; } TexRect;
#endif
typedef struct { u32 ramarray[15]; u32 pifstatus; } OSPifRam;
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u16 button; s8 stick_x; s8 stick_y; } __OSContReadFormat;
#ifndef _ULTRA64_H_
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errno; } OSContPad;
#endif
#ifndef _ULTRA64_H_
typedef struct { u16 type; u8 status; u8 errno; } OSContStatus;
#endif
#ifndef _ULTRA64_H_
typedef s32 OSPri;
#endif
#ifndef _ULTRA64_H_
typedef struct { u32 errStatus; void *dramAddr; void *C2Addr; u32 sectorSize; u32 C1ErrNum; u32 C1ErrSector[4]; } __OSBlockInfo;
#endif
typedef char *va_list;
#ifndef _ULTRA64_H_
typedef u32 OSIntMask;
#endif
typedef struct { u32 type; } OSTaskHdr;
typedef struct { f32 factor; u16 offset; u32 scale; } __OSViScale;
struct Fields_global_D_80078064 { s16 unk_0; char pad_2[0x6]; struct Fields_member_global_D_80078064_8 *unk_8; s32 unk_C; };
struct Fields_member_global_D_80078064_8 { char pad_0[0x4]; s32 unk_4; };
typedef int TVid;
typedef struct { u32 *breakAddress; u32 oldInstruction; } BREAKINFO;
typedef struct { unsigned int addr; short major; short minor; } KKFaultInfo;
typedef struct { s32 length; u8 code; u8 pad5; s16 unk_06; s32 unk_08; s32 unk_0C; s16 unk_10; s16 unk_12; s32 unk_14; s32 unk_18; s32 unk_1C; s32 unk_20; s16 unk_24; s16 unk_26; s32 unk_28; s32 unk_2C; s32 unk_30; s32 unk_34; s32 unk_38; s32 unk_3C; s32 unk_40; s32 unk_44; s32 unk_48; } Stopped;
typedef struct { unsigned int vaddr; unsigned int size; short flags; unsigned int paddr; } KKRegion;
typedef struct { u32 gregs[37]; } KKGregSet;
#ifndef _ULTRA64_H_
typedef struct { u64 at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, gp, sp, s8, ra, lo, hi; u32 sr, pc, cause, badvaddr, rcp, fpcsr; } __OSThreadContext;
#endif
typedef struct { u32 regs[32]; u32 fpcsr; u32 fir; } KKFPregSet;
typedef struct { u16 number; s32 objects[1]; } KKObjs;
typedef struct { u8 pad0[4]; u8 unk_4; u8 pad5; u16 unk_6; u8 rest[0x48]; } Block;
typedef struct { unsigned int flags; unsigned int vaddr; } KKRun;
#ifndef _ULTRA64_H_
typedef unsigned int size_t;
#endif
typedef struct { long quot; long rem; } ldiv_t;
struct exception { s32 type; char *name; f64 arg1; f64 arg2; f64 retval; };
typedef struct { u8 pad[0x166]; s16 unk_166; u8 pad1[0x190 - 0x168]; s32 unk_190, unk_194, unk_198, unk_19C; u8 pad2[0x11A0 - 0x1A0]; u16 unk_11A0; s16 unk_11A2; u32 unk_11A4; s32 unk_11A8; u8 unk_11AC[0x18]; u8 unk_11C4[4]; s32 unk_11C8, unk_11CC, unk_11D0, unk_11D4, unk_11D8; u8 unk_11DC, unk_11DD, unk_11DE; } State;
struct Stream { u8 pad0[0x190]; void (*unk_190)(u8 *src, void *dst, s32 len, void *mq); u8 *unk_194; u8 *unk_198; u32 unk_19C; u32 unk_1A4_pad; u32 unk_1A4; u32 unk_1A8; u8 pad1AC[0x11A0 - 0x1AC]; u16 unk_11A0; u16 unk_11A2; u32 unk_11A4; u32 unk_11A8; u8 unk_11AC[0x11C8 - 0x11AC]; u32 unk_11C8; u32 unk_11CC; u32 unk_11D0; u32 unk_11D4; u32 unk_11D8; s8 unk_11DC; s8 unk_11DD; s8 unk_11DE; };
typedef struct { s16 v[8]; } Coefs;
typedef struct { u8 unk_00[0x18A]; s16 unk_18A; s16 unk_18C; u8 unk_18E[0x1A0 - 0x18E]; u32 buf[0x400]; u8 unk_11A0[0x11C8 - 0x11A0]; s32 bitpos; } GsmDec;

#endif
