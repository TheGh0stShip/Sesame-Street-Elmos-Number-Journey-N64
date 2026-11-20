#include "common.h"

/* half the XZ distance between two points of the object's position block */

typedef struct { u8 pad[0x24]; Pts *unk_24; } Obj;
f32 func_800239A0(Obj *arg0) {
    register Pts *p;
    s32 unused;
    register f32 dx;
    register f32 dz;
    p = arg0->unk_24;
    dx = p->x0 - p->x1;
    dz = p->z0 - p->z1;
    dx = dx * dx;
    dz = dz * dz;
    dx = dx + dz;
    dz = sqrtf(dx);
    dx = dz * 0.5;
    return dx;
}
