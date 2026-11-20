typedef struct Obj {
    u8 pad0[0x24];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
    f32 unk_40;
    f32 unk_44;
} Obj;
typedef f32 Mtx44[4][4];
extern u8 D_800AD03A;
extern u16 D_800B1092;
extern u8 D_800A7F00[];
extern s32 D_80108FB4;
extern s32 D_8013EF28;
extern Obj *D_80106C84;
void func_80032894(void);
void func_80001424(void (*)(void), Obj *, s32, s32, s32);
f32 func_80056690(f32);
void func_800567F0(Mtx44 *, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_80056F00(Mtx44 *, s32);
void func_80057180(Mtx44 *, Mtx44 *, Mtx44 *);
void func_80057410(Mtx44 *, u8 *, s32, f32, f32, f32, f32);

/* builds the object's view matrix, or places it 1100 units from the player at height 225 */
void func_80032924(Obj *arg0) {
    Mtx44 m0;
    Mtx44 m1;
    Mtx44 m2;
    register Obj *o = arg0;
    register f32 dx;
    register f32 dz;
    register f32 s;

    if (D_800AD03A != 0) {
        func_80057410(&m0, D_800A7F00, D_80108FB4, 320.0 / D_800B1092, 50.0f, 10000.0f, 1.0f);
        func_800567F0(&m1, o->unk_24, o->unk_28, o->unk_2C, o->unk_30, o->unk_34, o->unk_38, o->unk_3C, o->unk_40, o->unk_44);
        func_80057180(&m1, &m0, &m2);
        func_80056F00(&m2, D_8013EF28);
        func_80001424(func_80032894, o, 1, 0, 0);
    } else {
        dx = D_80106C84->unk_24 - o->unk_30;
        dz = D_80106C84->unk_2C - o->unk_38;
        s = 1100.0 / func_80056690(dx * dx + dz * dz);
        dx = dx * s;
        dz = dz * s;
        o->unk_24 = D_80106C84->unk_24 + dx;
        o->unk_28 = 225.0f;
        o->unk_2C = D_80106C84->unk_2C + dz;
    }
}
