typedef struct Actor {
    u8 pad0[0x24];
    f32 unk_24; f32 unk_28; f32 unk_2C;
    u8 pad30[0x1B8 - 0x30];
    f32 unk_1B8; f32 unk_1BC; f32 unk_1C0;
    u8 pad1C4[0x1CC - 0x1C4];
    s32 unk_1CC;
    u8 pad1D0[0x1F0 - 0x1D0];
    struct Actor *unk_1F0;
    u8 pad1F4[0x200 - 0x1F4];
    s32 unk_200;
    s32 unk_204;
} Actor;
#define SINF(x) ({ func_800566A0(x); })
#define COSF(x) ({ func_80057E70(x); })
extern s32 D_8006FE58;
extern s32 D_80074ACC;
extern u8 D_800B1098;
extern u16 D_80107CBA;
extern u16 D_80109DF6;
extern s32 *D_8011551C;
extern s32 D_8012F118;
extern s32 D_8012FE68;
extern s16 D_8013C7C0;
extern s32 D_8013EEF8;
void func_800375A8(Actor *a, s32 state);
f32 func_800566A0(f32 a);
f32 func_80057E70(f32 a);
void func_80058BA0(s32 *a, s32 b, s32 c);
void func_80058FC0(s32 *a);
f64 func_80069920(f64 y, f64 x);

/* Actor update: state changes on input/timer, then faces its target actor. */
void func_80038388(Actor *arg0) {
    register f32 dx;
    register f32 dz;

    if (D_800B1098 == 0) {
        if ((D_8006FE58 & 2) && D_8011551C == NULL) {
            D_800B1098 = 1;
            arg0->unk_200 |= 1;
            arg0->unk_200 |= 2;
        }
    } else if (arg0->unk_1CC >= 50 && (u16)(D_80107CBA & 0xC00F)) {
        if (D_8011551C != NULL) {
            D_8013C7C0 = D_80109DF6 >> 3;
            func_80058FC0(&D_8012F118);
        }
        D_8012FE68 = 9;
        func_800375A8(arg0, 0x23);
    }
    if (arg0->unk_1CC == 20 && D_8011551C == NULL) {
        D_8011551C = &D_80074ACC;
        func_80058BA0(&D_8013EEF8, 0, 1);
    }
    if (arg0->unk_1CC >= 20) {
        if (arg0->unk_204 != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(&D_8012F118);
            }
            func_800375A8(arg0, 1);
            return;
        }
        return;
    }
    dx = arg0->unk_1F0->unk_24 - arg0->unk_24;
    dz = arg0->unk_1F0->unk_2C - arg0->unk_2C;
    dz = func_80069920(dz, dx);
    arg0->unk_1B8 = SINF(dz);
    arg0->unk_1C0 = COSF(dz);
}
