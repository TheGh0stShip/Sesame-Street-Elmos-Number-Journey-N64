typedef struct Obj { u8 pad0[0x24]; f32 x; f32 pad28; f32 z; u8 pad30[0x1B8-0x30]; f32 unk_1B8; f32 pad1BC; f32 unk_1C0; u8 pad1C4[0x1CC-0x1C4]; s32 unk_1CC; u8 pad1D0[0x1F0-0x1D0]; struct Obj *unk_1F0; u8 pad1F4[0x200-0x1F4]; s32 unk_200; s32 unk_204; } Obj;
extern u8 D_80107CD8;
extern s32 D_8006FE58;
extern void *D_8011551C;
extern u16 D_80107CBA;
extern u16 D_80109DF6;
extern u16 D_8013C7C0;
extern s32 D_8012FE68;
extern u8 D_8012F118[];
extern u8 D_8013EEF8[];
extern u8 D_80074B6C[];
void func_80058FC0(void *);
void func_800375A8(Obj *, s32);
void func_80058BA0(void *, s32, s32);
f64 func_80069920(f64, f64);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
/* per-frame update: input-triggered state changes, otherwise faces the target object */
void func_80038A50(Obj *o) {
    register f32 dx;
    register f32 dz;
    register Obj *p;
    if (D_80107CD8 == 0) {
        if (D_8006FE58 & 2) {
            if (D_8011551C == NULL) {
                D_80107CD8 = 1;
                o->unk_200 = o->unk_200 | 1;
                o->unk_200 = o->unk_200 | 2;
            }
        }
    } else if (o->unk_1CC >= 50) {
        if ((u16)(D_80107CBA & 0xC00F)) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(D_8012F118);
            }
            D_8012FE68 = 5;
            func_800375A8(o, 0x23);
            return;
        }
    }
    if (o->unk_1CC == 20) {
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B6C;
            func_80058BA0(D_8013EEF8, 0, 1);
        }
    }
    if (o->unk_1CC >= 20) {
        if (o->unk_204 != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                func_80058FC0(D_8012F118);
            }
            func_800375A8(o, 1);
            return;
        }
        return;
    }
    dx = o->unk_1F0->x - o->x;
    dz = o->unk_1F0->z - o->z;
    dz = func_80069920(dz, dx);
    p = o; p->unk_1B8 = func_800566A0(dz);
    p = o; p->unk_1C0 = func_80057E70(dz);
}
