typedef struct Obj { u8 pad0[0x1CC]; s32 unk_1CC; } Obj;
typedef struct { s32 flags; s32 unk_4; s32 unk_8; } Entry;
extern s32 D_8012FE68;
extern void *D_8011551C;
extern u8 D_8013EEF8[];
extern u8 D_80074F24[], D_80074A4C[], D_80074A5C[], D_80074A6C[], D_80074A7C[], D_80074A8C[], D_80074A9C[], D_80074370[];
extern s32 D_800A7A74;
extern s32 D_800A7A58;
extern Entry D_8013ECA8[];
extern s32 D_8012F154;
extern void *D_800A7F6C;
extern s32 D_8006FE58;
extern u8 D_800A7A5C;
extern u8 D_80128443;
extern u8 D_80076B60;
extern u8 D_8006FE60;
extern u16 D_80107CBA;
extern s32 D_800ACFB0;
extern u16 D_80109DF6;
extern u16 D_8013C7C0;
extern u8 D_8012F118[];
void func_80058BA0(void *, s32, s32);
void func_800375A8(Obj *, s32);
void func_8004E884(void *, s32);
extern u8 D_004E7620[], D_004E9810[], D_004B8E70[], D_004BA590[], D_004E0870[], D_004E1090[];
void func_8001933C(void *, void *, s32, s32);
void func_8004E990(void *, s32);
void *func_8004E5A0(s32);
void func_80003D68(void);
void func_80058FC0(void *);
/* timeline script for a cutscene object: plays sounds/streams at fixed frame counts per scene */
void func_8003C494(Obj *o) {
    if (D_8012FE68 == 11) {
        if (o->unk_1CC == 150) {
            if (D_8011551C == NULL) {
                D_8011551C = D_80074F24;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        }
        if (o->unk_1CC == 400) {
            func_800375A8(o, 0x23);
        }
        return;
    }
    if (D_800A7A74 == D_800A7A58) {
        if (!(D_8013ECA8[D_800A7A74 - 1].flags & 2)) {
            o->unk_1CC = 0;
            return;
        }
    }
    if (D_8012F154 != 11 && D_8012F154 != 19 && o->unk_1CC == 270) {
        func_8004E884(D_800A7F6C, 0);
        func_8001933C(D_004E7620, D_004E9810, 0x40, 0x14);
    }
    switch (D_8012F154) {
    case 9:
    case 10:
        if (o->unk_1CC == 300) {
            D_8006FE58 &= ~0x10;
            D_8006FE58 |= 0x60A;
            if (D_8011551C == NULL) {
                D_8011551C = D_80074A4C;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        } else if (o->unk_1CC == 900) {
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
        if (o->unk_1CC == 300) {
            D_8006FE58 &= ~0x10;
            D_8006FE58 |= 0x60A;
            if (D_8011551C == NULL) {
                D_8011551C = D_80074A6C;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        } else if (o->unk_1CC == 900) {
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
        if (o->unk_1CC == 300) {
            D_8006FE58 &= ~0x10;
            D_8006FE58 |= 0x60A;
            if (D_8011551C == NULL) {
                D_8011551C = D_80074A8C;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        } else if (o->unk_1CC == 900) {
            D_8006FE58 &= ~0x608;
            D_8006FE58 |= 4;
            if (D_8011551C == NULL) {
                D_8011551C = D_80074A9C;
                func_80058BA0(D_8013EEF8, 0, 1);
            }
        }
        break;
    case 11:
        if (D_800A7A5C == 7) {
            if (o->unk_1CC >= 200 && o->unk_1CC < 250) {
                if (D_8011551C == NULL) {
                    D_8011551C = D_80074370;
                    func_80058BA0(D_8013EEF8, 0, 1);
                }
            }
            if (o->unk_1CC == 300) {
                D_80128443 = 0;
                D_80076B60 = 1;
                func_8004E884(D_800A7F6C, 0);
                func_8001933C(D_004B8E70, D_004BA590, 0x46, 0x14);
            }
            if (o->unk_1CC >= 351) {
                if (D_80076B60 == 0) {
                    func_8004E884(D_800A7F6C, 0);
                    D_800A7A5C = 0;
                    func_800375A8(o, 0x23);
                }
            }
        } else if (D_8006FE60 == 3) {
            if (o->unk_1CC == 130) {
                func_800375A8(o, 0x23);
            }
        } else {
            if (o->unk_1CC == 200) {
                func_8004E990(func_8004E5A0(0x33), 100);
            }
            if (o->unk_1CC == 470) {
                func_8004E884(D_800A7F6C, 0);
                func_8001933C(D_004E0870, D_004E1090, 0x55, 0x14);
            }
            if (o->unk_1CC == 730) {
                func_800375A8(o, 0x23);
            }
        }
        break;
    }
    if (D_8012F154 == 11 || D_8012F154 == 19) {
        return;
    }
    if (((D_80107CBA & 0xC00F) && o->unk_1CC < 600) || o->unk_1CC >= 1801) {
        if (D_800A7A74 == D_800A7A58) {
            D_800ACFB0 = 0;
            D_800A7A74 = 0;
            func_80003D68();
        }
        if (D_8011551C != NULL) {
            D_8013C7C0 = D_80109DF6 >> 3;
            func_80058FC0(D_8012F118);
        }
        D_8006FE58 &= ~0x60E;
        func_800375A8(o, 0x23);
    }
    if ((D_80107CBA & 0x1000) && o->unk_1CC < 1200) {
        D_800ACFB0 = 0;
        D_800A7A74 = 0;
        D_8012FE68 = 1;
        if (D_8011551C != NULL) {
            D_8013C7C0 = D_80109DF6 >> 3;
            func_80058FC0(D_8012F118);
        }
        D_8006FE58 &= ~0x60E;
        func_800375A8(o, 0x23);
    }
}
