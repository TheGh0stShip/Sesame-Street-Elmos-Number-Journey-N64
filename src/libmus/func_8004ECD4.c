#include "common.h"
#include "libmus/player.h"

/* posts a type-0 message carrying one argument */
void MusHandlePause(s32 a) {
    mus_msg_t m;
    m.type = 0;
    m.data = a;
    func_8004F044(&m);
}
