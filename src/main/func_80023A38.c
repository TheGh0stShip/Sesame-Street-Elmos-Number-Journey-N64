typedef struct {
    f32 x, y, z;
} Vec3;
typedef struct {
    s16 unk_00;
    u16 unk_02;
    s16 unk_04;
    u16 type;
    s32 unk_08;
    Vec3 pos;
    Vec3 dir;
    Vec3 up;
    s32 unk_30;
} Spawn;
typedef struct {
    u8 unk_00[4];
    u16 count;
    u8 unk_06[0xC - 6];
    u32 *src;
    u32 *buf;
    s32 unk_14;
    u8 *data;
    s32 unk_1C;
    s32 size;
} Anim;
typedef struct {
    u8 unk_00[0x20];
    Vec3 a;
    Vec3 b;
    Vec3 c;
    Vec3 d;
} Node;
typedef struct {
    s32 count;
    Node **nodes;
} Model;
typedef struct Obj Obj;
typedef struct {
    s32 type;
    Obj *owner;
    s32 flags;
    u8 unk_0C[0x20 - 0xC];
    Vec3 a;
    Vec3 b;
    Vec3 c;
    Vec3 d;
} Shape;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    Shape *shape;
} Coll;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    void *list;
    s32 count;
    Obj *owner;
    s32 flags;
    s32 unk_18;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
} CollHead;
struct Obj {
    u8 unk_00[8];
    u16 unk_08;
    u16 type;
    s32 unk_0C;
    void (*update)(void);
    void (*draw)(void);
    u8 unk_18[0x24 - 0x18];
    Vec3 pos;
    Vec3 dir;
    Vec3 up;
    Vec3 side;
    f32 radius;
    f32 collRadius;
    u8 unk_5C[0x84 - 0x5C];
    s32 unk_84;
    s32 unk_88;
    u8 unk_8C[0xB0 - 0x8C];
    s32 group;
    s32 unk_B4;
    s32 unk_B8;
    s32 *unk_BC;
    s32 unk_C0;
    Obj *unk_C4;
    s32 unk_C8;
    s32 unk_CC;
    f32 unk_D0;
    f32 unk_D4;
    f32 unk_D8;
    f32 unk_DC;
    Coll *colls;
    Shape *shapes;
    u8 unk_E8[0x168 - 0xE8];
    f32 mtx[4][4];
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    s32 unk_1B4;
    s32 unk_1B8;
    u8 unk_1BC[0x1C4 - 0x1BC];
    s32 unk_1C4;
    u8 unk_1C8[0x1D4 - 0x1C8];
    s32 unk_1D4;
    s32 unk_1D8;
    s32 unk_1DC;
    s32 unk_1E0;
    s32 unk_1E4;
    Anim *anim;
    s32 unk_1EC;
    s32 unk_1F0;
    u8 unk_1F4[0x218 - 0x1F4];
};
void func_80024F58(void);
void func_80025A54(void);
void func_800254C4(void);
void func_80025700(void);
void *func_80042080(s32 size);
void func_80069C70(void *dst, s32 c, s32 n);
void func_80000EDC(u32 *dst, u32 *src, u32 n);
f32 func_80056690(f32);
void func_8001E74C(f32 m[4][4], Vec3 *pos, Vec3 *dir, Vec3 *up, Vec3 *side);
void func_80041A80(Obj *o, s32 list, s32 a, s32 b);
void func_80046250(s32 group, void *item, s32 a, s32 b);
void func_80059540(char *expr, char *file, s32 line);
void func_8005725C(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_80021334(Shape *s, Vec3 *a, Vec3 *b, Vec3 *c, Vec3 *d);
extern s32 D_80133FB0;
extern s32 D_80115408;
extern s32 D_80115520;
extern char D_8007A768[];
extern char D_8007A76C[];
extern Anim D_80177E78;
extern Anim D_80178590;
extern Anim D_80178CB0;
extern Anim D_80179078;
extern Anim D_80179B00;
extern Anim D_8017AD08;
extern Anim D_8017B618;
extern Anim D_8017CA80;
extern Anim D_8017CEE0;
extern Anim D_8017E800;
extern Anim D_8017E810;
extern Anim D_8017EC00;
extern Anim D_8017FD80;
extern Anim D_80180580;
extern Anim D_801826D8;
extern Anim D_80183188;
extern Anim D_80184A38;
extern Anim D_801862A0;
extern Anim D_801866A8;
extern Anim D_801868F0;
extern Anim D_80188B90;
extern Anim D_801893C0;
extern Anim D_8018DFF8;
extern Anim D_801900E8;
extern Anim D_80190E50;
extern Anim D_80190F84;
extern Anim D_801968F0;
extern Anim D_80198AA8;
extern Anim D_8019AB80;
extern Anim D_8019C450;
extern Anim D_8019CAB0;
extern Anim D_8019D388;
extern Anim D_8019E040;
extern Anim D_801AA570;
extern Anim D_8025C5C8;
extern Anim D_80267E70;
extern Anim D_80271CE4;
extern Anim D_80274E88;
extern Model D_8017C62C;
extern Model D_8017E3AC;
extern Model D_8018012C;
extern Model D_801867CC;
extern Model D_801AA450;
extern Model D_80260390;
extern Model D_80273860;

/* creates an object from a spawn record: picks its animation/collision by type, sets its basis and collision shapes */
void func_80023A38(Spawn *arg0) {
    Spawn *s;
    Anim *anim;
    Obj *o;
    f32 len;
    s32 i;
    s32 unk_34;
    u32 *buf;
    s32 unk_3C;
    Vec3 unk_40;
    s32 unk_50;
    f32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    f32 unk_60[4][4];
    Vec3 v;
    Vec3 unk_B0;
    Vec3 in;
    Node *nodes;
    Model *model;
    Coll *coll;
    Coll *colls;
    Shape *shape;
    Shape *shapes;
    s32 size;
    f32 radius;
    f64 inv;

    s = arg0;
    unk_50 = 0;
    unk_54 = 1.0f;
    unk_58 = 0;
    o = func_80042080(sizeof(Obj));
    func_80069C70(o, 0, sizeof(Obj));
    o->unk_08 = s->unk_02;
    o->type = s->type;
    o->unk_0C = s->unk_08;
    o->unk_1EC = D_80133FB0;
    o->colls = NULL;
    o->shapes = NULL;
    o->unk_1F0 = 4;
    o->update = func_80024F58;
    o->draw = func_80025A54;
    o->unk_1D4 = 0;
    o->unk_1D8 = 0;
    o->unk_1DC = 0;
    o->unk_1E0 = 0;
    o->radius = 0;
    anim = NULL;
    model = NULL;
    radius = 0;
    switch (o->type) {
    case 0x1014:
        anim = NULL;
        o->radius = 250.0f;
        break;
    case 0x1016:
        anim = NULL;
        o->radius = 70.0f;
        break;
    case 0x1502:
        anim = &D_80271CE4;
        break;
    case 0x3002:
        anim = &D_80267E70;
        model = &D_80260390;
        break;
    case 0x5501:
        anim = &D_80274E88;
        model = &D_80273860;
        break;
    case 0x5505:
        anim = &D_8025C5C8;
        o->radius = 100.0f;
        break;
    case 0x1001:
        o->radius = 10.0f;
        break;
    case 0x1015:
        anim = &D_801AA570;
        model = &D_801AA450;
        break;
    case 0x5004:
        anim = &D_80190F84;
        break;
    case 0x5006:
        anim = &D_801900E8;
        break;
    case 0x5503:
        anim = &D_8017E810;
        o->radius = 80.0f;
        break;
    case 0x5500:
        anim = &D_80183188;
        o->radius = 220.0f;
        break;
    case 0x5504:
        anim = &D_801862A0;
        o->radius = 40.0f;
        break;
    case 0x5506:
        anim = &D_801893C0;
        o->radius = 40.0f;
        break;
    case 0x5502:
        anim = &D_8018DFF8;
        o->radius = 40.0f;
        break;
    case 0x6000:
        o->update = func_800254C4;
        anim = &D_80188B90;
        o->radius = 175.0f;
        break;
    case 0x6001:
        o->update = func_800254C4;
        anim = &D_8017EC00;
        o->radius = 175.0f;
        break;
    case 0x6002:
        o->update = func_800254C4;
        anim = &D_8019CAB0;
        o->radius = 175.0f;
        break;
    case 0x6503:
        anim = &D_80190E50;
        o->radius = 25.0f;
        break;
    case 0x6507:
        anim = &D_80198AA8;
        o->radius = 60.0f;
        break;
    case 0x6508:
        anim = &D_801968F0;
        o->radius = 60.0f;
        break;
    case 0x6509:
        anim = &D_8019E040;
        o->radius = 100.0f;
        break;
    case 0x7000:
        anim = &D_80179078;
        break;
    case 0x7002:
        anim = &D_801826D8;
        o->radius = 30.0f;
        break;
    case 0x7003:
        anim = &D_801866A8;
        o->radius = 20.0f;
        break;
    case 0x7004:
        anim = &D_8017CA80;
        model = &D_8017C62C;
        break;
    case 0x7005:
        anim = &D_8017E800;
        model = &D_8017E3AC;
        break;
    case 0x7006:
        anim = &D_80180580;
        model = &D_8018012C;
        break;
    case 0x7009:
        anim = &D_80184A38;
        break;
    case 0x7011:
        anim = &D_8017AD08;
        break;
    case 0x7016:
        anim = &D_801868F0;
        model = &D_801867CC;
        break;
    case 0x7017:
        anim = NULL;
        o->radius = 230.0f;
        break;
    case 0x7502:
        anim = &D_8019AB80;
        o->radius = 50.0f;
        break;
    case 0x7503:
        anim = &D_8019D388;
        o->radius = 50.0f;
        break;
    case 0x8001:
        anim = &D_80178590;
        o->radius = 25.0f;
        break;
    case 0x8002:
        anim = &D_80178CB0;
        o->radius = 25.0f;
        break;
    case 0x8003:
        o->radius = 100.0f;
        anim = &D_8017FD80;
        break;
    case 0x8004:
        anim = &D_80179B00;
        o->radius = 75.0f;
        break;
    case 0x8005:
        anim = &D_8017B618;
        o->radius = 50.0f;
        break;
    case 0x8006:
        anim = &D_80177E78;
        o->radius = 25.0f;
        break;
    case 0x8888:
        anim = &D_8019C450;
        o->update = func_80025700;
        D_80115520 = 0;
        break;
    case 0x8889:
        anim = &D_8017CEE0;
        o->update = func_80025700;
        D_80115520 = 0;
        break;
    }
    if (anim != NULL && anim->size != 0) {
        buf = func_80042080(anim->size);
        anim->buf = buf;
        anim->data = (u8 *)buf + (anim->count << 4);
        func_80069C70(anim->data, 0, anim->size);
        func_80000EDC(anim->buf, anim->src, anim->count * 4);
    }
    o->scaleX = 1.0f;
    o->scaleY = 1.0f;
    o->scaleZ = 1.0f;
    o->unk_1B8 = 0;
    o->anim = anim;
    o->pos.x = s->pos.x;
    o->pos.y = s->pos.y;
    o->pos.z = s->pos.z;
    o->dir.x = s->dir.x;
    o->dir.y = s->dir.y;
    o->dir.z = s->dir.z;
    len = func_80056690(o->dir.x * o->dir.x + o->dir.y * o->dir.y + o->dir.z * o->dir.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        o->dir.x = o->dir.x * inv;
        o->dir.y = o->dir.y * inv;
        o->dir.z = o->dir.z * inv;
    } else {
    }
    o->up.x = s->up.x;
    o->up.y = s->up.y;
    o->up.z = s->up.z;
    len = func_80056690(o->up.x * o->up.x + o->up.y * o->up.y + o->up.z * o->up.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        o->up.x = o->up.x * inv;
        o->up.y = o->up.y * inv;
        o->up.z = o->up.z * inv;
    } else {
    }
    o->side.x = o->dir.y * o->up.z - o->dir.z * o->up.y;
    o->side.y = o->dir.z * o->up.x - o->dir.x * o->up.z;
    o->side.z = o->dir.x * o->up.y - o->dir.y * o->up.x;
    func_8001E74C(o->mtx, &s->pos, &s->dir, &s->up, &o->side);
    o->unk_1C4 = 0;
    func_80041A80(o, D_80115408, 4, 0);
    if (radius != 0.0f) {
        o->group = s->unk_30;
        o->collRadius = radius;
        o->unk_84 = 1;
        o->unk_88 = 0;
        o->unk_B4 = 0;
        o->unk_B8 = 0;
        o->unk_BC = &o->unk_C0;
        o->unk_C0 = 1;
        o->unk_C4 = o;
        o->unk_C8 = 0x10;
        o->unk_DC = o->collRadius;
        o->unk_D0 = o->pos.x;
        o->unk_D4 = -o->pos.z;
        o->unk_D8 = o->pos.y;
        func_80046250(o->group, &o->unk_B4, 2, 0);
    } else if (model != NULL) {
        nodes = *model->nodes;
        o->group = arg0->unk_30;
        v.x = o->pos.x;
        v.y = -o->pos.z;
        v.z = o->pos.y;
        unk_B0.x = o->dir.x;
        unk_B0.y = -o->dir.z;
        unk_B0.z = o->dir.y;
        size = model->count * sizeof(Coll);
        colls = func_80042080(size);
        if (colls) {
        } else {
            func_80059540(D_8007A768, D_8007A76C, 818);
        }
        o->colls = colls;
        size = model->count * sizeof(Shape);
        shapes = func_80042080(size);
        if (shapes) {
        } else {
            func_80059540(D_8007A768, D_8007A76C, 825);
        }
        o->shapes = shapes;
        for (i = 0; i < model->count; i++) {
            coll = &colls[i];
            shape = &shapes[i];
            coll->unk_00 = 0;
            coll->unk_04 = 0;
            coll->shape = shape;
            shape->type = 3;
            shape->owner = o;
            shape->flags = 0x02000000;
            v.x = (&o->pos)->x;
            v.y = (&o->pos)->y;
            v.z = (&o->pos)->z;
            in.x = nodes[i].a.x;
            in.y = nodes[i].a.z;
            in.z = -nodes[i].a.y;
            func_8005725C(o->mtx, in.x, in.y, in.z, &(&v)->x, &(&v)->y, &(&v)->z);
            shape->b.x = v.x;
            shape->b.y = -v.z;
            shape->b.z = v.y;
            v.x = (&o->pos)->x;
            v.y = (&o->pos)->y;
            v.z = (&o->pos)->z;
            in.x = nodes[i].c.x;
            in.y = nodes[i].c.z;
            in.z = -nodes[i].c.y;
            func_8005725C(o->mtx, in.x, in.y, in.z, &(&v)->x, &(&v)->y, &(&v)->z);
            shape->a.x = v.x;
            shape->a.y = -v.z;
            shape->a.z = v.y;
            v.x = (&o->pos)->x;
            v.y = (&o->pos)->y;
            v.z = (&o->pos)->z;
            in.x = nodes[i].b.x;
            in.y = nodes[i].b.z;
            in.z = -nodes[i].b.y;
            func_8005725C(o->mtx, in.x, in.y, in.z, &(&v)->x, &(&v)->y, &(&v)->z);
            shape->d.x = v.x;
            shape->d.y = -v.z;
            shape->d.z = v.y;
            v.x = (&o->pos)->x;
            v.y = (&o->pos)->y;
            v.z = (&o->pos)->z;
            in.x = nodes[i].d.x;
            in.y = nodes[i].d.z;
            in.z = -nodes[i].d.y;
            func_8005725C(o->mtx, in.x, in.y, in.z, &(&v)->x, &(&v)->y, &(&v)->z);
            shape->c.x = v.x;
            shape->c.y = -v.z;
            shape->c.z = v.y;
            func_80021334(shape, &shape->a, &shape->b, &shape->c, &shape->d);
            func_80046250(o->group, coll, 2, 0);
        }
    }
    return;
}
