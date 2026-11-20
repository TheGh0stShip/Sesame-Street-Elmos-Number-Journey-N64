#include "common.h"
/* title/menu scene controller: fades, mode select with the stick, and per-mode level setup */


extern void *D_80106C84;
extern u16 D_80107CBA;

void func_80019470(Scene *arg0) {
    Scene *obj;
    s32 unused1;
    s32 unused2;
    register f32 x;
    register f32 y;

    obj = arg0;
    if (D_800AD03A == 0) {
        return;
    }
    obj->timer++;
    switch (obj->state) {
    case 12:
        if (obj->timer >= obj->duration) {
            func_80042620();
            func_80042648(&D_802B5120, D_80000318 - (s32)D_002B5120);
            func_800495C0(50);
            obj->timer = 0;
            D_80109D4C = 0;
            obj->state = 10;
            D_80108D24 = 1;
            D_8012F154 = D_8012FE68;
        }
        break;
    case 18:
        D_80128443 = 0xFF;
        obj->state = 19;
        D_80108D24 = 1;
        break;
    case 0:
        if (obj->timer >= obj->duration) {
            if (!(D_8006EE70 & 1)) {
                D_80109E30 = 1;
            } else {
                func_8001933C(D_004B8E70, D_004BA590, 0x46, 0x14);
                func_8001E2CC(0x15);
                obj->timer = 0;
                obj->state = 1;
                D_80128443 = 0;
                D_80109D4C = 0;
                D_8012F154 = 0x15;
            }
        }
        break;
    case 1:
        if (D_80075094 == 0) {
            if (obj->timer >= 330) {
                obj->state = 19;
                obj->timer = 0;
            }
        } else if (D_80075094 == 4) {
            if (D_80107CBA & 0x1000) {
                obj->state = 19;
                obj->timer = 0;
            }
        } else if (obj->timer >= 330 || (D_80107CBA & 0x1000)) {
            obj->state = 19;
            obj->timer = 0;
        }
        break;
    case 4:
        if (obj->timer >= obj->duration) {
            func_8004E884(D_800A7F6C, 0);
            func_8001E2CC(0x16);
            obj->timer = 0;
            D_800A7F10 = NULL;
            D_80108FB0 = 0;
            D_8006F75C = 0;
            D_80128443 = 0xFF;
            osViBlack(0);
            D_8012F154 = 0x16;
            obj->state = 17;
            D_800A91E4 = 0;
            D_8006FE58 |= 1;
            D_8006FE58 |= 0x10;
            D_8006FE58 |= 0x20;
            D_8006FE58 |= 0x40;
        }
        break;
    case 14:
        func_8001AA30();
        x = D_80107CBC;
        y = D_80107CBD;
        D_8007509C++;
        if (D_8007509D == 0) {
            if ((D_80107CBA & 0x100) || (x > 8.0f && D_8007509C >= 16)) {
                func_8004E5A0(8);
                D_8007509D = 1;
                D_80075090++;
                D_8007509C = 0;
                if (D_80075090 >= 3) {
                    D_80075090 = 2;
                }
            }
            if ((D_80107CBA & 0x200) || (x < -8.0f && D_8007509C >= 16)) {
                func_8004E5A0(8);
                D_8007509D = 1;
                D_80075090--;
                D_8007509C = 0;
                if (D_80075090 < 0) {
                    D_80075090 = 0;
                }
            }
        }
        switch (D_80075090) {
        case 0:
            D_80106C70 = 0;
            D_800A7A58 = 6;
            D_8010E648 = 12;
            break;
        case 1:
            D_80106C70 = 1;
            D_800A7A58 = 4;
            D_8010E648 = 16;
            break;
        case 2:
            D_80106C70 = 2;
            D_800A7A58 = 2;
            D_8010E648 = 20;
            break;
        default:
            D_80106C70 = 0;
            D_800A7A58 = 6;
            D_8010E648 = 12;
            break;
        }
        if (((D_80107CBA & 0x8000) && D_8011551C == 0) || (obj->timer >= 800 && D_8011551C == 0)) {
            obj->state = 16;
            obj->timer = 0;
        }
        break;
    case 16:
        if ((D_80128443 += 0x10) < 0x10) {
            D_80128443 = 0xFF;
            osViBlack(1);
            func_8004E884(D_800A7F6C, 0);
            func_80042620();
            func_80042648(&D_802B5120, D_80000318 - (s32)D_002B5120);
            func_800495C0(50);
            obj->timer = 0;
            D_80109D4C = 0;
            obj->state = 10;
            D_8012F154 = 1;
        }
        break;
    case 10:
        if (obj->timer >= 6) {
            obj->state = 13;
            func_8001E2CC(D_8012F154);
            D_800A91E4 = 0;
            D_80115410 = D_80075030[D_8012F154] * 0.375;
            D_8013ECA0 = D_80115410 * 1.3333334f;
            D_80109DFC = 0;
            D_80106C84 = NULL;
            D_80076800 = 0;
            D_8012F150 = 0;
            D_80107CF0 = 1;
            D_800A7A70 = 0;
            D_800A7F18 = 0;
            D_800AD039 = 0;
            D_80109E28 = 0;
            func_80012E08();
            if (D_8012F154 == 1 || D_8012F154 == 2 || D_8012F154 == 11 || D_8012F154 == 19) {
                D_8006FE60 = 0;
                func_80003D68();
            }
            D_800A7F6C = 0;
            D_8010E64C = 0;
            D_80115518 = 0;
            switch (D_8012F154) {
            case 1:
                switch (D_80075090) {
                case 0:
                    D_80106C70 = 0;
                    D_800A7A58 = 6;
                    D_8010E648 = 12;
                    break;
                case 1:
                    D_80106C70 = 1;
                    D_800A7A58 = 4;
                    D_8010E648 = 16;
                    break;
                case 2:
                    D_80106C70 = 2;
                    D_800A7A58 = 2;
                    D_8010E648 = 20;
                    break;
                default:
                    D_80106C70 = 0;
                    D_800A7A58 = 6;
                    D_8010E648 = 12;
                    break;
                }
                D_800A7A74 = 0;
                D_800ACFB0 = 0;
                D_8012F158 = 0;
                D_80109DF4 = -1;
                func_8001933C(D_004E1090, D_004E7620, 0x46, 0x14);
                D_80108D20 = -1;
                D_800A7F10 = NULL;
                func_8001FBE0(&D_801AA2A4);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x4A534A53;
                D_8006FE58 &= ~1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 &= ~0x10;
                D_8006FE58 &= ~0x20;
                D_8006FE58 &= ~0x40;
                break;
            case 9:
                D_8012F158 = 0;
                func_8001933C(D_004BCB50, D_004C12F0, 0x40, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 0, D_80108D20);
                func_8001FBE0(&D_8017D82C);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x1B071B07;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 10:
                D_8012F158 = 0xCB20;
                func_8001933C(D_004B00C0, D_004B8E70, 0x40, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 10, D_80108D20);
                func_8001FBE0(&D_80174B50);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x1B071B07;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 7:
                D_8012F158 = 0;
                func_8001933C(D_004C1970, D_004C6350, 0x4B, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 0, D_80108D20);
                func_8001FBE0(&D_8018F46C);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x1B071B07;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 8:
                D_8012F158 = 0;
                func_8001933C(D_004C69A0, D_004CB8D0, 0x4B, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 10, D_80108D20);
                func_8001FBE0(&D_80177D44);
                D_80108FB0 = 0;
                D_8006F75C = 0;
                D_80107D10 = 0x4A534A53;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 5:
                D_8012F158 = 0;
                func_8001933C(D_004D0690, D_004D7C90, 0x40, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 0, D_80108D20);
                func_8001FBE0(&D_80197E10);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x4A534A53;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 6:
                D_8012F158 = 0x1ADB0;
                func_8001933C(D_004CBCD0, D_004CFEC0, 0x40, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = func_80034164(30, D_8007508C[D_80075090], 10, D_80108D20);
                func_8001FBE0(&D_80176BAC);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0xCE7FCE7F;
                D_8006FE58 |= 1;
                D_8006FE58 &= ~0x80;
                D_8006FE58 &= ~2;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                break;
            case 11:
                D_8012F158 = 0;
                func_8001933C(D_004D83F0, D_004DF7C0, 0x40, 0x14);
                if (D_80108D20 == -1) {
                    func_80033D70(&D_8007A410, 0);
                }
                D_800A7F10 = NULL;
                func_8001FBE0(&D_80167F40);
                D_80108FB0 = 1;
                D_8006F75C = 0;
                D_80107D10 = 0x4A534A53;
                D_8006FE58 &= ~1;
                D_8006FE58 |= 0x80;
                D_8006FE58 |= 2;
                D_8006FE58 &= ~0x10;
                D_8006FE58 &= ~0x20;
                D_8006FE58 &= ~0x40;
                D_80118240 = 100;
                D_800BD858 = 0;
                func_80018F60();
                break;
            default:
                return;
            }
            osViBlack(0);
        }
        break;
    case 13:
        if ((D_80128443 -= 0x10) > 0xF0) {
            D_80128443 = 0;
            obj->state = 11;
            obj->flags |= 1;
        }
        break;
    case 17:
        if ((D_80128443 -= 0x10) > 0xF0) {
            D_80128443 = 0;
            obj->state = 14;
        }
        break;
    case 19:
        if ((D_80128443 += 0x10) < 0x10) {
            D_80075094++;
            D_80128443 = 0xFF;
            if (D_80075094 >= 5) {
                osViBlack(1);
                D_80109D4C = 0;
                func_8001E2CC(0x16);
                obj->timer = 0;
                D_800A7F10 = NULL;
                D_80108FB0 = 0;
                D_8006F75C = 0;
                D_8012F154 = 0x16;
                obj->state = 17;
                D_800A91E4 = 0;
                D_8006FE58 |= 1;
                D_8006FE58 |= 0x10;
                D_8006FE58 |= 0x20;
                D_8006FE58 |= 0x40;
                func_8004E884(D_800A7F6C, 0);
                func_8001933C(D_004BA590, D_004BC400, 0x55, 0x14);
                D_80107D10 = 0xD000D;
                D_8007509D = 1;
                D_80075094 = 5;
                osViBlack(0);
            } else {
                obj->state = 20;
            }
        }
        break;
    case 20:
        if ((D_80128443 -= 0x10) > 0xF0) {
            D_80128443 = 0;
            obj->state = 1;
        }
        break;
    }
}
