#include "common.h"
#include "game_structs.h"

typedef struct { s32 v[20]; } Pad80;
typedef struct {
    u8 pad0[4];
    u16 unk_4;
} Def;
typedef struct {
    u8 pad0[4];
    u16 unk_4;
    u8 pad6[6];
    void *unk_C;
    u8 *unk_10;
    u8 pad14[4];
    u8 *unk_18;
    u8 pad1C[4];
    s32 unk_20;
} Model;

#define ASSERT(cond, line) if (cond) {} else func_80059540(D_8007B130, D_8007B134, line)

extern u8 **D_800774A4[];
extern Def D_80270C38;
extern Def D_8026A230;
extern Def D_8026E478;
extern Def D_801B55A0;
extern Def D_801B7408;
extern Def D_801AC870;
extern Def D_801B1B30;
extern Def D_8018C0C0;
extern Def D_8018C8C8;
extern Def D_80188640;
void func_8003E078(void);

/* creates an object from a spawn record: picks its model by type, normalises its axes */
void func_8003D3D0(Actor *arg0) {
    Actor *src = arg0;
    Model *m = NULL;
    Def *def = NULL;
    Actor *obj;
    f32 len;
    s32 unk2C;
    s32 unk30;
    u8 *buf;
    Pad16 unk38;
    s32 unk48 = 0;
    f32 unk4C = 1.0f;
    s32 unk50 = 0;
    s32 unk54;
    Pad80 unk58;
    s32 unkA8;
    u8 **tbl;
    f64 inv;

    obj = func_80042080(0x218);
    func_80069C70(obj, 0, 0x218);
    obj->unk_8.l1.type = src->unk_0.l1.unk_2;
    obj->unk_8.l1.unk_A = src->unk_6;
    obj->unk_C.l1.flags = src->unk_8.l0.type;
    obj->u.mapobj.unk_1EC = D_80133FB0;
    obj->u.mapobj.unk_E0 = 0;
    obj->u.mapobj.unk_E4 = 0;
    obj->u.mapobj.unk_54.x = 0;
    obj->u.mapobj.unk_1F0 = 4;
    switch (obj->unk_8.l1.unk_A) {
    case 0x6601:
        def = &D_80270C38;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 459);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_80270C60;
        obj->u.mapobj.unk_1D8 = 1;
        break;
    case 0x5601:
        def = &D_8026A230;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 471);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_8026A258;
        obj->u.mapobj.unk_1D8 = 1;
        break;
    case 0x5602:
        def = &D_8026E478;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 481);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_8026E4A0;
        obj->u.mapobj.unk_1D8 = 1;
        break;
    case 0x8102:
        def = &D_801B55A0;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 494);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_801B55C8;
        obj->u.mapobj.unk_1D8 = 1;
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    case 0x8103:
        def = &D_801B7408;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 522);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_801B7430;
        obj->u.mapobj.unk_1D8 = 1;
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    case 0x6102:
        def = &D_801AC870;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 534);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_801AC898;
        obj->u.mapobj.unk_1D8 = 3;
        obj->u.mapobj.sub = 1;
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    case 0x6104:
        def = &D_801B1B30;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 546);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_801B1B58;
        obj->u.mapobj.unk_1D8 = 3;
        obj->u.mapobj.sub = 1;
        break;
    case 0x7103:
        def = &D_8018C0C0;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 557);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.anim = D_8018C0E8;
        obj->u.mapobj.unk_1D8 = 3;
        obj->u.mapobj.sub = 1;
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    case 0x7601:
        def = &D_8018C8C8;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 571);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.unk_1D8 = 2;
        obj->u.mapobj.sub = 1;
        obj->u.mapobj.unk_1DA = 0;
        obj->u.mapobj.unk_1DB = 0;
        tbl = D_800774A4[obj->u.mapobj.unk_1DB];
        obj->u.mapobj.anim = tbl[obj->u.mapobj.unk_1DA];
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    case 0x7101:
        def = &D_80188640;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 591);
        if (m == NULL) {
            return;
        }
        obj->u.mapobj.unk_1D8 = 2;
        obj->u.mapobj.sub = 1;
        obj->u.mapobj.unk_1DA = 0;
        obj->u.mapobj.unk_1DB = 1;
        tbl = D_800774A4[obj->u.mapobj.unk_1DB];
        obj->u.mapobj.anim = tbl[obj->u.mapobj.unk_1DA];
        obj->u.mapobj.unk_54.x = 60.0f;
        break;
    default:
        break;
    }
    if (m->unk_20 != 0) {
        buf = func_80042080(m->unk_20);
        m->unk_10 = buf;
        m->unk_18 = buf + (m->unk_4 << 4);
        func_80069C70(m->unk_18, 0, m->unk_20);
        func_80000EDC(m->unk_10, m->unk_C, m->unk_4 * 4);
    }
    obj->unk_C.l3.update = func_8003E078;
    obj->unk_C.l2.draw = func_8003EF9C;
    obj->u.mapobj.scale.x = 1.0f;
    obj->u.mapobj.scale.y = 1.0f;
    obj->u.mapobj.scale.z = 1.0f;
    obj->u.mapobj.unk_1B8 = 0;
    obj->u.mapobj.unk_1B4 = 0;
    obj->u.mapobj.animIndex = 0;
    obj->u.mapobj.unk_1DF = 0;
    obj->u.mapobj.unk_1E8 = m;
    obj->pos.x = src->unk_C.l0.flags.x;
    obj->pos.y = src->unk_C.l0.flags.y;
    obj->pos.z = src->unk_C.l0.flags.z;
    obj->fwd.x = src->unk_18.l0.dir.x;
    obj->fwd.y = src->unk_18.l0.dir.y;
    obj->fwd.z = src->unk_18.l0.dir.z;
    len = sqrtf(obj->fwd.x * obj->fwd.x + obj->fwd.y * obj->fwd.y + obj->fwd.z * obj->fwd.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        obj->fwd.x = obj->fwd.x * inv;
        obj->fwd.y = obj->fwd.y * inv;
        obj->fwd.z = obj->fwd.z * inv;
    } else {
    }
    obj->up.x = src->pos.x;
    obj->up.y = src->pos.y;
    obj->up.z = src->pos.z;
    len = sqrtf(obj->up.x * obj->up.x + obj->up.y * obj->up.y + obj->up.z * obj->up.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        obj->up.x = obj->up.x * inv;
        obj->up.y = obj->up.y * inv;
        obj->up.z = obj->up.z * inv;
    } else {
    }
    obj->side.x = obj->fwd.y * obj->up.z - obj->fwd.z * obj->up.y;
    obj->side.y = obj->fwd.z * obj->up.x - obj->fwd.x * obj->up.z;
    obj->side.z = obj->fwd.x * obj->up.y - obj->fwd.y * obj->up.x;
    func_8001E74C((*(u8 (*)[0x1A8-0x168])((u8 *)&obj->u.mapobj.unk_F8 + 112)), &src->unk_C.l0.flags, &src->unk_18.l0.dir, &src->pos, &obj->side);
    if (obj->unk_8.l1.unk_A == 0x4601) {
        obj->u.mapobj.speed = obj->pos.y;
    }
    obj->u.mapobj.unk_1C4 = 0;
    func_80041A80(obj, D_80115408, 4, 0);
    return;
}
