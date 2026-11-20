typedef struct { f32 x, y, z; } Vec3;
typedef struct { s32 v[4]; } Pad16;
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
typedef struct {
    u8 pad0[2];
    u16 unk_2;
    u8 pad4[2];
    u16 unk_6;
    s32 unk_8;
    Vec3 unk_C;
    Vec3 unk_18;
    Vec3 unk_24;
} Spawn;
typedef struct {
    u8 pad0[8];
    u16 unk_8;
    u16 unk_A;
    s32 unk_C;
    void (*unk_10)(void);
    void (*unk_14)(void);
    u8 pad18[0xC];
    Vec3 unk_24;
    Vec3 unk_30;
    Vec3 unk_3C;
    Vec3 unk_48;
    f32 unk_54;
    u8 pad58[0xE0 - 0x58];
    s32 unk_E0;
    s32 unk_E4;
    u8 padE8[0x168 - 0xE8];
    u8 unk_168[0x1A8 - 0x168];
    f32 unk_1A8;
    f32 unk_1AC;
    f32 unk_1B0;
    s32 unk_1B4;
    s32 unk_1B8;
    u8 *unk_1BC;
    u8 pad1C0[4];
    s32 unk_1C4;
    u8 pad1C8[0x1D4 - 0x1C8];
    u8 unk_1D4;
    u8 pad1D5[3];
    u8 unk_1D8;
    u8 unk_1D9;
    u8 unk_1DA;
    u8 unk_1DB;
    u8 pad1DC[3];
    u8 unk_1DF;
    f32 unk_1E0;
    u8 pad1E4[4];
    Model *unk_1E8;
    s32 unk_1EC;
    s32 unk_1F0;
    u8 pad1F4[0x218 - 0x1F4];
} Obj;

#define ASSERT(cond, line) if (cond) {} else func_80059540(D_8007B130, D_8007B134, line)

extern char D_8007B130[];
extern char D_8007B134[];
extern s32 D_80133FB0;
extern s32 D_80115408;
extern u8 **D_800774A4[];
extern Def D_80270C38;
extern u8 D_80270C60[];
extern Def D_8026A230;
extern u8 D_8026A258[];
extern Def D_8026E478;
extern u8 D_8026E4A0[];
extern Def D_801B55A0;
extern u8 D_801B55C8[];
extern Def D_801B7408;
extern u8 D_801B7430[];
extern Def D_801AC870;
extern u8 D_801AC898[];
extern Def D_801B1B30;
extern u8 D_801B1B58[];
extern Def D_8018C0C0;
extern u8 D_8018C0E8[];
extern Def D_8018C8C8;
extern Def D_80188640;
void *func_80042080(s32 size);
void func_80069C70(void *dst, s32 val, s32 size);
Model *func_800189A0(Def *def, u16 n);
void func_80059540(char *expr, char *file, s32 line);
void func_80000EDC(void *dst, void *src, s32 size);
f32 func_80056690(f32 x);
void func_8001E74C(void *arg0, Vec3 *arg1, Vec3 *arg2, Vec3 *arg3, Vec3 *arg4);
void func_80041A80(Obj *obj, s32 arg1, s32 arg2, s32 arg3);
void func_8003E078(void);
void func_8003EF9C(void);

