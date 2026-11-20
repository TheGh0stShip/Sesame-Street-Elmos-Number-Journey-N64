#include "common.h"
#include "game_structs.h"

/* sets two fields of an object, clears 0x1CC, sets flag 0x80 and clears flag 0x8 */
void func_8002C7A4(register Actor *o, s32 a, s32 b) {
    o->u.model.unk_1B4 = b;
    o->u.model.animTick = 0;
    o->u.model.anim = a;
    o->unk_C.l1.flags |= 0x80;
    o->unk_C.l1.flags &= ~8;
}
