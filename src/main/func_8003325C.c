typedef struct Obj { u8 pad0[0xC]; s32 unk_0C; s32 unk_10; u8 pad14[0x10]; f32 unk_24; f32 unk_28; f32 unk_2C; } Obj;
extern u8 D_800AD03A;
extern s32 D_80109D4C;
extern Obj *D_80106C84;
void func_800375A8(Obj *, s32);
/* distance check against player object; triggers on close approach */
void func_8003325C(Obj *arg0) {
    register Obj *o = arg0;
    register f32 dx;
    register f32 dz;
    s32 pad;
    if (D_800AD03A != 0) return;
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) o->unk_0C |= 1;
        return;
    }
    dx = o->unk_24 - D_80106C84->unk_24;
    dz = o->unk_2C - D_80106C84->unk_2C;
    dx = dx * dx;
    dz = dz * dz;
    dz = dz + dx;
    if (dz < 300000.0) {
        func_800375A8(D_80106C84, 0x22);
        o->unk_0C |= 1;
        o->unk_10 = 0;
    }
}
