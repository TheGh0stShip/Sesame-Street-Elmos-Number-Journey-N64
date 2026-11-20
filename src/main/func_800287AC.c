/* Elmo's Number Journey: per-frame update of a guide actor driving a scripted state machine (cutscene/voice/segment loading). */
typedef struct { f32 x, y, z; } Vec3f;
typedef struct { u8 pad0[4]; u8 unk_4[1]; } Anim;
typedef struct { s16 unk_0; u16 unk_2; s16 unk_4; u8 pad6[0xA]; } Vtx16;
typedef struct { u16 unk_0; u8 pad2[6]; s32 unk_8; Vtx16 *unk_C; u8 pad10[0xC]; s32 unk_1C; } Model;
typedef struct { u8 pad0[0x40]; } Mtx64;
typedef struct { u8 pad0[0xC]; Vtx16 *unk_C; s32 unk_10; Mtx64 *unk_14; s32 unk_18; u8 pad1C[4]; u16 unk_20; } DrawInfo;
typedef struct { s32 unk_0, unk_4, unk_8; } Unk1A8;
typedef struct { u8 unk_1D4; u8 unk_1D5; } TimerPair;
typedef union { Anim *anim; TimerPair t; } Unk1D4;
typedef struct Actor {
    u8 pad0[0xC];
    s32 unk_C;
    u8 pad10[0x14];
    Vec3f pos;            /* 0x24 */
    f32 unk_30, unk_34, unk_38, unk_3C, unk_40, unk_44, unk_48, unk_4C, unk_50, unk_54;
    u8 pad58[0x90];
    Mtx64 unk_E8[3];
    Unk1A8 unk_1A8;
    s32 unk_1B4;
    s32 unk_1B8;
    Anim *unk_1BC;
    u8 pad1C0[0xC];
    s32 unk_1CC;
    u8 pad1D0[4];
    Unk1D4 unk_1D4;
    u8 pad1D8[8];
    u8 unk_1E0;
    u8 pad1E1[7];
    Model *unk_1E8;
    u8 pad1EC[8];
    DrawInfo unk_1F4;
} Actor;
typedef struct {
    u8 pad0[0x24];
    f32 unk_24, unk_28, unk_2C, unk_30, unk_34, unk_38;
    u8 pad3C[0x17C];
    f32 unk_1B8;
    u8 pad1BC[4];
    f32 unk_1C0;
    u8 pad1C4[4];
    s32 unk_1C8;
    s32 unk_1CC;
    u8 pad1D0[0x1C];
    s8 unk_1EC;
    u8 pad1ED[5];
    s8 unk_1F2;
} Player;
typedef struct {
    u8 pad0[0x10];
    void (*unk_10)();
    u8 pad14[0x1C];
    f32 unk_30, unk_34, unk_38;
    u8 pad3C[0xC];
    f32 unk_48, unk_4C, unk_50;
    u8 unk_54;
} Obj;
typedef struct { s32 unk_0, unk_4; } Pair;

