#include "common.h"

typedef struct {
    u8 *next_in;
    s32 avail_in;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
} Stream;

/* feeds data from src in 16KB chunks (skipping a 9-byte header and name) to a stream */
s32 func_800139D0(Stream *s, u8 *src, s32 size) {
    s32 done;
    s32 chunk;
    s32 len;
    s32 left;
    s32 pos;
    s16 skip;

    done = 0;
    left = size;
    pos = 0;
    skip = 9;
    while (s->unk_10 != 0) {
        if (s->avail_in == 0 && done == 0) {
            chunk = (left > 0x4000) ? 0x4000 : left;
            if (chunk != 0) {
                len = (chunk + 7) & ~7;
                func_8000C588(src + pos, D_8012FED0, len);
                left -= chunk;
                pos += chunk;
                s->avail_in = chunk;
            } else {
                s->avail_in = 0;
                done = 1;
            }
            if (skip != 0) {
                while (D_8012FED0[++skip] != 0) {
                }
                s->next_in = D_8012FED0 + skip + 1;
                s->avail_in = s->avail_in - (1 + skip);
                skip = 0;
            } else {
                s->next_in = D_8012FED0;
            }
        }
        if (func_80013F48(s->unk_18, s, -5) != 0) {
            break;
        }
        if (done != 0) {
            break;
        }
    }
    return s->unk_14;
}
