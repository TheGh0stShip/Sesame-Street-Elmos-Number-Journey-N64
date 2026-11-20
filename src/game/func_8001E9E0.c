#include "common.h"
/* segment: compute direction from start to end, normalise it; clamp the length to a limit and move the end point; -1 if zero length */


s32 func_8001E9E0(Seg *s)
{
    f32 inv;
    f32 limit;

    limit = s->len - s->limitBase;
    s->dir[0] = s->end[0] - s->start[0];
    s->dir[1] = s->end[1] - s->start[1];
    s->dir[2] = s->end[2] - s->start[2];
    s->len = sqrtf(s->dir[0] * s->dir[0] + s->dir[1] * s->dir[1] + s->dir[2] * s->dir[2]);
    if (s->len <= 0.0f) {
        return -1;
    }
    inv = 1.0f / s->len;
    s->dir[0] = s->dir[0] * inv;
    s->dir[1] = s->dir[1] * inv;
    s->dir[2] = s->dir[2] * inv;
    if (s->len > limit) {
        s->len = limit;
        s->end[0] = s->start[0] + s->dir[0] * limit;
        s->end[1] = s->start[1] + s->dir[1] * limit;
        s->end[2] = s->start[2] + s->dir[2] * limit;
    }
    return 0;
}
