#include "common.h"
#include "libmus/player.h"

/* stores the pointer in a global and posts a type-2 message carrying it */
void MusSetFxType(void *p) {
    mus_msg_t m;
    m.type = 2;
    m.data = p;
    D_800A0574 = p;
    func_8004F044(&m);
}
