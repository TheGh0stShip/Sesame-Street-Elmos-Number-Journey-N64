#include "common.h"
#include "game_structs.h"


typedef struct {
    s32 count;
    Vec3f *points;
} Path;

/* Catmull-Rom interpolation along a path at parameter t */
void func_80049180(Path *path, Vec3f *out, f32 t) {
    s32 i;
    s32 idx;
    s32 base;
    s32 last;
    f32 frac;
    f32 a;
    f32 b;
    f32 c;
    Vec3f *p[4];

    base = t;
    last = path->count - 1;
    frac = t - base;
    for (i = 0; i < 4; i++) {
        idx = base + i - 1;
        if (idx < 0) {
            idx = 0;
        } else if (idx > last) {
            idx = last;
        }
        p[i] = &path->points[idx];
    }
    a = -p[0]->x + p[1]->x * 3.0f - p[2]->x * 3.0f + p[3]->x;
    b = 2.0f * p[0]->x - p[1]->x * 5.0f + p[2]->x * 4.0f - p[3]->x;
    c = -p[0]->x + p[2]->x;
    out->x = ((a * frac + b) * frac + c) * frac * 0.5f + p[1]->x;
    a = -p[0]->y + p[1]->y * 3.0f - p[2]->y * 3.0f + p[3]->y;
    b = 2.0f * p[0]->y - p[1]->y * 5.0f + p[2]->y * 4.0f - p[3]->y;
    c = -p[0]->y + p[2]->y;
    out->y = ((a * frac + b) * frac + c) * frac * 0.5f + p[1]->y;
    a = -p[0]->z + p[1]->z * 3.0f - p[2]->z * 3.0f + p[3]->z;
    b = 2.0f * p[0]->z - p[1]->z * 5.0f + p[2]->z * 4.0f - p[3]->z;
    c = -p[0]->z + p[2]->z;
    out->z = ((a * frac + b) * frac + c) * frac * 0.5f + p[1]->z;
}
