typedef struct Target {
    u8 pad0[0x24];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
} Target;

typedef struct Actor {
    u8 pad0[0x24];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    u8 pad30[0x1B8 - 0x30];
    f32 unk_1B8;
    f32 unk_1BC;
    f32 unk_1C0;
    u8 pad1C4[0x1CC - 0x1C4];
    s32 unk_1CC;
    u8 pad1D0[0x1F0 - 0x1D0];
    Target *unk_1F0;
    u8 pad1F4[0x200 - 0x1F4];
    s32 unk_200;
    s32 unk_204;
} Actor;

extern u8 D_800B1090;
extern s32 D_8006FE58;
extern void *D_8011551C;
extern u16 D_80107CBA;
extern u16 D_80109DF6;
extern u16 D_8013C7C0;
extern u8 D_8012F118[];
extern s32 D_8012FE68;
extern u8 D_80074B1C[];
extern u8 D_8013EEF8[];
extern void func_80058FC0(void *);
extern void func_800375A8(Actor *, s32);
extern void func_80058BA0(void *, s32, s32);
extern f64 func_80069920(f64, f64);
extern f32 func_800566A0(f32);
extern f32 func_80057E70(f32);

/* Per-frame actor update: input-triggered state changes, then turns toward its target. */
void func_800386EC(Actor *a) {
    register f32 dx;
    register f32 dz;
    register Actor *r;

    if (D_800B1090 == 0) {
        if (D_8006FE58 & 2) {
            if (D_8011551C == NULL) {
                D_800B1090 = 1;
                a->unk_200 |= 1;
                a->unk_200 |= 2;
            }
        }
    } else if (a->unk_1CC >= 50) {
        if (D_80107CBA & 0xC00F) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(D_8012F118);
            }
            D_8012FE68 = 7;
            func_800375A8(a, 0x23);
        }
    }
    if (a->unk_1CC == 20) {
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B1C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
    }
    if (a->unk_1CC >= 20) {
        if (a->unk_204 != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(D_8012F118);
            }
            func_800375A8(a, 1);
            return;
        }
        return;
    }
    dx = a->unk_1F0->unk_24 - a->unk_24;
    dz = a->unk_1F0->unk_2C - a->unk_2C;
    dz = func_80069920(dz, dx);
    r = a;
    r->unk_1B8 = func_800566A0(dz);
    r = a;
    r->unk_1C0 = func_80057E70(dz);
}
