#include "common.h"

extern s32 D_80154000;

void func_8001E2CC(s32 arg0) {
    s32 sp10[8];
    s32 sp30;
    s32 sp34;

    func_80013BCC(((s32)&sp10), ((s32)&D_80154000), 0x86800);
    switch (arg0) {
    case 21:
        sp30 = ((s32)&D_0010D560) - ((s32)&D_000A5940);
        sp34 = (s32)&D_000A5940;
        func_80017788(sp34, ((s32)&D_80154000), sp30);
        break;
    case 22:
        sp30 = ((s32)&D_000A5940) - ((s32)&D_0007CB20);
        sp34 = (s32)&D_0007CB20;
        func_80017788(sp34, ((s32)&D_80154000), sp30);
        break;
    case 1:
        sp30 = ((s32)&D_001BCE90) - ((s32)&D_00177680);
        sp34 = (s32)&D_00177680;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 9:
        sp30 = ((s32)&D_001D6CA0) - ((s32)&D_001BCE90);
        sp34 = (s32)&D_001BCE90;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 10:
        sp30 = ((s32)&D_00206CC0) - ((s32)&D_001D6CA0);
        sp34 = (s32)&D_001D6CA0;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 8:
        sp30 = ((s32)&D_0022EC10) - ((s32)&D_00206CC0);
        sp34 = (s32)&D_00206CC0;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 7:
        sp30 = ((s32)&D_0025AF80) - ((s32)&D_0022EC10);
        sp34 = (s32)&D_0022EC10;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 6:
        sp30 = ((s32)&D_0027ADC0) - ((s32)&D_0025AF80);
        sp34 = (s32)&D_0025AF80;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 5:
        sp30 = ((s32)&D_002B8FD0) - ((s32)&D_0027ADC0);
        sp34 = (s32)&D_0027ADC0;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    case 11:
        sp30 = ((s32)&D_002E6FC0) - ((s32)&D_002B8FD0);
        sp34 = (s32)&D_002B8FD0;
        func_800139D0(((s32)&sp10), sp34, sp30);
        break;
    default:
        return;
    }
    sp30 = ((s32)&D_00177680) - ((s32)&D_0010D560);
    func_80017788(((s32)&D_0010D560), ((s32)&D_8024B000), sp30);
    func_8000D3C0();
    func_8000E480(0, 0);
}
