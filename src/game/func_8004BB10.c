#include "common.h"

typedef struct Src {
    u8 unk_00[0x1C];
    f32 unk_1C;
} Src;

typedef struct Entry {
    Src *unk_00;
    u8 unk_04[0x10];
    f32 unk_14;
    f32 unk_18;
    u8 unk_1C[4];
    f32 unk_20;
    u8 unk_24[4];
} Entry;

typedef struct Path {
    Entry unk_00[1];
    u8 unk_28[8];
    s32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
} Path;

/* allocates and initialises a 76-byte path record */
Path *func_8004BB10(Src *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    register s32 i;
    register Path *p;

    p = func_80042080(76);
    func_80069C70(p, 0, 76);
    p->unk_00[0].unk_00 = arg0;
    i = 0;
    while (!(i > 0)) {
        p->unk_00[i].unk_20 = arg2;
        p->unk_00[i].unk_14 = arg0->unk_1C;
        p->unk_00[i].unk_18 = func_80069920(arg3, arg2);
        i = i + 1;
    }
    p->unk_30 = 0;
    p->unk_34 = arg2;
    p->unk_38 = arg3;
    p->unk_40 = arg4;
    p->unk_44 = arg5;
    p->unk_48 = arg6;
    p->unk_3C = arg1 * arg1;
    D_8012C400 = 0;
    return p;
}
