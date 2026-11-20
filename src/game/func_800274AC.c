#include "common.h"

typedef struct Obj {
    u8 pad0[0xC];
    s32 unk_0C;
    u8 pad10[0x24 - 0x10];
    s32 unk_24;
    u8 pad28[0x40 - 0x28];
    s32 unk_40;
    u8 pad44[0x48 - 0x44];
    s32 unk_48;
    s16 unk_4C;
} Obj;
extern u16 D_80107CBA;

/* Per-frame ambient sound/timer update for an object depending on game state. */
void func_800274AC(Obj *arg0) {
    Obj *sp18;
    s32 pad0;
    s8 pad1;

    sp18 = arg0;
    if (D_800AD03A == 0) {
        if ((*(u8 (*))&D_8012F150) != 0) {
            return;
        }
        switch (D_8012F154) {
        case 1:
            if ((--sp18->unk_4C) < 0) {
                sp18->unk_48 = func_800179D8(D_8007682C[func_800217C4(5)]);
                sp18->unk_4C = func_800217C4(180) + 60;
            }
            break;
        case 11:
            if (D_80076B60 != 1) {
                if ((--sp18->unk_4C) < 0) {
                    sp18->unk_48 = func_800179D8(43);
                    sp18->unk_4C = func_800217C4(180) + 60;
                }
            }
            break;
        case 9:
            if ((--sp18->unk_4C) < 0) {
                sp18->unk_48 = func_800179D8(D_80076848[func_800217C4(3)]);
                sp18->unk_4C = func_800217C4(180) + 60;
            }
            break;
        case 10:
            if (((D_80107CBA & 0x4000) & 0xFFFF) != 0) {
                if (func_8004E938(sp18->unk_40) == 0) {
                    sp18->unk_40 = func_800179D8(21);
                }
            }
            break;
        case 8:
            if ((--sp18->unk_4C) < 0) {
                sp18->unk_48 = func_800179D8(D_80076840[func_800217C4(2)]);
                sp18->unk_4C = func_800217C4(180) + 60;
            }
            break;
        case 7:
            break;
        case 6:
            break;
        case 5:
            break;
        }
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            sp18->unk_0C = sp18->unk_0C | 1;
        }
    } else {
        if (D_80109D4C == 0) {
            if (sp18->unk_24 == 0) {
            } else if (sp18->unk_24 >= 16) {
                D_80109D4C = 1;
            }
        }
        addActorDL(D_80027838, 0, 3, 0, 0);
        sp18->unk_24++;
    }
}
