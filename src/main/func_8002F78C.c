typedef struct { f32 x, y, z; } Vec3;
typedef struct Actor {
    u8 pad0[0x24];
    Vec3 pos;          /* 0x24 */
    Vec3 fwd;          /* 0x30 */
    Vec3 up;           /* 0x3C */
    Vec3 side;         /* 0x48 */
    u8 pad54[0x60 - 0x54];
    u8 unk_60[0x7C - 0x60];
    Vec3 unk_7C;       /* 0x7C */
    u8 pad88[0xAC - 0x88];
    Vec3 unk_AC;       /* 0xAC */
    u8 padB8[0xBC - 0xB8];
    u8 unk_BC[0xD0 - 0xBC];
    s32 unk_D0;        /* 0xD0 */
    u8 padD4[0xD8 - 0xD4];
    f32 unk_D8;
    f32 unk_DC;
    f32 unk_E0;
    u8 padE4[0x1AC - 0xE4];
    Vec3 unk_1AC;      /* 0x1AC */
    Vec3 goalDir;      /* 0x1B8 */
    f32 unk_1C4;
    u8 pad1C8[0x1D4 - 0x1C8];
    s8 unk_1D4;
    s8 unk_1D5;
    s8 unk_1D6;
    s8 unk_1D7;
    f32 unk_1D8;
    f32 unk_1DC;
    f32 unk_1E0;
    f32 unk_1E4;
    f32 unk_1E8;
    u8 pad1EC[0x1F4 - 0x1EC];
    f32 unk_1F4;
    u8 pad1F8[0x1FC - 0x1F8];
    s32 unk_1FC;
    s32 flags;         /* 0x200 */
    s32 turning;       /* 0x204 */
} Actor;
typedef struct {
    u8 pad0[0x24];
    Vec3 eye;          /* 0x24 */
    Vec3 at;           /* 0x30 */
    Vec3 up;           /* 0x3C */
} Camera;
typedef struct {
    u8 pad0[0xFC];
    f32 unk_FC;
} View;

extern Actor *D_80106C84;
extern Camera *D_80076800;
extern View *D_80112E90;
extern s8 D_80107CBC;
extern s8 D_80107CBD;
extern s32 D_800A7F1C;
extern u8 D_800AD039;
extern s16 D_800A7F18;
extern f32 D_80106C60;
extern f32 D_80106C64;
extern f32 D_80106C68;
extern s8 D_80109DF8;
extern s8 D_80109DF9;
extern s8 D_80109DFA;
extern u32 D_800A7A70;
extern u32 D_8012F158;

void func_800179D8(s32 a);
s32 func_800208D0(void *a, void *b, Vec3 *from, Vec3 *to, Vec3 *dir, s32 *flags);
Actor *func_8002E35C(Vec3 *pos);
void func_8002E568(f32 *x, f32 *y);
void func_8002F640(f32 angle);
void func_800375A8(Actor *a, s32 b);
void func_8004DCD0(s32 a, s32 x, s32 y);
f32 func_80056690(f32 x);
f32 func_800566A0(f32 a);
f32 func_80057E70(f32 a);
f64 func_80069920(f64 y, f64 x);
f64 func_80069FA0(f64 a);
f64 func_8006A080(f64 a);

#define ABSF(x) ((x) >= 0.0f ? (x) : -(x))

