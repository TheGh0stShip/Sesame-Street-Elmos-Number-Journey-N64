typedef struct {
    f32 x, y, z;
} Vec3;

typedef u8 Anim;
typedef struct { u8 b[0x80]; } Pad80;
typedef struct { u8 b[0x70]; } Pad70;
typedef struct { u8 b[0x68]; } Pad68;

typedef struct {
    u8 unk_00[8];
} Entry8;

typedef struct Actor Actor;
struct Actor {
    u8 pad0[0xC];
    s32 unk_0C;
    u8 pad10[0x18 - 0x10];
    void (*unk_18)(Actor *, s32);
    s32 unk_1C;
    u8 pad20[4];
    Vec3 pos;
    Vec3 unk_30;
    u8 pad3C[0x54 - 0x3C];
    Vec3 unk_54;
    u8 pad60[0xEC - 0x60];
    void (*unk_EC)(void);
    s32 unk_F0;
    u8 padF4[4];
    f32 mtx[2][4][4];
    u8 unk_178[0x1C8 - 0x178];
    s32 unk_1C8;
    u8 pad1CC[0x1D4 - 0x1CC];
    s32 unk_1D4;
    u8 pad1D8[0x1EC - 0x1D8];
    s8 unk_1EC;
    u8 pad1ED[0x224 - 0x1ED];
    u16 unk_224;
    u16 unk_226;
};

extern void func_8000092C(s32, u8 *, Anim *, u16, Vec3, Vec3 *, f32 (*)[4], Actor *);
extern void func_80001424(void (*)(), void *, s32, s32, f32);
extern void func_8001FAA8(f32 (*)[4], Vec3 *, Vec3);
extern s32 func_800217C4(s32);
extern void func_80033C68(Entry8 *, Entry8 *);
extern void func_80033CF0(s32, Entry8 *);
extern void func_80037508(Actor *);
extern void func_800375A8(Actor *, s32);
extern void func_80049894(Actor *);
extern void func_8004BC54(s32, Vec3 *, Vec3 *);
extern void func_8004CE30(s32, s32, s32, s32);
extern void func_8004E738(s32, s32);
extern void func_80057310(f32 (*)[4], f32 (*)[4], f32 (*)[4]);
extern void func_80057BF8(f32 (*)[4], f32, f32, f32);
extern void func_80058BA0(u8 *, s32, s32);
extern void func_80058FC0(u8 *);
extern void func_8002E154();
extern s32 D_8006FE58;
extern s32 D_800717E0;
extern Entry8 D_800743A8[];
extern Entry8 D_800743B8[];
extern s32 D_80074458[];
extern s32 D_800744C8[];
extern Entry8 D_800744D4[];
extern Entry8 D_800744EC[];
extern Entry8 D_80074964[];
extern Entry8 D_800749AC[];
extern Entry8 D_800749F4[];
extern u8 D_80074A3C[];
extern u8 D_80074A4C[];
extern u8 D_80074A5C[];
extern u8 D_80074A6C[];
extern u8 D_80074A7C[];
extern u8 D_80074A8C[];
extern u8 D_80074A9C[];
extern Anim *D_800768A0[];
extern u16 D_800A7F18;
extern u16 D_800A7F6A;
extern u8 D_800A91E4;
extern u8 D_800AD039;
extern u8 D_800AD03A;
extern Actor *D_80106C84;
extern u16 D_80107CBA;
extern s32 D_80109D4C;
extern u16 D_80109DF6;
extern u8 *D_8011551C;
extern s32 D_80115524;
extern u8 D_80128443;
extern s32 D_8012F110;
extern u8 D_8012F118[];
extern u8 D_8012F150;
extern s32 D_8012F154;
extern s32 D_8012FE68;
extern s32 D_80133FB8;
extern s16 D_8013C7C0;
extern u8 D_8013EEF8[];

