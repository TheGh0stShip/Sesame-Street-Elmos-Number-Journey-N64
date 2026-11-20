#include "common.h"


void func_8001933C(s32 arg0, s32 arg1, register s32 arg2, register s32 arg3) {
    u16 sp10;
    u16 sp12;

    sp10 = arg2;
    sp12 = arg3;
    D_80133EF0 = func_80042080((((s32)&D_003D0980) - ((s32)&D_003CB880)));
    func_80017788(((s32)&D_003CB880), D_80133EF0, (((s32)&D_003D0980) - ((s32)&D_003CB880)));
    D_800AF088 = func_80042080((arg1 - arg0));
    func_80017788(arg0, D_800AF088, (arg1 - arg0));
    ((s32 (*)())func_8004EBE4)(D_80133EF0, ((s32)&D_003D0980));
    func_8004EC20(D_80133EF0);
    D_800A7F6C = ((s32 (*)())MusStartSong)(D_800AF088);
    func_8004EB5C(D_800A7F6C, sp12);
    ((s32 (*)())func_8004E990)(D_800A7F6C, sp10);
}
