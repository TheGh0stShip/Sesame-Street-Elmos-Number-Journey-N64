#include "common.h"
#include "game_structs.h"


typedef struct Pool {
    s32 unk_0;
    s32 unk_4;
    Arg *unk_8;
    Arg *unk_C;
} Pool;

extern Pool D_80115368[];

/* registers a 16-byte-aligned buffer as the next pool (up to 8) */
void func_80042648(s32 arg0, s32 arg1) {
    Arg *node;

    if ((arg0 & 15) != 0) {
        return;
    }
    if ((*(u32 (*))&D_8013C724) >= 8) {
        return;
    }
    node = (Arg *)arg0;
    node->unk_0 = 0;
    node->unk_4 = 0;
    node->unk_8 = 0;
    node->unk_C = 0;
    D_80115368[(*(u32 (*))&D_8013C724)].unk_8 = node;
    D_80115368[(*(u32 (*))&D_8013C724)].unk_C = node;
    D_80115368[(*(u32 (*))&D_8013C724)].unk_0 = arg1;
    D_80115368[(*(u32 (*))&D_8013C724)].unk_4 = (s32)D_80115368[(*(u32 (*))&D_8013C724)].unk_C + arg1;
    (*(u32 (*))&D_8013C724)++;
}
