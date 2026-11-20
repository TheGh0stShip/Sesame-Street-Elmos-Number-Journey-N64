#include "common.h"
#include "libmus/player.h"

/* posts a type-1 message carrying one pointer argument (audio player command) */
void MusHandleUnPause(void *p) {
    mus_msg_t m;
    m.type = 1;
    m.data = p;
    func_8004F044(&m);
}