void func_8000092C(Model *, DrawInfo *, Anim *, s32, Vec3f, f32 *, Mtx64 *, Actor *);
void func_80001424(void (*)(), Actor *, s32, s32, f32);
void func_800121C0(s32, s32, s32);
s32 func_80012FD0(Vec3f *);
void func_8001933C(void *, void *, s32, s32);
void func_8001FAA8(Mtx64 *, Vec3f *, Unk1A8);
s32 func_80021640(void);
f64 func_800236B4(Vec3f *);
void func_8002C424();
s32 func_8002C68C(Actor *, f32, f32, f32);
void func_80032B28();
void func_80032DB0();
void func_80035260(s32, s32, s32);
void func_800375A8(Player *, s32);
void func_80042300(Anim *);
void func_800496B8(Actor *, Player *, s32, s32);
s32 func_8004E5A0(s32);
void func_8004E884(s32, s32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
void func_800581E8(Mtx64 *, f32, f32, s32, f32, f32, f32, f32);
void func_80058BA0(void *, s32, s32);
f64 func_80069920(f64, f64);

extern s32 D_8006FE58;
extern s32 D_800717E0;
extern s32 D_80074504[], D_8007451C[], D_8007454C[], D_8007456C[], D_800745A4[], D_800745CC[], D_8007460C[],
    D_80074634[], D_80074674[], D_80074694[], D_800746CC[], D_800746EC[], D_80074AAC[], D_80074ABC[],
    D_80074ADC[], D_80074AEC[], D_80074AFC[], D_80074B0C[], D_80074B2C[], D_80074B3C[], D_80074B4C[],
    D_80074B5C[], D_80074B7C[], D_80074B8C[], D_80074B9C[], D_80074E6C[], D_80074E8C[], D_80074EA4[],
    D_80074ECC[], D_80074EEC[], D_80074F0C[];
extern Pair D_8007476C[], D_800747FC[], D_8007488C[], D_800749AC[], D_800749F4[];
extern s32 D_800A7A50;
extern u8 D_800A7A5C;
extern s32 D_800A7F6C;
extern f32 D_800A9150;
extern f32 D_800AD000;
extern u8 D_800AD03A;
extern u8 D_80106C70;
extern s32 D_80106C74;
extern f32 D_80106C78;
extern s32 D_80106C7C;
extern Player *D_80106C84;
extern f32 D_80107CB0;
extern f32 D_80107CB4;
extern u16 D_80107CBA;
extern s8 D_80107CF0;
extern s32 D_80107D10;
extern s8 D_80108FB0;
extern Obj *D_80109D00;
extern s32 D_80109D4C;
extern u8 D_80109D54;
extern s8 D_80109DF4;
extern s32 D_80109E00;
extern f32 D_8010E64C;
extern f32 D_801153FC;
extern Pair *D_80115404;
extern s32 *D_8011551C;
extern s32 D_80115520;
extern Anim *D_80118244;
extern s32 D_8012F110;
extern s8 D_8012FEC8;
extern Pair *D_8013E7F0;
extern u16 D_8013EC00;
extern u8 D_8013EEF8[];
extern u8 D_80140F30[];
extern u16 D_8029A89C;
extern Vtx16 *D_8029A8A4;
extern u8 D_004DF7C0[], D_004E0870[];
extern u8 D_0061F510[], D_00625430[], D_006289C0[], D_00637780[], D_006482F0[], D_006505B0[], D_006634C0[], D_00674C10[], D_0067C2F0[], D_00687B30[], D_00691BB0[], D_006A44D0[], D_006A99B0[], D_006B92E0[], D_006C6700[], D_006D25E0[], D_006E3450[], D_006F8CF0[], D_00701190[], D_00712000[], D_00723E50[], D_00732CF0[], D_00739C20[], D_0075F380[], D_007710B0[], D_00780C00[], D_0078E0B0[], D_007AB810[], D_007B35C0[], D_007BB850[], D_007C57E0[], D_007CE410[];

void func_800287AC(Actor *arg0) {
    s32 i;
    f32 scale;
    DrawInfo *info;
    s32 pad;
    register Actor *s;
    register Player *p;
    register f32 dx;
    register f32 dz;

    scale = 1.0f;
    s = arg0;
    s->unk_C |= 0x8000;
    if (D_800AD03A != 0) {
        if (s->unk_1E0 == 0) {
            if (s->unk_1B4 == 0x3E9 || s->unk_1B4 == 0x3EA) {
                func_800581E8(&s->unk_E8[D_800717E0], -(s->unk_1B8 * 9),
                              func_80069920(D_80106C84->unk_30, D_80106C84->unk_38) * 57.2957, 0,
                              1.0f, D_80106C84->unk_24 - D_80106C84->unk_30 * 75.0f, 10.0f,
                              D_80106C84->unk_2C - D_80106C84->unk_38 * 75.0f);
            } else {
                s->pos.y += D_8010E64C;
                func_8001FAA8(&s->unk_E8[D_800717E0], &s->pos, s->unk_1A8);
                s->pos.y -= D_8010E64C;
            }
            if (s->unk_1BC != NULL) {
                func_8000092C(s->unk_1E8, &s->unk_1F4, s->unk_1BC, s->unk_1CC, s->pos, &s->unk_30,
                              &s->unk_E8[D_800717E0], s);
            } else {
                info = &s->unk_1F4;
                info->unk_14 = &s->unk_E8[D_800717E0];
                info->unk_C = s->unk_1E8->unk_C;
                info->unk_10 = s->unk_1E8->unk_8;
                info->unk_18 = s->unk_1E8->unk_1C;
                info->unk_20 = s->unk_1E8->unk_0;
            }
            func_80001424(func_8002C424, s, 4, 1, (f32) func_800236B4(&s->pos));
        }
        return;
    }
    if (s->unk_1E0 == 0 && s->unk_1BC != NULL && !(D_80106C7C & 1)) {
        s->unk_1CC++;
        if (s->unk_1CC >= *s->unk_1BC->unk_4) {
            s->unk_C |= 8;
            if (s->unk_C & 0x80) {
                s->unk_1CC = *s->unk_1BC->unk_4;
            } else {
                s->unk_1CC = 0;
            }
        }
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            s->unk_C |= 1;
        }
        return;
    }
    switch (s->unk_1B4) {
    case 0x1:
        if (--s->unk_1B8 == 0) {
            s->unk_1B4 = 2;
            s->unk_1B8 = 0x3C;
        }
        break;
    case 0x2:
        s->unk_1B4 = 0;
        break;
    case 0x3:
        if (func_80012FD0(&s->pos) == 0) {
            s->unk_C |= 1;
            s->unk_1E0 = 1;
            D_80115520 = -1;
        }
        break;
    case 0x4:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_0061F510;
            D_800A7A50 = D_00625430 - D_0061F510;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 5;
        }
        break;
    case 0x5:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 6;
        }
        break;
    case 0x6:
        if (s->unk_1D4.t.unk_1D5 != 0) {
            s->unk_1D4.t.unk_1D5--;
        }
        D_800AD000 = (s->pos.x - -319.0f) - D_80106C84->unk_24;
        D_80107CB0 = (s->pos.z - 114.0f) - D_80106C84->unk_2C;
        if ((func_8002C68C(s, -319.0f, 114.0f, 50000.0f) != 0) && (D_80106C84->unk_1C8 == 0) && (s->unk_1D4.t.unk_1D5 == 0)) {
            D_80106C74 = (s32) D_00674C10;
            D_800A7A50 = D_0067C2F0 - D_00674C10;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 7;
            func_800496B8(s, D_80106C84, 1, 0);
        }
        break;
    case 0x7:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if ((D_8011551C == NULL) && (D_80106C84->unk_1CC >= 0x15)) {
            D_8011551C = D_80074AAC;
            func_80058BA0(D_8013EEF8, 0, 1);
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 8;
            D_80106C74 = (s32) D_0061F510;
            D_800A7A50 = D_00625430 - D_0061F510;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x8:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 9;
            D_80106C74 = (s32) D_00687B30;
            D_800A7A50 = D_00691BB0 - D_00687B30;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x9:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            D_8011551C = D_80074ADC;
            func_80058BA0(D_8013EEF8, 0, 1);
            s->unk_1B4 = 0xA;
        }
    case 0xA:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0xB;
            D_80106C74 = (s32) D_0061F510;
            D_800A7A50 = D_00625430 - D_0061F510;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074ABC;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0xB:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            s->unk_1D4.t.unk_1D4 = 0;
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0xC;
            D_80106C74 = (s32) D_00691BB0;
            D_800A7A50 = D_006A44D0 - D_00691BB0;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0xC:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        s->unk_1D4.t.unk_1D4++;
        if ((D_8011551C == NULL) && (s->unk_1D4.t.unk_1D4 >= 0x1F)) {
            D_8006FE58 |= 0x60A;
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0xD;
            D_80106C74 = (s32) D_0061F510;
            D_800A7A50 = D_00625430 - D_0061F510;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074AEC;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0xD:
        if (D_80106C84->unk_1C8 != 0x24) {
            if (D_80106C74 == 0) {
                D_8006FE58 &= ~0x60A;
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 6;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            s->unk_1D4.t.unk_1D4 = 0;
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0xE;
        }
        break;
    case 0xE:
        if (D_80106C84->unk_1C8 != 0x24) {
            D_8006FE58 &= ~0x60A;
            s->unk_1B4 = 6;
        }
        break;
    case 0xF:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_8007476C[D_8012F110];
                    D_8011551C = D_8007454C;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
                s->unk_1B4 = 0x11;
            } else {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_8007476C[D_8012F110];
                    D_8011551C = D_8007456C;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
                D_80106C74 = (s32) D_006634C0;
                D_800A7A50 = D_00674C10 - D_006634C0;
                func_80058BA0(D_80140F30, 0, 1);
                s->unk_1B4 = 0x10;
            }
        }
        break;
    case 0x10:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x11;
        }
        break;
    case 0x11:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_1B4 = 0;
            s->unk_1B4 = 3;
        }
        break;
    case 0x12:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_8007476C[D_8012F110];
                    D_8011551C = D_80074504;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
                s->unk_1B4 = 0x14;
            } else {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_8007476C[D_8012F110];
                    D_8011551C = D_8007451C;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
                D_80106C74 = (s32) D_006482F0;
                D_800A7A50 = D_006505B0 - D_006482F0;
                func_80058BA0(D_80140F30, 0, 1);
                s->unk_1B4 = 0x13;
            }
        }
        break;
    case 0x13:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x14;
        }
        break;
    case 0x14:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_54 = 50.0f;
            s->unk_1B4 = 3;
        }
        break;
    case 0x15:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_00625430;
            D_800A7A50 = D_006289C0 - D_00625430;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x16;
        }
        break;
    case 0x16:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x17;
        }
        break;
    case 0x17:
        if (D_80109D00->unk_10 == func_80032B28) {
            s->unk_1E0 = 0;
            s->unk_1B4 = 0;
            if (D_80109DF4 == 0) {
                D_80106C74 = (s32) D_006289C0;
                D_800A7A50 = D_00637780 - D_006289C0;
                func_80058BA0(D_80140F30, 0, 1);
                s->unk_1B4 = 0x18;
            }
        }
        break;
    case 0x18:
        if (D_80106C84->unk_1EC < 0) {
            s->unk_1D4.anim = s->unk_1BC;
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x19;
            if (D_8011551C == NULL) {
                if (D_80109D54 == 7) {
                    D_8011551C = D_80074E8C;
                } else {
                    D_8011551C = D_80074E6C;
                }
            }
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x19:
        if (s->unk_C & 8) {
            s->unk_1BC = s->unk_1D4.anim;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0;
            s->unk_1B4 = 0x48;
            s->unk_1D4.t.unk_1D4 = 0x78;
            D_800A7A5C = D_80109D54;
        }
        break;
    case 0x1A:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_006A44D0;
            D_800A7A50 = D_006A99B0 - D_006A44D0;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x1B;
        }
        break;
    case 0x1B:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x1C;
        }
        break;
    case 0x1C:
        if (s->unk_1D4.t.unk_1D5 != 0) {
            s->unk_1D4.t.unk_1D5--;
        }
        D_80107CB4 = (s->pos.x - -107.0f) - D_80106C84->unk_24;
        D_80106C78 = (s->pos.z - 149.0f) - D_80106C84->unk_2C;
        if ((func_8002C68C(s, -107.0f, 149.0f, 50000.0f) != 0) && (D_80106C84->unk_1C8 == 0) && (s->unk_1D4.t.unk_1D5 == 0)) {
            D_80106C74 = (s32) D_006A99B0;
            D_800A7A50 = D_006B92E0 - D_006A99B0;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x1D;
            func_800496B8(s, D_80106C84, 2, 0);
        } else {
            dx = D_80106C84->unk_24 - s->pos.x;
            dz = D_80106C84->unk_2C - s->pos.z;
            dx = func_80069920(dz, dx);
            s->unk_30 = func_800566A0(dx);
            s->unk_38 = func_80057E70(dx);
            s->unk_48 = ((s->unk_34 * s->unk_44) - (s->unk_38 * s->unk_40));
            s->unk_4C = ((s->unk_38 * s->unk_3C) - (s->unk_30 * s->unk_44));
            s->unk_50 = ((s->unk_30 * s->unk_40) - (s->unk_34 * s->unk_3C));
        }
        break;
    case 0x1D:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x1C;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if ((D_8011551C == NULL) && (D_80106C84->unk_1CC >= 0x15)) {
            D_8011551C = D_80074AFC;
            func_80058BA0(D_8013EEF8, 0, 1);
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x1E;
            D_80106C74 = (s32) D_006A44D0;
            D_800A7A50 = D_006A99B0 - D_006A44D0;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x1E:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 0x1C;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x1F;
            D_80106C74 = (s32) D_006B92E0;
            D_800A7A50 = D_006C6700 - D_006B92E0;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x1F:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x1C;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B2C;
            func_80058BA0(D_8013EEF8, 0, 1);
            s->unk_1B4 = 0x20;
        }
    case 0x20:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x1C;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x21;
            D_80106C74 = (s32) D_006C6700;
            D_800A7A50 = D_006D25E0 - D_006C6700;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074B0C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x21:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                func_80042300(s->unk_1BC);
                s->unk_1B4 = 0x1A;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if ((s->unk_C & 8) && (D_8011551C == NULL)) {
            D_8006FE58 |= 0x60A;
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x23;
            D_80106C74 = (s32) D_006A44D0;
            D_800A7A50 = D_006A99B0 - D_006A44D0;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074B3C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x23:
        if (D_80106C84->unk_1C8 != 0x25) {
            if (D_80106C74 == 0) {
                D_8006FE58 &= ~0x60A;
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 0x1C;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x22;
        }
        break;
    case 0x22:
        if (D_80106C84->unk_1C8 != 0x25) {
            D_8006FE58 &= ~0x60A;
            s->unk_1B4 = 0x1C;
        }
        break;
    case 0x24:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_800749AC[D_8012F110];
                    D_8011551C = D_800745A4;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            } else if (D_8011551C == NULL) {
                D_80115404 = &D_800749AC[D_8012F110];
                D_8013E7F0 = &D_800747FC[D_8012F110];
                D_8011551C = D_800745CC;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
            D_80106C74 = (s32) D_006E3450;
            D_800A7A50 = D_006F8CF0 - D_006E3450;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x25;
        }
        break;
    case 0x25:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                s->unk_1B4 = 0x27;
            } else {
                s->unk_1B4 = 0x26;
                D_80106C74 = (s32) D_006F8CF0;
                D_800A7A50 = D_00701190 - D_006F8CF0;
                func_80058BA0(D_80140F30, 0, 1);
            }
        }
        break;
    case 0x26:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x27;
        }
        break;
    case 0x27:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_54 = 50.0f;
            s->unk_1B4 = 3;
        }
        break;
    case 0x28:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_800749AC[D_8012F110];
                    D_8011551C = D_8007460C;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            } else if (D_8011551C == NULL) {
                D_80115404 = &D_800749AC[D_8012F110];
                D_8013E7F0 = &D_800747FC[D_8012F110];
                D_8011551C = D_80074634;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
            D_80106C74 = (s32) D_00712000;
            D_800A7A50 = D_00723E50 - D_00712000;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x29;
        }
        break;
    case 0x29:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                s->unk_1B4 = 0x2B;
            } else {
                s->unk_1B4 = 0x2A;
                D_80106C74 = (s32) D_006F8CF0;
                D_800A7A50 = D_00701190 - D_006F8CF0;
                func_80058BA0(D_80140F30, 0, 1);
            }
        }
        break;
    case 0x2A:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x2B;
        }
        break;
    case 0x2B:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_54 = 50.0f;
            s->unk_1B4 = 3;
        }
        break;
    case 0x2C:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_006A44D0;
            D_800A7A50 = D_006A99B0 - D_006A44D0;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x2D;
        }
        break;
    case 0x2D:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x2E;
        }
        break;
    case 0x2E:
        if (D_80109D00->unk_10 == func_80032B28) {
            s->unk_1E0 = 0;
            s->unk_1B4 = 0;
            if (D_80109DF4 == 1) {
                D_80106C74 = (s32) D_00723E50;
                D_800A7A50 = D_00732CF0 - D_00723E50;
                func_80058BA0(D_80140F30, 0, 1);
                s->unk_1B4 = 0x2F;
            }
        }
        break;
    case 0x2F:
        if (D_80106C84->unk_1EC < 0) {
            s->unk_1D4.anim = s->unk_1BC;
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x30;
            if (D_8011551C == NULL) {
                if (D_80109D54 == 7) {
                    D_8011551C = D_80074ECC;
                } else {
                    D_8011551C = D_80074EA4;
                }
            }
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x30:
        if (s->unk_C & 8) {
            s->unk_1BC = s->unk_1D4.anim;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0;
            s->unk_1B4 = 0x48;
            s->unk_1D4.t.unk_1D4 = 0x78;
            D_800A7A5C = D_80109D54;
        }
        break;
    case 0x31:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_00732CF0;
            D_800A7A50 = D_00739C20 - D_00732CF0;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x32;
        }
        break;
    case 0x32:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x33;
        }
        break;
    case 0x33:
        if (s->unk_1D4.t.unk_1D5 != 0) {
            s->unk_1D4.t.unk_1D5--;
        }
        D_801153FC = (s->pos.x - 5.0f) - D_80106C84->unk_24;
        D_800A9150 = (s->pos.z - 260.0f) - D_80106C84->unk_2C;
        if ((func_8002C68C(s, 5.0f, 260.0f, 30000.0f) != 0) && (D_80106C84->unk_1C8 == 0) && (s->unk_1D4.t.unk_1D5 == 0)) {
            D_80106C74 = (s32) D_0075F380;
            D_800A7A50 = D_007710B0 - D_0075F380;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x34;
            func_800496B8(s, D_80106C84, 3, 0);
        } else {
            dx = D_80106C84->unk_24 - s->pos.x;
            dz = D_80106C84->unk_2C - s->pos.z;
            dx = func_80069920(dz, dx);
            s->unk_30 = func_800566A0(dx);
            s->unk_38 = func_80057E70(dx);
            s->unk_48 = ((s->unk_34 * s->unk_44) - (s->unk_38 * s->unk_40));
            s->unk_4C = ((s->unk_38 * s->unk_3C) - (s->unk_30 * s->unk_44));
            s->unk_50 = ((s->unk_30 * s->unk_40) - (s->unk_34 * s->unk_3C));
        }
        break;
    case 0x34:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x33;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if ((D_8011551C == NULL) && (D_80106C84->unk_1CC >= 0x15)) {
            D_8011551C = D_80074B4C;
            func_80058BA0(D_8013EEF8, 0, 1);
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x35;
            D_80106C74 = (s32) D_00732CF0;
            D_800A7A50 = D_00739C20 - D_00732CF0;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x35:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 0x33;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x36;
            D_80106C74 = (s32) D_007710B0;
            D_800A7A50 = D_00780C00 - D_007710B0;
            func_80058BA0(D_80140F30, 0, 1);
        }
        break;
    case 0x36:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x33;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B7C;
            func_80058BA0(D_8013EEF8, 0, 1);
            s->unk_1B4 = 0x37;
        }
    case 0x37:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x33;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (D_8011551C == NULL) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x38;
            D_80106C74 = (s32) D_00780C00;
            D_800A7A50 = D_0078E0B0 - D_00780C00;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074B5C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x38:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                func_80042300(D_80118244);
                s->unk_1B4 = 0x31;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            D_8006FE58 |= 0x60A;
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x39;
            D_80106C74 = (s32) D_00732CF0;
            D_800A7A50 = D_00739C20 - D_00732CF0;
            func_80058BA0(D_80140F30, 0, 1);
            D_8011551C = D_80074B8C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x39:
        if (D_80106C84->unk_1C8 != 0x26) {
            if (D_80106C74 == 0) {
                D_8006FE58 &= ~0x60A;
                func_80042300(s->unk_1BC);
                s->unk_1BC = D_80118244;
                s->unk_C &= ~0x80;
                s->unk_1CC = 0;
                s->unk_1B4 = 0x33;
                s->unk_1D4.t.unk_1D5 = 0x78;
            }
            break;
        }
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x3A;
        }
        break;
    case 0x3A:
        if (D_80106C84->unk_1C8 != 0x26) {
            D_8006FE58 &= ~0x60A;
            s->unk_1B4 = 0x33;
        }
        break;
    case 0x3B:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_800749F4[D_8012F110];
                    D_8011551C = D_80074674;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            } else if (D_8011551C == NULL) {
                D_80115404 = &D_800749F4[D_8012F110];
                D_8013E7F0 = &D_8007488C[D_8012F110];
                D_8011551C = D_80074694;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
            D_80106C74 = (s32) D_007AB810;
            D_800A7A50 = D_007B35C0 - D_007AB810;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x3C;
        }
        break;
    case 0x3C:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                s->unk_1B4 = 0x3E;
            } else {
                s->unk_1B4 = 0x3D;
                D_80106C74 = (s32) D_007B35C0;
                D_800A7A50 = D_007BB850 - D_007B35C0;
                func_80058BA0(D_80140F30, 0, 1);
            }
        }
        break;
    case 0x3D:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x3E;
        }
        break;
    case 0x3E:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_54 = 50.0f;
            s->unk_1B4 = 3;
        }
        break;
    case 0x3F:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                if (D_8011551C == NULL) {
                    D_80115404 = &D_800749F4[D_8012F110];
                    D_8011551C = D_800746CC;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            } else if (D_8011551C == NULL) {
                D_80115404 = &D_800749F4[D_8012F110];
                D_8013E7F0 = &D_8007488C[D_8012F110];
                D_8011551C = D_800746EC;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
            D_80106C74 = (s32) D_007AB810;
            D_800A7A50 = D_007B35C0 - D_007AB810;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x40;
        }
        break;
    case 0x40:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            if (D_80106C70 == 0) {
                s->unk_1B4 = 0x42;
            } else {
                s->unk_1B4 = 0x41;
                D_80106C74 = (s32) D_007B35C0;
                D_800A7A50 = D_007BB850 - D_007B35C0;
                func_80058BA0(D_80140F30, 0, 1);
            }
        }
        break;
    case 0x41:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x42;
        }
        break;
    case 0x42:
        if (s->unk_C & 8) {
            func_80042300(s->unk_1BC);
            s->unk_1BC = NULL;
            D_80115520 = 1;
            s->unk_54 = 50.0f;
            s->unk_1B4 = 3;
        }
        break;
    case 0x43:
        if (D_80106C74 == 0) {
            D_80106C74 = (s32) D_00732CF0;
            D_800A7A50 = D_00739C20 - D_00732CF0;
            func_80058BA0(D_80140F30, 0, 1);
            s->unk_1B4 = 0x44;
        }
        break;
    case 0x44:
        if (D_80106C74 == 0) {
            s->unk_1BC = D_80118244;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x45;
        }
        break;
    case 0x45:
        if (D_80109D00->unk_10 == func_80032B28) {
            s->unk_1E0 = 0;
            s->unk_1B4 = 0;
            if (D_80109DF4 == 2) {
                D_80106C74 = (s32) D_007C57E0;
                D_800A7A50 = D_007CE410 - D_007C57E0;
                func_80058BA0(D_80140F30, 0, 1);
                s->unk_1B4 = 0x46;
            }
        }
        break;
    case 0x46:
        if (D_80106C84->unk_1EC < 0) {
            s->unk_1D4.anim = s->unk_1BC;
            s->unk_1BC = D_80118244;
            s->unk_C |= 0x80;
            s->unk_C &= ~8;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x47;
            if (D_8011551C == NULL) {
                if (D_80109D54 == 7) {
                    D_8011551C = D_80074F0C;
                } else {
                    D_8011551C = D_80074EEC;
                }
            }
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    case 0x47:
        if (s->unk_C & 8) {
            s->unk_1BC = s->unk_1D4.anim;
            s->unk_C &= ~0x80;
            s->unk_1CC = 0;
            s->unk_1B4 = 0x48;
            s->unk_1D4.t.unk_1D4 = 0x78;
            D_800A7A5C = D_80109D54;
        }
        break;
    case 0x48:
        if (D_800A7A5C == 7) {
            if ((s8) s->unk_1D4.t.unk_1D4 > 0) {
                if ((s8) s->unk_1D4.t.unk_1D4 == 0x78) {
                    func_8004E884(D_800A7F6C, 0);
                    func_8001933C(D_004DF7C0, D_004E0870, 0x55, 0x14);
                    D_80109E00 = func_8004E5A0(0x39);
                    D_8012FEC8 = 8;
                    D_8013EC00 = 0x708;
                    D_80107CF0 = 0;
                    dx = 800.0 - D_80106C84->unk_24;
                    dz = 700.0 - D_80106C84->unk_2C;
                    dx = func_80069920(dz, dx);
                    p = D_80106C84;
                    p->unk_1B8 = func_800566A0(dx);
                    p = D_80106C84;
                    p->unk_1C0 = func_80057E70(dx);
                    D_80108FB0 = 0;
                    D_80107D10 = 0x529518C7;
                    D_80109D00->unk_54 = 0x80;
                    D_80109D00->unk_48 = (800.0 - D_80109D00->unk_30) * 0.0078125;
                    D_80109D00->unk_4C = (300.0 - D_80109D00->unk_34) * 0.0078125;
                    D_80109D00->unk_50 = (700.0 - D_80109D00->unk_38) * 0.0078125;
                    D_80109D00->unk_10 = func_80032DB0;
                    D_80106C84->unk_1F2 = -1;
                }
                s->unk_1D4.t.unk_1D4--;
            } else {
                func_800121C0(800, 75, 700);
                if ((s8) s->unk_1D4.t.unk_1D4 == 0) {
                    func_8004E5A0(0x37);
                    func_80035260(800, 75, 700);
                    s->unk_1D4.t.unk_1D4 = -90 - (func_80021640() & 0x1F);
                } else {
                    s->unk_1D4.t.unk_1D4++;
                }
                D_8013EC00--;
                if ((D_80107CBA & 0xC00F) || D_8013EC00 == 0) {
                    func_8004E884(D_80109E00, 2);
                    func_800375A8(D_80106C84, 0x22);
                    s->unk_1B4 = 0;
                }
            }
        } else {
            if (--s->unk_1D4.t.unk_1D4 == 0) {
                func_800375A8(D_80106C84, 0x22);
                s->unk_1B4 = 0;
            }
        }
        break;
    case 0x3E8:
        s->unk_1B8 = 0;
        dx = s->pos.x - D_80106C84->unk_24;
        dz = s->pos.z - D_80106C84->unk_2C;
        dx *= dx;
        dz *= dz;
        dz += dx;
        if (dz < 18000.0f) {
            s->unk_1B4 = 0x3E9;
            for (i = 0; i != D_8029A89C; i++) {
                s->unk_1E8->unk_C[i].unk_0 = D_8029A8A4[i].unk_0;
                s->unk_1E8->unk_C[i].unk_2 = D_8029A8A4[i].unk_2;
                s->unk_1E8->unk_C[i].unk_4 = 0;
            }
            s->unk_1BC = NULL;
            s->pos.y += 100.0f;
        }
        break;
    case 0x3E9:
        if (++s->unk_1B8 == 10) {
            s->unk_1B4 = 0x3EA;
            D_8011551C = D_80074B9C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
        break;
    }
}
