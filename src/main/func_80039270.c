typedef struct {
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
extern void func_80023308(void);
extern void func_800375A8(Obj *, s32);
extern void func_8004BD1C(s32, s32);
extern f32 func_800566A0(f32);
extern f32 func_80057E70(f32);
extern f64 func_80069920(f64, f64);
extern f32 D_80109DF0;
extern f32 D_80112E94;
extern s32 D_80115520;
extern s32 D_80133FB8;

/* turns obj toward a target point and rebuilds a cross product, or ends on negative state */
void func_80038EEC(Obj *obj) {
    register f32 a;
    register f32 b;

    if (D_80115520 == 0) {
        a = D_80112E94 - obj->unk_24;
        b = D_80109DF0 - obj->unk_2C;
        a = func_80069920(b, a);
        obj->unk_30 = (a = a, func_800566A0(a));
        obj->unk_38 = (a = a, func_80057E70(a));
        obj->unk_48 = obj->unk_34 * obj->unk_44 - obj->unk_38 * obj->unk_40;
        obj->unk_4C = obj->unk_38 * obj->unk_3C - obj->unk_30 * obj->unk_44;
        obj->unk_50 = obj->unk_30 * obj->unk_40 - obj->unk_34 * obj->unk_3C;
    } else if (D_80115520 < 0) {
        func_800375A8(obj, 0xE);
        func_80023308();
        func_8004BD1C(D_80133FB8, 0);
    }
}
