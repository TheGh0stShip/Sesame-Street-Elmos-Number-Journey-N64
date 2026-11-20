#include "common.h"
#include "libmus/player.h"

s32 Fgoto(channel_t *p, u8 *ptr) {
    s32 a, b;
    a = *ptr++ << 8; a += *ptr++;
    b = *ptr++ << 8; b += *ptr++;
    p->volume_count = 1;
    p->pvolume = p->pvolumebase + b;
    b = *ptr++ << 8; b += *ptr++;
    p->pitchbend_count = 1;
    p->ppitchbend = p->ppitchbendbase + b;
    return p->pbase + a;
}
