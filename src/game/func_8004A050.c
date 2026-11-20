#include "common.h"
#include "game_structs.h"


/* builds a direction vector from two angles, a point behind it and the normalised side vector */
void func_8004A050(Actor *arg0) {
    f32 len;
    f64 inv;

    arg0->u.camera.unk_7C.z = 0;
    arg0->u.camera.unk_88 = 1.0f;
    arg0->u.camera.unk_8C = 0;
    arg0->u.camera.unk_70 = ({ -__sinf(arg0->unk_18.l0.dir.z); });
    arg0->u.camera.unk_6C = ({ __cosf(arg0->unk_18.l0.dir.y) * __cosf(arg0->unk_18.l0.dir.z); });
    arg0->u.camera.unk_74 = ({ __sinf(arg0->unk_18.l0.dir.y) * __cosf(arg0->unk_18.l0.dir.z); });
    arg0->u.camera.unk_60.x = arg0->unk_C.l0.flags.y - arg0->u.camera.unk_6C * arg0->pos.y;
    arg0->u.camera.unk_60.y = arg0->unk_C.l0.flags.z - arg0->u.camera.unk_70 * arg0->pos.y;
    arg0->u.camera.unk_60.z = arg0->unk_18.l0.dir.x - arg0->u.camera.unk_74 * arg0->pos.y;
    arg0->u.camera.unk_78 = arg0->u.camera.unk_70 * arg0->u.camera.unk_8C - arg0->u.camera.unk_74 * arg0->u.camera.unk_88;
    arg0->u.camera.unk_7C.x = arg0->u.camera.unk_74 * arg0->u.camera.unk_7C.z - arg0->u.camera.unk_6C * arg0->u.camera.unk_8C;
    arg0->u.camera.unk_7C.y = arg0->u.camera.unk_6C * arg0->u.camera.unk_88 - arg0->u.camera.unk_70 * arg0->u.camera.unk_7C.z;
    len = sqrtf(arg0->u.camera.unk_78 * arg0->u.camera.unk_78 + arg0->u.camera.unk_7C.x * arg0->u.camera.unk_7C.x + arg0->u.camera.unk_7C.y * arg0->u.camera.unk_7C.y);
    if (len >= 1e-07f) {
        inv = D_8007B408 / len;
        arg0->u.camera.unk_78 = arg0->u.camera.unk_78 * inv;
        arg0->u.camera.unk_7C.x = arg0->u.camera.unk_7C.x * inv;
        arg0->u.camera.unk_7C.y = arg0->u.camera.unk_7C.y * inv;
    } else {
    }
}
