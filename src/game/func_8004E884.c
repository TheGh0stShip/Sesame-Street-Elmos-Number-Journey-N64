#include "common.h"
#include "libmus/player.h"

/* assigns arg1 to idle (-1) objects with id arg0; returns count */
s32 MusHandleStop(s32 arg0, s32 arg1) {
    s32 i;
    s32 n;
    s32 v;
    channel_t *o;
    if (arg0 == 0) return 0;
    v = arg1;
    if (arg1 == 0) v = 1;
    i = 0;
    o = mus_channels;
    n = 0;
    for (; i < max_channels; i++, o++) {
        if (o->handle == arg0 && o->stopping == -1) {
            if (o->flags & 1) {
                o->stopping_speed = 1;
                o->stopping = 0;
                o->flags &= ~1;
            } else {
                o->stopping_speed = v;
                o->stopping = arg1;
            }
            n++;
        }
    }
    return n;
}
