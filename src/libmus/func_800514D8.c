#include "common.h"
#include "libmus/player.h"

u8 *FrandNote(channel_t *p, u8 *ptr) {
    p->transpose = func_800504E8(*ptr++);
    p->transpose += *ptr++;
    return ptr;
}
