#include "common.h"
#include "game_structs.h"


extern Actor *D_80106C84;
s32 func_800208D0(void *a, void *b, Vec3f *from, Vec3f *to, Vec3f *dir, s32 *flags);
Actor *func_8002E35C(Vec3f *pos);

#define ABSF(x) ((x) >= 0.0f ? (x) : -(x))

/* Player movement update: stick input relative to the camera, turning, collision and floor response. */
void func_8002F78C(void) {
    f32 sx;
    f32 sy;
    Vec3f n;
    Vec3f cam;
    Vec3f side;
    Vec3f pos;
    Vec3f move;
    s32 ret;
    s32 flags;
    f32 dot;
    f64 inv;
    register Actor *p = D_80106C84;
    register Actor *other;
    register f32 len;
    register f32 step;

    if (p == NULL) {
        return;
    }
    p->u.walker.turning = 0;
    sx = D_80107CBC;
    sy = D_80107CBD;
    func_8004DCD0(D_800A7F1C, sx, sy);
    cam.x = (*(Actor *(*))&D_80076800)->fwd.x - (*(Actor *(*))&D_80076800)->pos.x;
    cam.y = (*(Actor *(*))&D_80076800)->fwd.y - (*(Actor *(*))&D_80076800)->pos.y;
    cam.z = (*(Actor *(*))&D_80076800)->fwd.z - (*(Actor *(*))&D_80076800)->pos.z;
    pos.x = p->pos.x;
    pos.y = p->pos.y;
    pos.z = p->pos.z;
    move.x = p->u.walker.goalDir.x;
    move.y = p->u.walker.goalDir.y;
    move.z = p->u.walker.goalDir.z;
    if (p->u.walker.unk_200 & 3) {
        if (ABSF(sx) > 8.0f) {
            side.x = cam.y * (*(Actor *(*))&D_80076800)->up.z - cam.z * (*(Actor *(*))&D_80076800)->up.y;
            side.y = cam.z * (*(Actor *(*))&D_80076800)->up.x - cam.x * (*(Actor *(*))&D_80076800)->up.z;
            side.z = cam.x * (*(Actor *(*))&D_80076800)->up.y - cam.y * (*(Actor *(*))&D_80076800)->up.x;
            len = sqrtf(side.x * side.x + side.z * side.z);
            side.x = side.x / len;
            side.z = side.z / len;
            sx = sx * ABSF(sx);
            sx = sx * 0.01f;
            step = sx * side.x * 0.5;
            move.x = move.x + step;
            step = sx * side.z * 0.5;
            move.z = move.z + step;
            p->u.walker.turning = 1;
        }
        if (ABSF(sy) > 8.0f) {
            len = sqrtf(cam.x * cam.x + cam.z * cam.z);
            cam.x = cam.x / len;
            cam.z = cam.z / len;
            sy = sy * ABSF(sy);
            sy = sy * 0.01f;
            step = sy * cam.x * 0.5;
            move.x = move.x + step;
            step = sy * cam.z * 0.5;
            move.z = move.z + step;
            p->u.walker.turning = 1;
        }
        if (p->u.walker.turning == 0) {
            func_8002E568(&sx, &sy);
            if (p->u.walker.turning == 1) {
                register f64 d;
                d = sx;
                step = d * func_8006A080((*(View *(*))&D_80112E90)->yaw + 1.570795) * 0.5;
                move.x = move.x + step;
                d = sx;
                step = d * func_80069FA0((*(View *(*))&D_80112E90)->yaw + 1.570795) * 0.5;
                move.z = move.z + step;
                d = sy;
                step = d * func_8006A080((*(View *(*))&D_80112E90)->yaw) * 0.5;
                move.x = move.x + step;
                d = sy;
                step = d * func_80069FA0((*(View *(*))&D_80112E90)->yaw) * 0.5;
                move.z = move.z + step;
            }
        }
    }
    if (!(p->u.walker.unk_200 & 1)) {
        pos.x = p->pos.x;
        pos.z = p->pos.z;
    }
    if (!(p->u.walker.unk_200 & 2)) {
        move.x = p->fwd.x;
        move.y = p->fwd.y;
        move.z = p->fwd.z;
    }
    if (p->u.walker.turning != 0) {
        if (D_800AD039 >= 3) {
            D_800AD039 = 0;
            D_800A7F18 = 0;
        }
        if ((len = sqrtf(move.x * move.x + move.y * move.y + move.z * move.z)) >= 1e-7f) {
            inv = 1.0 / len;
            move.x = move.x * inv;
            move.y = move.y * inv;
            move.z = move.z * inv;
        } else {
        }
        if (len > 1e-7f) {
            p->u.walker.goalDir.x = move.x;
            p->u.walker.goalDir.y = move.y;
            p->u.walker.goalDir.z = move.z;
        }
    }
    if (p->u.walker.unk_200 & 1) {
        len = sqrtf(sx * sx + sy * sy);
        if (len > p->u.walker.unk_1C4) {
            len = p->u.walker.unk_1C4;
        } else if (len < 5.0) {
            len = 0.0f;
        }
        if (len > p->u.walker.offset.v.y) {
            p->u.walker.offset.v.y = p->u.walker.offset.v.y + (len - p->u.walker.offset.v.y) * 0.05;
        } else if (len < p->u.walker.offset.v.y) {
            p->u.walker.offset.v.y = p->u.walker.offset.v.y - (p->u.walker.offset.v.y - len) * 0.1;
        }
    } else {
        p->u.walker.offset.v.y = 0.0f;
    }
    sx = func_80069920(p->fwd.x, p->fwd.z);
    sy = func_80069920(p->u.walker.goalDir.x, p->u.walker.goalDir.z);
    sy = sy - sx;
    if (sy > 3.14159) {
        sy = sy - 6.2831853;
    } else if (sy < -3.14159) {
        sy = sy + 6.2831853;
    }
    if (ABSF(sy) < 0.03) {
        p->fwd.x = p->u.walker.goalDir.x;
        p->fwd.y = p->u.walker.goalDir.y;
        p->fwd.z = p->u.walker.goalDir.z;
    } else {
        sx = sx + sy * 0.1;
        sx = sx + p->u.walker.offset.v.z;
        p->u.walker.offset.v.z = p->u.walker.offset.v.z * 0.85;
        if (ABSF(p->u.walker.offset.v.z) < 0.01) {
            p->u.walker.offset.v.z = 0.0f;
        }
        p->fwd.x = __sinf(sx);
        p->fwd.z = __cosf(sx);
        p->u.walker.turning = 1;
    }
    p->side.x = p->fwd.y * p->up.z - p->fwd.z * p->up.y;
    p->side.y = p->fwd.z * p->up.x - p->fwd.x * p->up.z;
    p->side.z = p->fwd.x * p->up.y - p->fwd.y * p->up.x;
    pos.x = pos.x + p->fwd.x * p->u.walker.offset.v.y * 0.5;
    pos.z = pos.z + p->fwd.z * p->u.walker.offset.v.y * 0.5;
    pos.x = pos.x + p->u.walker.speed * 0.5;
    pos.z = pos.z + p->u.walker.unk_1E8 * 0.5;
    p->u.walker.speed = p->u.walker.speed * 0.85;
    p->u.walker.unk_1E8 = p->u.walker.unk_1E8 * 0.85;
    if (p->u.walker.speed < 1.0 && p->u.walker.unk_1E8 < 1.0) {
        p->u.walker.speed = 0.0f;
        p->u.walker.unk_1E8 = 0.0f;
    }
    pos.y = pos.y + p->u.walker.unk_1E4;
    p->u.walker.unk_D0 = 1;
    flags = 0x22000010;
    ret = func_800208D0((*(u8 (*)[0x7C-0x60])&p->u.walker.unk_60.x), (*(u8 (*)[0xD0-0xBC])&p->u.walker.unk_BC.p), &p->pos, &pos, &move, &flags);
    p->u.walker.unk_1FC = flags;
    if (flags & 0x80000000) {
        D_80106C60 = p->u.walker.unk_AC.x;
        D_80106C64 = p->u.walker.unk_AC.z;
        D_80106C68 = -p->u.walker.unk_AC.y;
        dot = D_80106C60 * p->u.walker.unk_7C.x + D_80106C64 * p->u.walker.unk_7C.y + D_80106C68 * p->u.walker.unk_7C.z;
        D_80109DF8 = D_80106C60 * 127.0f;
        D_80109DF9 = D_80106C64 * 127.0f;
        D_80109DFA = D_80106C68 * 127.0f;
        if (D_80106C64 == 0.0f) {
            func_8002F640(func_80069920(D_80106C60, D_80106C68));
        }
        if (D_80106C64 >= 0.707) {
            p->u.walker.unk_1E4 = 0.0f;
            p->u.walker.unk_200 &= ~0x10000;
        } else {
            p->u.walker.unk_1E4 = p->u.walker.unk_1F4;
        }
    } else {
        p->u.walker.unk_1E4 = p->u.walker.unk_1F4;
    }
    other = func_8002E35C(&pos);
    if (other != NULL) {
        n.x = p->u.walker.unk_1A8.l1.unk_1AC.x;
        n.y = 0.0f;
        n.z = p->u.walker.unk_1A8.l1.unk_1AC.z;
        if ((len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z)) >= 1e-7f) {
            inv = 1.0 / len;
            n.x = n.x * inv;
            n.y = n.y * inv;
            n.z = n.z * inv;
        } else {
        }
        if (len > 3.0) {
            if (len > 15.0) {
                func_800179D8(0x12);
            }
            sx = func_80069920(p->pos.x - other->pos.x, p->pos.z - other->pos.z);
            func_8002F640(sx);
            other->u.prop.unk_1D4.b = __sinf(sx) * -127.0;
            other->u.prop.unk_1D5 = __cosf(sx) * -127.0;
            other->u.prop.unk_1D6 = 0;
            other->u.prop.unk_1D7 = 14;
        }
    }
    p->u.walker.unk_1A8.l1.unk_1AC.x = pos.x - p->pos.x;
    p->u.walker.unk_1A8.l1.unk_1AC.z = pos.z - p->pos.z;
    {
    register f32 total;
    total = D_800A7A70;
    D_800A7A70 = total + sqrtf(p->u.walker.unk_1A8.l1.unk_1AC.x * p->u.walker.unk_1A8.l1.unk_1AC.x + p->u.walker.unk_1A8.l1.unk_1AC.z * p->u.walker.unk_1A8.l1.unk_1AC.z);
    if (D_8012F158 != 0 && D_800A7A70 > D_8012F158) {
        func_800375A8(D_80106C84, 0x22);
    }
    }
    p->pos.x = pos.x;
    p->pos.y = pos.y;
    p->pos.z = pos.z;
    p->u.walker.unk_D8 = pos.x;
    p->u.walker.unk_DC = -pos.z;
    p->u.walker.unk_E0 = pos.y;
}
