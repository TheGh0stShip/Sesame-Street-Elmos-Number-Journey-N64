typedef struct {
    f32 x, y, z;
} Vec3;
typedef struct {
    u8 unk_00[0x24];
    Vec3 pos;
    Vec3 dir;
} Player;
typedef struct {
    u8 unk_00[0x60];
    f32 x;
    f32 y;
    f32 z;
} Target;
typedef struct {
    s32 a, b, c;
} Triple;
typedef struct {
    u8 unk_00[0xC];
    u32 flags;
    u8 unk_10[0x24 - 0x10];
    Vec3 pos;
    Vec3 dir;
    Vec3 up;
    Vec3 side;
    u8 unk_54[0xE8 - 0x54];
    f32 mtx[3][4][4];
    Triple unk_1A8;
    u8 unk_1B4[0x1BC - 0x1B4];
    u8 *anim;
    u8 unk_1C0[0x1CC - 0x1C0];
    s32 frame;
    u8 unk_1D0[0x1D4 - 0x1D0];
    Vec3 offset;
    u8 unk_1E0[0x1E8 - 0x1E0];
    s32 unk_1E8;
    s32 unk_1EC;
    s32 unk_1F0;
    u8 unk_1F4[4];
} Obj;
extern u8 D_800AD03A;
extern s32 D_800717E0;
extern s32 D_80109D4C;
extern Target *D_80112E90;
extern u32 D_80106C7C;
extern Player *D_80106C84;
void func_8003FA80(void);
void func_8001FAA8(f32 m[4][4], Vec3 *pos, Triple t);
void func_8000092C(s32 a, u8 *b, u8 *anim, s32 frame, Vec3 pos, Vec3 *dir, f32 m[4][4], Obj *o);
void func_80001424(void (*fn)(void), void *arg, s32 a, s32 b, f32 c);
f64 func_80069920(f64 y, f64 x);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
f32 func_80056690(f32);
/* the game's sin/cos wrappers are statement-expression macros: the store address is taken before the call */
#define SINF(x) ({ func_800566A0(x); })
#define COSF(x) ({ func_80057E70(x); })

/* object update: faces the target, rebuilds its basis, advances its animation and follows the player */
void func_8003F52C(Obj *arg0) {
    Obj *p;
    f32 ang;
    Vec3 unused;
    Vec3 v;
    Vec3 d;
    f64 len;

    p = arg0;
    if (D_800AD03A != 0) {
        func_8001FAA8(p->mtx[D_800717E0], &p->pos, p->unk_1A8);
        func_8000092C(p->unk_1E8, p->unk_1F4, p->anim, p->frame, p->pos, &p->dir, p->mtx[D_800717E0], p);
        func_80001424(func_8003FA80, p, p->unk_1F0, 1, 6666666.5f);
        return;
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            p->flags |= 1;
        }
        return;
    }
    d.x = D_80112E90->x - p->pos.x;
    d.z = D_80112E90->z - p->pos.z;
    ang = func_80069920(d.z, d.x);
    p->dir.x = SINF(ang);
    p->dir.z = COSF(ang);
    len = func_80056690((&p->dir)->x * (&p->dir)->x + (&p->dir)->y * (&p->dir)->y + (&p->dir)->z * (&p->dir)->z);
    if (len != 0.0) {
        (&p->dir)->x = (&p->dir)->x * (1.0 / len);
        (&p->dir)->y = (&p->dir)->y * (1.0 / len);
        (&p->dir)->z = (&p->dir)->z * (1.0 / len);
    }
    p->side.x = p->dir.y * p->up.z - p->dir.z * p->up.y;
    p->side.y = p->dir.z * p->up.x - p->dir.x * p->up.z;
    p->side.z = p->dir.x * p->up.y - p->dir.y * p->up.x;
    p->flags |= 0x8000;
    p->flags &= ~0x80;
    if (p->anim != NULL && !(D_80106C7C & 1)) {
        p->frame++;
        if (p->frame > p->anim[4]) {
            p->flags |= 8;
            if (!(p->flags & 0x80)) {
                p->frame = 0;
            }
        }
    }
    v.x = D_80106C84->pos.x;
    v.y = D_80106C84->pos.y;
    v.z = D_80106C84->pos.z;
    v.x += p->offset.x;
    v.y += p->offset.y;
    v.z += p->offset.z;
    p->dir.x = D_80106C84->dir.x;
    p->dir.y = D_80106C84->dir.y;
    p->dir.z = D_80106C84->dir.z;
    p->pos.x = v.x;
    p->pos.y = v.y;
    p->pos.z = v.z;
}
