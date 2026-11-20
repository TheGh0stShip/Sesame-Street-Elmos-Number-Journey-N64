#include "common.h"
#include "game_structs.h"


typedef struct Ray {
    f32 unk_0;
    Vec3f unk_4;
    Vec3f unk_10;
    Vec3f unk_1C;
} Ray;

typedef struct Plane {
    u8 pad0[0x10];
    Vec3f normal; /* 0x10 */
    f32 dist;      /* 0x1C */
    f32 unk_20;
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4C;
} Plane;

#define DOT(u, v) ((u)->x * (v)->x + (u)->y * (v)->y + (u)->z * (v)->z)
#define ABS(x) ((x) >= 0.0f ? (x) : -(x))

/* Tests a ray against a plane and, if it crosses, runs the two triangle checks. */
void func_80042880(Ray *ray, Plane *pl) {
    f32 d;
    f32 e;
    f32 g;

    d = (ABS(DOT(&pl->normal, &ray->unk_1C)) > 1e-7f) ? DOT(&pl->normal, &ray->unk_1C) : 0.0f;
    if (d >= 0.0f) {
        return;
    }
    e = DOT(&pl->normal, &ray->unk_10) - pl->dist;
    if (ray->unk_0 - e < 0.001f) {
        return;
    }
    g = DOT(&pl->normal, &ray->unk_4) - pl->dist;
    if (g < 0.0f) {
        return;
    }
    func_800443A0(ray, pl, &pl->normal, e, g, pl->unk_20, pl->unk_24, pl->unk_28, pl->unk_2C, pl->unk_30,
                  pl->unk_34, pl->unk_44, pl->unk_48, pl->unk_4C);
    func_800443A0(ray, pl, &pl->normal, e, g, pl->unk_44, pl->unk_48, pl->unk_4C, pl->unk_38, pl->unk_3C,
                  pl->unk_40, pl->unk_20, pl->unk_24, pl->unk_28);
}
