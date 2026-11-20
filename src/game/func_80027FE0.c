#include "common.h"

typedef struct Src {
    u8 pad0[0x2];
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    s32 unk_08;
    f32 unk_0C;
    f32 unk_10;
    f32 unk_14;
    f32 unk_18;
    f32 unk_1C;
    f32 unk_20;
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
} Src;

typedef struct Obj {
    u8 pad0[0x8];
    s16 unk_08;
    s16 unk_0A;
    s32 unk_0C;
    void *unk_10;
    void *unk_14;
    s32 unk_18;
    u8 pad1C[0x24 - 0x1C];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4C;
    f32 unk_50;
    s32 unk_54;
    u8 pad58[0x1A8 - 0x58];
    f32 unk_1A8;
    f32 unk_1AC;
    f32 unk_1B0;
    s32 unk_1B4;
    s32 unk_1B8;
    s32 unk_1BC;
    u8 pad1C0[0x1C4 - 0x1C0];
    s32 unk_1C4;
    u8 pad1C8[0x1D4 - 0x1C8];
    s32 unk_1D4;
    union { s32 i; f32 f; } unk_1D8;
    union { s32 i; f32 f; } unk_1DC;
    union { s32 i; f32 f; s8 b; } unk_1E0;
    u8 pad1E4[0x1E8 - 0x1E4];
    void *unk_1E8;
    u8 pad1EC[0x218 - 0x1EC];
} Obj;
extern Obj *D_80106C84;

