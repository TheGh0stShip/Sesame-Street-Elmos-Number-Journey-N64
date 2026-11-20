#include "common.h"
#include "game_structs.h"

/* Sets fields 0x1B4/0x1BC, clears 0x1CC and flag 0x80. */
void func_8002C758(register Actor *o, s32 a, s32 b) {
    o->u.model.unk_1B4 = b;
    o->u.model.animTick = 0;
    o->u.model.anim = a;
    o->unk_C.l1.flags &= ~0x80;
}
