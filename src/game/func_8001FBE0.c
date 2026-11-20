#include "common.h"

s32 func_80023A38();
s32 func_8003D3D0();

void func_8001FBE0(s32 arg0) {
    s32 sp10;
    s32 sp14;
    s32 pad0;
    s32 sp1C;

    sp10 = 0;
    sp14 = arg0 + (sp10 * 4);
    while (!((*(s32 *)sp14) == 0)) {
        sp1C = *(s32 *)sp14;
        switch (*(s32 *)sp1C) {
        case 19:
            func_80033368(sp1C);
            break;
        case 1:
            func_80021A20(sp1C);
            break;
        case 4:
            func_80026110(sp1C);
            break;
        case 5:
            func_8002C800(sp1C);
            break;
        case 7:
            func_80023A38(sp1C);
            break;
        case 8:
            func_800347CC(sp1C);
            break;
        case 9:
            func_80023594(sp1C);
            break;
        case 10:
            func_80036A90(sp1C);
            break;
        case 12:
            func_8003D3D0(sp1C);
            break;
        case 13:
            func_80027F2C(sp1C);
            break;
        case 11:
            func_800408D0(sp1C);
            break;
        case 14:
            func_80027F68(sp1C);
            break;
        case 15:
            func_80027FA4(sp1C);
            break;
        case 18:
            ((struct gd_4_s32 *)sp1C)->v = 0x6000;
            func_800408D0(sp1C);
            break;
        }
        sp10++;
        sp14 = arg0 + (sp10 * 4);
    }
    return;
}