/* Allocates and initialises an object from a spawn record, per level. */
void func_80027FE0(Src *arg0, s32 arg1, Src *arg2) {
    register Obj *reg_s0;
    register Src *reg_s1;
    register Src *reg_s2;
    void *sp10;
    Src *sp14;
    f32 sp18;
    f64 sp20;

    sp10 = NULL;
    sp14 = NULL;
    reg_s1 = arg0;
    reg_s2 = arg0;
    reg_s0 = func_80042080(536);
    switch ((s32)reg_s0) {
    case 0:
        func_80059540(D_8007A8C0, D_8007A8C4, 324);
    }
    if (reg_s0 == 0) {
        return;
    }
    func_80069C70(reg_s0, 0, 536);
    reg_s0->unk_10 = D_800287AC;
    reg_s0->unk_14 = D_8002C3D0;
    reg_s0->unk_18 = 0;
    reg_s0->unk_08 = reg_s2->unk_02;
    reg_s0->unk_0A = reg_s2->unk_06;
    reg_s0->unk_0C = reg_s2->unk_08;
    sp14 = arg2;
    sp10 = func_800189A0(sp14, sp14->unk_04);
    switch ((s32)sp10) {
    case 0:
        func_80059540(D_8007A8C0, D_8007A8C4, 341);
    }
    reg_s0->unk_1C4 = 0;
    reg_s0->unk_1BC = 0;
    reg_s0->unk_1E8 = sp10;
    reg_s0->unk_1D4 = 0;
    reg_s0->unk_1D8.i = 0;
    reg_s0->unk_1DC.i = 0;
    reg_s0->unk_1E0.i = 0;
    reg_s0->unk_54 = 0;
    if (((u32)D_800A7F24 >= 49) && ((u32)D_800A7F24 < 58)) {
        D_8012F110 = D_800A7F24 - 49;
    } else {
        if (((u32)D_800A7F24 >= 65) && ((u32)D_800A7F24 < 91)) {
            D_8012F110 = D_800A7F24 - 65;
        } else {
            if (((u32)D_800A7F24 >= 97) && ((u32)D_800A7F24 < 123)) {
                D_8012F110 = D_800A7F24 - 97;
            } else {
                D_8012F110 = 0;
            }
        }
    }
    switch (D_8012F154) {
    case 1:
        reg_s0->unk_1BC = D_80076870[arg1];
        reg_s0->unk_0C = reg_s0->unk_0C & -129;
        reg_s0->unk_1B4 = D_8007687C[arg1];
        reg_s0->unk_1B8 = 0;
        break;
    case 9:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_00637780;
        D_800A7A50 = (s32)D_006482F0 - (s32)D_00637780;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 18;
        break;
    case 10:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_006505B0;
        D_800A7A50 = (s32)D_006634C0 - (s32)D_006505B0;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 15;
        break;
    case 7:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_006D25E0;
        D_800A7A50 = (s32)D_006E3450 - (s32)D_006D25E0;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 36;
        break;
    case 8:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_00701190;
        D_800A7A50 = (s32)D_00712000 - (s32)D_00701190;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 40;
        break;
    case 5:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_0078E0B0;
        D_800A7A50 = (s32)D_0079CD90 - (s32)D_0078E0B0;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 59;
        break;
    case 6:
        reg_s0->unk_1BC = 0;
        D_80106C74 = (s32)D_007BB850;
        D_800A7A50 = (s32)D_007C57E0 - (s32)D_007BB850;
        osSendMesg(D_80140F30, 0, 1);
        reg_s0->unk_1B4 = 63;
        break;
    case 11:
        reg_s0->unk_1B4 = D_80076888[arg1];
        reg_s0->unk_1B8 = 0;
        reg_s0->unk_1E0.b = 1;
        if (D_80109DF4 < 0) {
            D_80109DF4 = 1;
        }
        if (arg1 == D_80109DF4) {
            D_80109D54 = D_800A7A5C | (1 << D_80109DF4);
            D_80106C84->unk_1D8.f = reg_s2->unk_0C;
            D_80106C84->unk_1DC.f = 5e+02f;
            D_80106C84->unk_1E0.f = reg_s2->unk_14;
        }
        break;
    }
    reg_s0->unk_1A8 = 1.0f;
    reg_s0->unk_1AC = 1.0f;
    reg_s0->unk_1B0 = 1.0f;
    reg_s0->unk_24 = reg_s2->unk_0C;
    reg_s0->unk_28 = reg_s2->unk_10;
    reg_s0->unk_2C = reg_s2->unk_14;
    D_80112E94 = reg_s2->unk_0C;
    D_80109DF0 = reg_s2->unk_14;
    reg_s0->unk_30 = reg_s2->unk_18;
    reg_s0->unk_34 = reg_s2->unk_1C;
    reg_s0->unk_38 = reg_s2->unk_20;
    sp18 = sqrtf((((reg_s0->unk_30 * reg_s0->unk_30) + (reg_s0->unk_34 * reg_s0->unk_34)) + (reg_s0->unk_38 * reg_s0->unk_38)));
    if (sp18 >= 1e-07f) {
        sp20 = D_8007A908 / ((f64)sp18);
        reg_s0->unk_30 = (f32)(((f64)reg_s0->unk_30) * sp20);
        reg_s0->unk_34 = (f32)(((f64)reg_s0->unk_34) * sp20);
        reg_s0->unk_38 = (f32)(((f64)reg_s0->unk_38) * sp20);
    } else {
    }
    reg_s0->unk_3C = reg_s2->unk_24;
    reg_s0->unk_40 = reg_s2->unk_28;
    reg_s0->unk_44 = reg_s2->unk_2C;
    sp18 = sqrtf((((reg_s0->unk_3C * reg_s0->unk_3C) + (reg_s0->unk_40 * reg_s0->unk_40)) + (reg_s0->unk_44 * reg_s0->unk_44)));
    if (sp18 >= 1e-07f) {
        sp20 = D_8007A910 / ((f64)sp18);
        reg_s0->unk_3C = (f32)(((f64)reg_s0->unk_3C) * sp20);
        reg_s0->unk_40 = (f32)(((f64)reg_s0->unk_40) * sp20);
        reg_s0->unk_44 = (f32)(((f64)reg_s0->unk_44) * sp20);
    } else {
    }
    reg_s0->unk_48 = (reg_s0->unk_34 * reg_s0->unk_44) - (reg_s0->unk_38 * reg_s0->unk_40);
    reg_s0->unk_4C = (reg_s0->unk_38 * reg_s0->unk_3C) - (reg_s0->unk_30 * reg_s0->unk_44);
    reg_s0->unk_50 = (reg_s0->unk_30 * reg_s0->unk_40) - (reg_s0->unk_34 * reg_s0->unk_3C);
    func_80041A80(reg_s0, D_80115408, 4, 0);
}
