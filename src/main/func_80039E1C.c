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
    f32 unk_48;
    f32 unk_4C;
    f32 unk_50;
} Obj;
extern f32 D_80109DF0;
extern f32 D_80112E94;
extern s32 D_80115520;
extern s32 D_80133FB8;
void func_80023308(void);
void func_800375A8(Obj *, s32);
void func_8004BD1C(s32, s32);
f32 func_800566A0(f32);
f32 func_80057E70(f32);
f64 func_80069920(f64, f64);

/* aims the object at a target point and rebuilds its cross-product vector */
void func_80039E1C(Obj *arg0) {
    register f32 dx;
    register f32 dz;
    register Obj *o;

    if (D_80115520 == 0) {
        dx = D_80112E94 - arg0->unk_24;
        dz = D_80109DF0 - arg0->unk_2C;
        dx = func_80069920(dz, dx);
        o = arg0;
        o->unk_30 = func_800566A0(dx);
        o = arg0;
        o->unk_38 = func_80057E70(dx);
        arg0->unk_48 = arg0->unk_34 * arg0->unk_44 - arg0->unk_38 * arg0->unk_40;
        arg0->unk_4C = arg0->unk_38 * arg0->unk_3C - arg0->unk_30 * arg0->unk_44;
        arg0->unk_50 = arg0->unk_30 * arg0->unk_40 - arg0->unk_34 * arg0->unk_3C;
    } else if (D_80115520 < 0) {
        func_800375A8(arg0, 0);
        func_80023308();
        func_8004BD1C(D_80133FB8, 0);
    }
}