/* creates an object from a spawn record: picks its model by type, normalises its axes */
void func_8003D3D0(Spawn *arg0) {
    Spawn *src = arg0;
    Model *m = NULL;
    Def *def = NULL;
    Obj *obj;
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
    obj->unk_8 = src->unk_2;
    obj->unk_A = src->unk_6;
    obj->unk_C = src->unk_8;
    obj->unk_1EC = D_80133FB0;
    obj->unk_E0 = 0;
    obj->unk_E4 = 0;
    obj->unk_54 = 0;
    obj->unk_1F0 = 4;
    switch (obj->unk_A) {
    case 0x6601:
        def = &D_80270C38;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 459);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_80270C60;
        obj->unk_1D8 = 1;
        break;
    case 0x5601:
        def = &D_8026A230;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 471);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_8026A258;
        obj->unk_1D8 = 1;
        break;
    case 0x5602:
        def = &D_8026E478;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 481);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_8026E4A0;
        obj->unk_1D8 = 1;
        break;
    case 0x8102:
        def = &D_801B55A0;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 494);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_801B55C8;
        obj->unk_1D8 = 1;
        obj->unk_54 = 60.0f;
        break;
    case 0x8103:
        def = &D_801B7408;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 522);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_801B7430;
        obj->unk_1D8 = 1;
        obj->unk_54 = 60.0f;
        break;
    case 0x6102:
        def = &D_801AC870;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 534);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_801AC898;
        obj->unk_1D8 = 3;
        obj->unk_1D9 = 1;
        obj->unk_54 = 60.0f;
        break;
    case 0x6104:
        def = &D_801B1B30;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 546);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_801B1B58;
        obj->unk_1D8 = 3;
        obj->unk_1D9 = 1;
        break;
    case 0x7103:
        def = &D_8018C0C0;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 557);
        if (m == NULL) {
            return;
        }
        obj->unk_1BC = D_8018C0E8;
        obj->unk_1D8 = 3;
        obj->unk_1D9 = 1;
        obj->unk_54 = 60.0f;
        break;
    case 0x7601:
        def = &D_8018C8C8;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 571);
        if (m == NULL) {
            return;
        }
        obj->unk_1D8 = 2;
        obj->unk_1D9 = 1;
        obj->unk_1DA = 0;
        obj->unk_1DB = 0;
        tbl = D_800774A4[obj->unk_1DB];
        obj->unk_1BC = tbl[obj->unk_1DA];
        obj->unk_54 = 60.0f;
        break;
    case 0x7101:
        def = &D_80188640;
        m = func_800189A0(def, def->unk_4);
        ASSERT(m != NULL, 591);
        if (m == NULL) {
            return;
        }
        obj->unk_1D8 = 2;
        obj->unk_1D9 = 1;
        obj->unk_1DA = 0;
        obj->unk_1DB = 1;
        tbl = D_800774A4[obj->unk_1DB];
        obj->unk_1BC = tbl[obj->unk_1DA];
        obj->unk_54 = 60.0f;
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
    obj->unk_10 = func_8003E078;
    obj->unk_14 = func_8003EF9C;
    obj->unk_1A8 = 1.0f;
    obj->unk_1AC = 1.0f;
    obj->unk_1B0 = 1.0f;
    obj->unk_1B8 = 0;
    obj->unk_1B4 = 0;
    obj->unk_1D4 = 0;
    obj->unk_1DF = 0;
    obj->unk_1E8 = m;
    obj->unk_24.x = src->unk_C.x;
    obj->unk_24.y = src->unk_C.y;
    obj->unk_24.z = src->unk_C.z;
    obj->unk_30.x = src->unk_18.x;
    obj->unk_30.y = src->unk_18.y;
    obj->unk_30.z = src->unk_18.z;
    len = func_80056690(obj->unk_30.x * obj->unk_30.x + obj->unk_30.y * obj->unk_30.y + obj->unk_30.z * obj->unk_30.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        obj->unk_30.x = obj->unk_30.x * inv;
        obj->unk_30.y = obj->unk_30.y * inv;
        obj->unk_30.z = obj->unk_30.z * inv;
    } else {
    }
    obj->unk_3C.x = src->unk_24.x;
    obj->unk_3C.y = src->unk_24.y;
    obj->unk_3C.z = src->unk_24.z;
    len = func_80056690(obj->unk_3C.x * obj->unk_3C.x + obj->unk_3C.y * obj->unk_3C.y + obj->unk_3C.z * obj->unk_3C.z);
    if (len >= 1e-7f) {
        inv = 1.0 / len;
        obj->unk_3C.x = obj->unk_3C.x * inv;
        obj->unk_3C.y = obj->unk_3C.y * inv;
        obj->unk_3C.z = obj->unk_3C.z * inv;
    } else {
    }
    obj->unk_48.x = obj->unk_30.y * obj->unk_3C.z - obj->unk_30.z * obj->unk_3C.y;
    obj->unk_48.y = obj->unk_30.z * obj->unk_3C.x - obj->unk_30.x * obj->unk_3C.z;
    obj->unk_48.z = obj->unk_30.x * obj->unk_3C.y - obj->unk_30.y * obj->unk_3C.x;
    func_8001E74C(obj->unk_168, &src->unk_C, &src->unk_18, &src->unk_24, &obj->unk_48);
    if (obj->unk_A == 0x4601) {
        obj->unk_1E0 = obj->unk_24.y;
    }
    obj->unk_1C4 = 0;
    func_80041A80(obj, D_80115408, 4, 0);
    return;
}
