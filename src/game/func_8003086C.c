#include "common.h"
#include "game_structs.h"




extern Actor *D_80106C84;/* func_80054D20; must keep the builtin name: call order depends on it */
/* func_8006A080 */
/* func_80069FA0 */

extern s32 func_800208D0(void *, void *, f32 *, f32 *, f32 *, u32 *);
extern Actor *func_8002E35C(f32 *);

#define ABS(x) ((x) >= 0.0f ? (x) : -(x))

/* Actor movement: stick input relative to the camera, turning, gravity, collision and contact with others. */
void func_8003086C(void) {
    f32 ix;
    f32 iy;
    f32 n[3];
    f32 d[3];
    f32 c[3];
    f32 pos[3];
    f32 vel[3];
    s32 ret;
    u32 flags;
    f32 dot;
    f64 inv;
    register Actor *a;
    register Actor *o;
    register f32 len;
    register f32 t;

    a = D_80106C84;
    if (a == NULL) {
        return;
    }
    a->u.walker.turning = 0;
    ix = D_80107CBC;
    iy = D_80107CBD;
    func_8004DCD0(D_800A7F1C, ix, iy);
    d[0] = (*(Actor *(*))&D_80076800)->fwd.x - (*(Actor *(*))&D_80076800)->pos.x;
    d[1] = (*(Actor *(*))&D_80076800)->fwd.y - (*(Actor *(*))&D_80076800)->pos.y;
    d[2] = (*(Actor *(*))&D_80076800)->fwd.z - (*(Actor *(*))&D_80076800)->pos.z;
    pos[0] = a->pos.x;
    pos[1] = a->pos.y;
    pos[2] = a->pos.z;
    vel[0] = a->u.walker.goalDir.x;
    vel[1] = a->u.walker.goalDir.y;
    vel[2] = a->u.walker.goalDir.z;
    if (a->u.walker.unk_200 & 3) {
        if (ABS(ix) > 8.0f) {
            c[0] = d[1] * (*(Actor *(*))&D_80076800)->up.z - d[2] * (*(Actor *(*))&D_80076800)->up.y;
            c[1] = d[2] * (*(Actor *(*))&D_80076800)->up.x - d[0] * (*(Actor *(*))&D_80076800)->up.z;
            c[2] = d[0] * (*(Actor *(*))&D_80076800)->up.y - d[1] * (*(Actor *(*))&D_80076800)->up.x;
            len = sqrtf(c[0] * c[0] + c[2] * c[2]);
            c[0] = c[0] / len;
            c[2] = c[2] / len;
            ix = ix * ABS(ix);
            ix = ix * 0.01f;
            t = ix * c[0] * 0.5;
            vel[0] = vel[0] + t;
            t = ix * c[2] * 0.5;
            vel[2] = vel[2] + t;
            a->u.walker.turning = 1;
        }
        if (ABS(iy) > 8.0f) {
            len = sqrtf(d[0] * d[0] + d[2] * d[2]);
            d[0] = d[0] / len;
            d[2] = d[2] / len;
            iy = iy * ABS(iy);
            iy = iy * 0.01f;
            t = iy * d[0] * 0.5;
            vel[0] = vel[0] + t;
            t = iy * d[2] * 0.5;
            vel[2] = vel[2] + t;
            a->u.walker.turning = 1;
        }
        if (a->u.walker.turning == 0) {
            func_8002E568(&ix, &iy);
            if (a->u.walker.turning == 1) {
                t = ix * cos((*(View *(*))&D_80112E90)->yaw + 1.570795) * 0.5;
                vel[0] = vel[0] + t;
                t = ix * sin((*(View *(*))&D_80112E90)->yaw + 1.570795) * 0.5;
                vel[2] = vel[2] + t;
                t = iy * cos((*(View *(*))&D_80112E90)->yaw) * 0.5;
                vel[0] = vel[0] + t;
                t = iy * sin((*(View *(*))&D_80112E90)->yaw) * 0.5;
                vel[2] = vel[2] + t;
            }
        }
    }
    if (!(a->u.walker.unk_200 & 1)) {
        pos[0] = a->pos.x;
        pos[2] = a->pos.z;
    }
    if (!(a->u.walker.unk_200 & 2)) {
        vel[0] = a->fwd.x;
        vel[1] = a->fwd.y;
        vel[2] = a->fwd.z;
    }
    if (a->u.walker.turning != 0) {
        if (D_800AD039 >= 3) {
            D_800AD039 = 0;
            D_800A7F18 = 0;
        }
        if ((len = sqrtf(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2])) >= 1e-7f) {
            inv = 1.0 / len;
            vel[0] = vel[0] * inv;
            vel[1] = vel[1] * inv;
            vel[2] = vel[2] * inv;
        } else {
        }
        if (len > 1e-7f) {
            a->u.walker.goalDir.x = vel[0];
            a->u.walker.goalDir.y = vel[1];
            a->u.walker.goalDir.z = vel[2];
        }
    }
    if (a->u.walker.unk_200 & 1) {
        len = sqrtf(ix * ix + iy * iy);
        if (a->u.walker.unk_1C4 < len) {
            len = a->u.walker.unk_1C4;
        } else if (len < 5.0) {
            len = 0.0f;
        }
        if (a->u.walker.offset.v.y < len) {
            a->u.walker.offset.v.y = a->u.walker.offset.v.y + (len - a->u.walker.offset.v.y) * 0.05;
        } else if (len < a->u.walker.offset.v.y) {
            a->u.walker.offset.v.y = a->u.walker.offset.v.y - (a->u.walker.offset.v.y - len) * 0.1;
        }
    } else {
        a->u.walker.offset.v.y = 0.0f;
    }
    ix = func_80069920(a->fwd.x, a->fwd.z);
    iy = func_80069920(a->u.walker.goalDir.x, a->u.walker.goalDir.z);
    iy = iy - ix;
    if (iy > 3.14159) {
        iy = iy - 6.2831853;
    } else if (iy < -3.14159) {
        iy = iy + 6.2831853;
    }
    if (ABS(iy) < 0.03) {
        a->fwd.x = a->u.walker.goalDir.x;
        a->fwd.y = a->u.walker.goalDir.y;
        a->fwd.z = a->u.walker.goalDir.z;
    } else {
        ix = ix + iy * 0.1;
        ix = ix + a->u.walker.offset.v.z;
        a->u.walker.offset.v.z = a->u.walker.offset.v.z * 0.85;
        if (ABS(a->u.walker.offset.v.z) < 0.01) {
            a->u.walker.offset.v.z = 0.0f;
        }
        a->fwd.x = __sinf(ix);
        a->fwd.z = __cosf(ix);
        a->u.walker.turning = 1;
    }
    a->side.x = a->fwd.y * a->up.z - a->fwd.z * a->up.y;
    a->side.y = a->fwd.z * a->up.x - a->fwd.x * a->up.z;
    a->side.z = a->fwd.x * a->up.y - a->fwd.y * a->up.x;
    pos[0] = pos[0] + a->fwd.x * a->u.walker.offset.v.y * 0.5;
    pos[2] = pos[2] + a->fwd.z * a->u.walker.offset.v.y * 0.5;
    pos[0] = pos[0] + a->u.walker.speed * 0.5;
    pos[2] = pos[2] + a->u.walker.unk_1E8 * 0.5;
    a->u.walker.speed = a->u.walker.speed * 0.85;
    a->u.walker.unk_1E8 = a->u.walker.unk_1E8 * 0.85;
    if (a->u.walker.speed < 1.0 && a->u.walker.unk_1E8 < 1.0) {
        a->u.walker.speed = 0.0f;
        a->u.walker.unk_1E8 = 0.0f;
    }
    a->u.walker.unk_1E4 = a->u.walker.unk_1E4 + a->u.walker.unk_1F4;
    if (a->u.walker.unk_1E4 < -60.0) {
        a->u.walker.unk_1E4 = -60.0f;
    }
    pos[1] = pos[1] + a->u.walker.unk_1E4;
    a->u.walker.unk_D0 = 1;
    flags = 0x22000010;
    ret = func_800208D0((*(u8 (*)[0x1C])&a->u.walker.unk_60.x), (*(u8 (*)[0x14])&a->u.walker.unk_BC.p), (*(f32 (*)[3])&a->pos), pos, vel, &flags);
    a->u.walker.unk_1FC = flags;
    if (flags & 0x80000000) {
        D_80106C60 = a->u.walker.unk_AC.x;
        D_80106C64 = a->u.walker.unk_AC.z;
        D_80106C68 = -a->u.walker.unk_AC.y;
        dot = D_80106C60 * a->u.walker.unk_7C.x + D_80106C64 * a->u.walker.unk_7C.y + D_80106C68 * a->u.walker.unk_7C.z;
        D_80109DF8 = D_80106C60 * 127.0f;
        D_80109DF9 = D_80106C64 * 127.0f;
        D_80109DFA = D_80106C68 * 127.0f;
        if (D_80106C64 == 0.0f) {
            func_8002F640(func_80069920(D_80106C60, D_80106C68));
        }
        if (D_80106C64 >= 0.707) {
            a->u.walker.unk_1E4 = 0.0f;
            a->u.walker.unk_200 &= ~0x10000;
        }
        if (D_80106C64 < 0.0) {
            a->u.walker.unk_1E4 = a->u.walker.unk_1F4;
        }
    }
    o = func_8002E35C(pos);
    if (o != NULL) {
        n[0] = a->u.walker.unk_1A8.l0.scale.y;
        n[1] = 0.0f;
        n[2] = a->u.walker.unk_1A8.l1.unk_1AC.z;
        if ((len = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2])) >= 1e-7f) {
            inv = 1.0 / len;
            n[0] = n[0] * inv;
            n[1] = n[1] * inv;
            n[2] = n[2] * inv;
        } else {
        }
        if (len > 3.0) {
            if (len > 15.0) {
                func_800179D8(0x12);
            }
            ix = func_80069920(a->pos.x - o->pos.x, a->pos.z - o->pos.z);
            func_8002F640(ix);
            o->u.prop.unk_1D4.b = __sinf(ix) * -127.0;
            o->u.prop.unk_1D5 = __cosf(ix) * -127.0;
            o->u.prop.unk_1D6 = 0;
            o->u.prop.unk_1D7 = 14;
        }
    }
    a->u.walker.unk_1A8.l0.scale.y = pos[0] - a->pos.x;
    a->u.walker.unk_1A8.l1.unk_1AC.z = pos[2] - a->pos.z;
    D_800A7A70 += sqrtf(a->u.walker.unk_1A8.l0.scale.y * a->u.walker.unk_1A8.l0.scale.y + a->u.walker.unk_1A8.l1.unk_1AC.z * a->u.walker.unk_1A8.l1.unk_1AC.z);
    if (D_8012F158 != 0 && D_800A7A70 > D_8012F158) {
        func_800375A8(D_80106C84, 0x22);
    }
    a->pos.x = pos[0];
    a->pos.y = pos[1];
    a->pos.z = pos[2];
    a->u.walker.unk_D8 = pos[0];
    a->u.walker.unk_DC = -pos[2];
    a->u.walker.unk_E0 = pos[1];
}