/* Player movement update: stick input relative to the camera, turning, collision and floor response. */
void func_8002D060(void) {
    f32 sx;
    f32 sy;
    Vec3 n;
    Vec3 cam;
    Vec3 side;
    Vec3 pos;
    Vec3 move;
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
    p->turning = 0;
    sx = D_80107CBC;
    sy = D_80107CBD;
    func_8004DCD0(D_800A7F1C, sx, sy);
    cam.x = D_80076800->at.x - D_80076800->eye.x;
    cam.y = D_80076800->at.y - D_80076800->eye.y;
    cam.z = D_80076800->at.z - D_80076800->eye.z;
    pos.x = p->pos.x;
    pos.y = p->pos.y;
    pos.z = p->pos.z;
    move.x = p->goalDir.x;
    move.y = p->goalDir.y;
    move.z = p->goalDir.z;
    if (p->flags & 3) {
        if (ABSF(sx) > 8.0f) {
            side.x = cam.y * D_80076800->up.z - cam.z * D_80076800->up.y;
            side.y = cam.z * D_80076800->up.x - cam.x * D_80076800->up.z;
            side.z = cam.x * D_80076800->up.y - cam.y * D_80076800->up.x;
            len = func_80056690(side.x * side.x + side.z * side.z);
            side.x = side.x / len;
            side.z = side.z / len;
            sx = sx * ABSF(sx);
            sx = sx * 0.01f;
            step = sx * side.x * 0.5;
            move.x = move.x + step;
            step = sx * side.z * 0.5;
            move.z = move.z + step;
            p->turning = 1;
        }
        if (ABSF(sy) > 8.0f) {
            len = func_80056690(cam.x * cam.x + cam.z * cam.z);
            cam.x = cam.x / len;
            cam.z = cam.z / len;
            sy = sy * ABSF(sy);
            sy = sy * 0.01f;
            step = sy * cam.x * 0.5;
            move.x = move.x + step;
            step = sy * cam.z * 0.5;
            move.z = move.z + step;
            p->turning = 1;
        }
        if (p->turning == 0) {
            func_8002E568(&sx, &sy);
            if (p->turning == 1) {
                register f64 d;
                d = sx;
                step = d * func_8006A080(D_80112E90->unk_FC + 1.570795) * 0.5;
                move.x = move.x + step;
                d = sx;
                step = d * func_80069FA0(D_80112E90->unk_FC + 1.570795) * 0.5;
                move.z = move.z + step;
                d = sy;
                step = d * func_8006A080(D_80112E90->unk_FC) * 0.5;
                move.x = move.x + step;
                d = sy;
                step = d * func_80069FA0(D_80112E90->unk_FC) * 0.5;
                move.z = move.z + step;
            }
        }
    }
    if (!(p->flags & 1)) {
        pos.x = p->pos.x;
        pos.z = p->pos.z;
    }
    if (!(p->flags & 2)) {
        move.x = p->fwd.x;
        move.y = p->fwd.y;
        move.z = p->fwd.z;
    }
    if (p->turning != 0) {
        if (D_800AD039 >= 3) {
            D_800AD039 = 0;
            D_800A7F18 = 0;
        }
        if ((len = func_80056690(move.x * move.x + move.y * move.y + move.z * move.z)) >= 1e-7f) {
            inv = 1.0 / len;
            move.x = move.x * inv;
            move.y = move.y * inv;
            move.z = move.z * inv;
        } else {
        }
        if (len > 1e-7f) {
            p->goalDir.x = move.x;
            p->goalDir.y = move.y;
            p->goalDir.z = move.z;
        }
    }
    if (p->flags & 1) {
        len = func_80056690(sx * sx + sy * sy);
        if (len > p->unk_1C4) {
            len = p->unk_1C4;
        } else if (len < 5.0) {
            len = 0.0f;
        }
        if (len > p->unk_1D8) {
            p->unk_1D8 = p->unk_1D8 + (len - p->unk_1D8) * 0.05;
        } else if (len < p->unk_1D8) {
            p->unk_1D8 = p->unk_1D8 - (p->unk_1D8 - len) * 0.1;
        }
    } else {
        p->unk_1D8 = 0.0f;
    }
    sx = func_80069920(p->fwd.x, p->fwd.z);
    sy = func_80069920(p->goalDir.x, p->goalDir.z);
    sy = sy - sx;
    if (sy > 3.14159) {
        sy = sy - 6.2831853;
    } else if (sy < -3.14159) {
        sy = sy + 6.2831853;
    }
    if (ABSF(sy) < 0.03) {
        p->fwd.x = p->goalDir.x;
        p->fwd.y = p->goalDir.y;
        p->fwd.z = p->goalDir.z;
    } else {
        sx = sx + sy * 0.1;
        sx = sx + p->unk_1DC;
        p->unk_1DC = p->unk_1DC * 0.85;
        if (ABSF(p->unk_1DC) < 0.01) {
            p->unk_1DC = 0.0f;
        }
        p->fwd.x = func_80057E70(sx);
        p->fwd.z = func_800566A0(sx);
        p->turning = 1;
    }
    p->side.x = p->fwd.y * p->up.z - p->fwd.z * p->up.y;
    p->side.y = p->fwd.z * p->up.x - p->fwd.x * p->up.z;
    p->side.z = p->fwd.x * p->up.y - p->fwd.y * p->up.x;
    pos.x = pos.x + p->fwd.x * p->unk_1D8 * 0.5;
    pos.z = pos.z + p->fwd.z * p->unk_1D8 * 0.5;
    pos.x = pos.x + p->unk_1E0 * 0.5;
    pos.z = pos.z + p->unk_1E8 * 0.5;
    p->unk_1E0 = p->unk_1E0 * 0.85;
    p->unk_1E8 = p->unk_1E8 * 0.85;
    if (p->unk_1E0 < 1.0 && p->unk_1E8 < 1.0) {
        p->unk_1E0 = 0.0f;
        p->unk_1E8 = 0.0f;
    }
    pos.y = pos.y + p->unk_1E4;
    p->unk_D0 = 1;
    flags = 0x22000010;
    ret = func_800208D0(p->unk_60, p->unk_BC, &p->pos, &pos, &move, &flags);
    p->unk_1FC = flags;
    if (flags & 0x80000000) {
        D_80106C60 = p->unk_AC.x;
        D_80106C64 = p->unk_AC.z;
        D_80106C68 = -p->unk_AC.y;
        dot = D_80106C60 * p->unk_7C.x + D_80106C64 * p->unk_7C.y + D_80106C68 * p->unk_7C.z;
        D_80109DF8 = D_80106C60 * 127.0f;
        D_80109DF9 = D_80106C64 * 127.0f;
        D_80109DFA = D_80106C68 * 127.0f;
        if (D_80106C64 == 0.0f) {
            func_8002F640(func_80069920(D_80106C60, D_80106C68));
        }
        if (D_80106C64 >= 0.707) {
            p->unk_1E4 = 0.0f;
            p->flags &= ~0x10000;
        } else {
            p->unk_1E4 = p->unk_1F4;
        }
    } else {
        p->unk_1E4 = p->unk_1F4;
    }
    other = func_8002E35C(&pos);
    if (other != NULL) {
        n.x = p->unk_1AC.x;
        n.y = 0.0f;
        n.z = p->unk_1AC.z;
        if ((len = func_80056690(n.x * n.x + n.y * n.y + n.z * n.z)) >= 1e-7f) {
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
            other->unk_1D4 = func_80057E70(sx) * -127.0;
            other->unk_1D5 = func_800566A0(sx) * -127.0;
            other->unk_1D6 = 0;
            other->unk_1D7 = 14;
        }
    }
    p->unk_1AC.x = pos.x - p->pos.x;
    p->unk_1AC.z = pos.z - p->pos.z;
    {
    register f32 total;
    total = D_800A7A70;
    D_800A7A70 = total + func_80056690(p->unk_1AC.x * p->unk_1AC.x + p->unk_1AC.z * p->unk_1AC.z);
    if (D_8012F158 != 0 && D_800A7A70 > D_8012F158) {
        func_800375A8(D_80106C84, 0x22);
    }
    }
    p->pos.x = pos.x;
    p->pos.y = pos.y;
    p->pos.z = pos.z;
    p->unk_D8 = pos.x;
    p->unk_DC = -pos.z;
    p->unk_E0 = pos.y;
}
