#include "common.h"
#include "game_structs.h"

/* decrements a countdown */
void func_8003BD24(Actor *p) {
    if (p->u.walker.unk_1ED > 0) {
        if (--p->u.walker.unk_1ED == 0) {
        }
    }
}