/* anim is a byte record: anim[4] is its frame count */
/* per-frame update of a scene actor: timed hints/flags, animation, callbacks */
void func_8002D0A4(void *arg) {
    Actor *obj;
    s32 unk2C;
    Pad80 unk30;
    Actor *it;
    f32 scale = 1.0f;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    Anim *anim;
    Pad70 unkC8;
    s32 r = 0;
    s32 g = 0;
    s32 b = 0;
    s32 unk144;
    Pad68 unk148;
    f32 mf[4][4];
    s32 unk1F0[4];

    obj = arg;
    anim = D_800768A0[obj->unk_1D4];
    if (D_800AD03A != 0) {
        func_8001FAA8(obj->mtx[D_800717E0], &obj->pos, obj->unk_54);
        if (D_8012F154 == 6 && obj->unk_1EC != 0) {
            func_80057BF8(mf, 0.0f, (f32)obj->unk_1EC * 1.40625, 0.0f);
            func_80057310(mf, obj->mtx[D_800717E0], obj->mtx[D_800717E0]);
        }
        func_8000092C(obj->unk_F0, obj->unk_178, anim, obj->unk_224 >> 4, obj->pos, &obj->unk_30, obj->mtx[D_800717E0], obj);
        func_80001424(func_8002E154, obj, 4, 1, 0);
        return;
    }
    if (D_8012F150 != 0) {
        if (D_8012F150 == 1) {
            func_8004E738(1, 0);
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(D_8012F118);
            }
            D_8012F150 = 2;
        }
        if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
            func_8004BC54(D_80133FB8, &obj->pos, &obj->unk_30);
        }
        D_800A91E4 = 1;
        D_80128443 = 0x80;
        switch (D_8012F154) {
            case 1:
            case 2:
                if (D_800A7F6A % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 |= 0x60A;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A3C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 9:
            case 10:
                if (D_800A7F6A % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A4C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                } else if (D_800A7F6A % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A5C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 7:
            case 8:
                if (D_800A7F6A % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A6C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                } else if (D_800A7F6A % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A7C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 5:
            case 6:
                if (D_800A7F6A % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A8C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                } else if (D_800A7F6A % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A9C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 11:
                if (D_800A7F6A % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A3C;
                        func_80058BA0(D_8013EEF8, 0, 1);
                    }
                } else if (D_800A7F6A % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                }
                break;
        }
        if (D_800A7F6A < 7198) {
            D_800A7F6A++;
        }
        if (!(D_8012F154 != 1 && D_8012F154 != 2)) {
            if (D_800A7F6A >= 331 && (D_80107CBA & 0xC00F)) {
                D_800A91E4 = 0;
                D_80128443 = 0;
                D_8012F150 = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    func_80058FC0(D_8012F118);
                }
                return;
            }
        } else {
            if (D_800A7F6A >= 901 && (D_80107CBA & 0x1000)) {
                D_8012FE68 = 1;
                D_800A91E4 = 0;
                D_80128443 = 0;
                D_8012F150 = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    func_80058FC0(D_8012F118);
                }
                func_800375A8(obj, 0x23);
                return;
            }
            if (D_800A7F6A >= 331 && (D_80107CBA & 0xC00F)) {
                D_800A91E4 = 0;
                D_80128443 = 0;
                D_8012F150 = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    func_80058FC0(D_8012F118);
                }
                return;
            }
        }
        return;
    }
    obj->unk_224 += obj->unk_226;
    if (obj->unk_224 >= anim[4] << 4) {
        obj->unk_0C |= 8;
        if (obj->unk_0C & 0x80) {
            obj->unk_224 = anim[4] << 4;
        } else {
            obj->unk_224 = 0;
        }
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            obj->unk_0C |= 1;
        }
        return;
    }
    obj->unk_0C |= 0x8000;
    if (D_80106C84->unk_1C8 != 0x22 && D_80106C84->unk_1C8 != 0x23 && D_800A7F18 >= 3601 && D_800AD039 < 3 && D_8011551C == NULL) {
        switch (D_8012F154) {
            case 9:
                func_80033C68(&D_800743A8[func_800217C4(2)], &D_80074964[D_8012F110]);
                break;
            case 10:
                func_80033C68(&D_800743B8[func_800217C4(3)], &D_80074964[D_8012F110]);
                break;
            case 7:
                func_80033CF0(D_80074458[func_800217C4(4)], &D_800749AC[D_8012F110]);
                break;
            case 8:
                func_80033CF0(D_800744C8[func_800217C4(3)], &D_800749AC[D_8012F110]);
                break;
            case 5:
                func_80033C68(&D_800744D4[func_800217C4(3)], &D_800749F4[D_8012F110]);
                break;
            case 6:
                func_80033C68(&D_800744EC[func_800217C4(3)], &D_800749F4[D_8012F110]);
                break;
        }
    }
    it = obj;
    if (it->unk_18 != NULL) {
        while (it->unk_1C != 0) {
            it->unk_18(it, it->unk_1C);
            func_80049894(it);
        }
    }
    if (obj->unk_EC != NULL) {
        obj->unk_EC();
    }
    func_80037508(obj);
    if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
        func_8004BC54(D_80133FB8, &obj->pos, &obj->unk_30);
    }
    if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
        func_8004CE30(D_80115524, r, g, b);
    }
}
