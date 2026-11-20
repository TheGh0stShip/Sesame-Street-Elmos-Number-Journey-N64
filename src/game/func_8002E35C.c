#include "common.h"
#include "game_structs.h"

extern S6888 *D_80106C84;

/* pushes the point pos out of the first type 7/8/12 object it overlaps (in x/z); returns that object */
Actor *func_8002E35C(Vec3f *pos) {
    register Actor *obj = D_80115408;
    register f32 px = pos->x;
    register f32 pz = pos->z;
    register f32 ox;
    register f32 oz;
    register f32 dx;
    register f32 dz;
    register f32 extra = D_80106C84->unk_60;
    register f32 r;
    register f32 r2;
    register f32 d2;

    while (obj != NULL) {
        if (obj->unk_8.l1.type == 7 || obj->unk_8.l1.type == 8 || obj->unk_8.l1.type == 12) {
            r = obj->u.prop.radius.x;
            if (r > 0.0f) {
                r = r + extra;
                r2 = r * r;
                ox = obj->pos.x;
                oz = obj->pos.z;
                dx = px - ox;
                dz = pz - oz;
                d2 = dx * dx;
                d2 = d2 + dz * dz;
                if (d2 < r2) {
                    r2 = 1.0 / sqrtf(d2);
                    dx = dx * r2;
                    dz = dz * r2;
                    dx = dx * r;
                    dz = dz * r;
                    pos->x = ox + dx;
                    pos->z = oz + dz;
                    return obj;
                }
            }
        }
        obj = obj->unk_0.l0.next;
    }
    return NULL;
}
