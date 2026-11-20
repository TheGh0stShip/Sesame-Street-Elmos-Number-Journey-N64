#include "common.h"
#include "codec/stream.h"

/* advance a stream by up to n units; returns amount consumed */
u32 func_8006B1EC(stream_t *s, u32 n) {
    u32 avail;
    u32 left;
    if (s->state == 3) {
        if ((*(s32 *)&s->drain) > 0) {
            (*(s32 *)&s->drain) -= n;
            if ((*(s32 *)&s->drain) <= 0) { s->state = 4; (*(s32 *)&s->drain) = 0; }
        }
        left = s->write_pos - s->play_pos;
        avail = (n < left) ? n : left;
        s->play_pos += avail;
        return avail;
    }
    return 0;
}
