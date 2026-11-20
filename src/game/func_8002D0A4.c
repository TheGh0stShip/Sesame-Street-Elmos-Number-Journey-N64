#include "common.h"
#include "game_structs.h"


typedef u8 Anim;
typedef struct { u8 b[0x80]; } Pad80;


extern Event D_800743A8[];
extern Event D_800743B8[];
extern Event D_800744D4[];
extern Event D_800744EC[];
extern Event D_80074964[];
extern Event D_800749AC[];
extern Event D_800749F4[];
extern Anim *D_800768A0[];
extern Actor *D_80106C84;
extern u16 D_80107CBA;

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
    anim = D_800768A0[obj->u.model.animIndex.s];
    if (D_800AD03A != 0) {
        ((void (*)(f32 (*)[4], Vec3f *, Vec3f))func_8001FAA8)(obj->u.model.unk_E8.l3.mtx[D_800717E0], &obj->pos, obj->u.model.unk_54);
        if (D_8012F154 == 6 && obj->u.model.unk_1EC != 0) {
            guRotateRPY(mf, 0.0f, (f32)obj->u.model.unk_1EC * 1.40625, 0.0f);
            guMtxCatL(mf, obj->u.model.unk_E8.l3.mtx[D_800717E0], obj->u.model.unk_E8.l3.mtx[D_800717E0]);
        }
        ((void (*)(s32, u8 *, Anim *, u16, Vec3f, Vec3f *, f32 (*)[4], Actor *))func_8000092C)(obj->u.model.unk_E8.l2.unk_F0, (*(u8 (*)[0x1C8-0x178])((u8 *)&obj->u.model.unk_E8.l1.unk_E8 + 144)), anim, obj->u.model.unk_224 >> 4, obj->pos, &obj->fwd, obj->u.model.unk_E8.l3.mtx[D_800717E0], obj);
        addActorDL(func_8002E154, obj, 4, 1, 0);
        return;
    }
    if ((*(u8 (*))&D_8012F150) != 0) {
        if ((*(u8 (*))&D_8012F150) == 1) {
            func_8004E738(1, 0);
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(D_8012F118);
            }
            (*(u8 (*))&D_8012F150) = 2;
        }
        if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
            func_8004BC54(D_80133FB8, &obj->pos, &obj->fwd);
        }
        D_800A91E4 = 1;
        D_80128443 = 0x80;
        switch (D_8012F154) {
            case 1:
            case 2:
                if ((*(u16 (*))&D_800A7F6A) % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 |= 0x60A;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A3C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 9:
            case 10:
                if ((*(u16 (*))&D_800A7F6A) % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A4C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                } else if ((*(u16 (*))&D_800A7F6A) % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A5C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 7:
            case 8:
                if ((*(u16 (*))&D_800A7F6A) % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A6C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                } else if ((*(u16 (*))&D_800A7F6A) % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A7C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 5:
            case 6:
                if ((*(u16 (*))&D_800A7F6A) % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A8C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                } else if ((*(u16 (*))&D_800A7F6A) % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A9C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                }
                break;
            case 11:
                if ((*(u16 (*))&D_800A7F6A) % 1800 == 30) {
                    D_8006FE58 &= ~0x70;
                    D_8006FE58 &= ~0x10;
                    D_8006FE58 |= 0x60A;
                    D_8006FE58 &= ~4;
                    if (D_8011551C == NULL) {
                        D_8011551C = D_80074A3C;
                        osSendMesg(D_8013EEF8, 0, 1);
                    }
                } else if ((*(u16 (*))&D_800A7F6A) % 1800 == 600) {
                    D_8006FE58 &= ~0x608;
                    D_8006FE58 |= 4;
                }
                break;
        }
        if ((*(u16 (*))&D_800A7F6A) < 7198) {
            (*(u16 (*))&D_800A7F6A)++;
        }
        if (!(D_8012F154 != 1 && D_8012F154 != 2)) {
            if ((*(u16 (*))&D_800A7F6A) >= 331 && (D_80107CBA & 0xC00F)) {
                D_800A91E4 = 0;
                D_80128443 = 0;
                (*(u8 (*))&D_8012F150) = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    osStopTimer(D_8012F118);
                }
                return;
            }
        } else {
            if ((*(u16 (*))&D_800A7F6A) >= 901 && (D_80107CBA & 0x1000)) {
                D_8012FE68 = 1;
                D_800A91E4 = 0;
                D_80128443 = 0;
                (*(u8 (*))&D_8012F150) = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    osStopTimer(D_8012F118);
                }
                func_800375A8(obj, 0x23);
                return;
            }
            if ((*(u16 (*))&D_800A7F6A) >= 331 && (D_80107CBA & 0xC00F)) {
                D_800A91E4 = 0;
                D_80128443 = 0;
                (*(u8 (*))&D_8012F150) = 0;
                D_8006FE58 |= 0x70;
                D_8006FE58 &= ~0x60E;
                if (D_8011551C != NULL) {
                    D_8013C7C0 = D_80109DF6 >> 3;
                    osStopTimer(D_8012F118);
                }
                return;
            }
        }
        return;
    }
    obj->u.model.unk_224 += obj->u.model.unk_226;
    if (obj->u.model.unk_224 >= anim[4] << 4) {
        obj->unk_C.l1.flags |= 8;
        if (obj->unk_C.l1.flags & 0x80) {
            obj->u.model.unk_224 = anim[4] << 4;
        } else {
            obj->u.model.unk_224 = 0;
        }
    }
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            obj->unk_C.l1.flags |= 1;
        }
        return;
    }
    obj->unk_C.l1.flags |= 0x8000;
    if (D_80106C84->u.model.unk_1C8 != 0x22 && D_80106C84->u.model.unk_1C8 != 0x23 && (*(u16 (*))&D_800A7F18) >= 3601 && D_800AD039 < 3 && D_8011551C == NULL) {
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
    if (it->unk_18.l1.unk_18 != NULL) {
        while (it->unk_18.l1.unk_1C != 0) {
            it->unk_18.l1.unk_18(it, it->unk_18.l1.unk_1C);
            func_80049894(it);
        }
    }
    if (obj->u.model.unk_E8.l2.unk_EC != NULL) {
        obj->u.model.unk_E8.l2.unk_EC();
    }
    func_80037508(obj);
    if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
        func_8004BC54(D_80133FB8, &obj->pos, &obj->fwd);
    }
    if (D_8012F154 != 0xB && D_8012F154 != 0x13) {
        ((void (*)(s32, s32, s32, s32))func_8004CE30)(D_80115524, r, g, b);
    }
}
