typedef struct Obj { u8 pad0[0x24]; f32 x; f32 pad28; f32 z; f32 unk_30; f32 unk_34; f32 unk_38; f32 unk_3C; f32 unk_40; f32 unk_44; f32 unk_48; f32 unk_4C; f32 unk_50; u8 pad54[0xEC-0x54]; void (*unk_EC)(struct Obj *); } Obj;
extern s32 D_80115520;
extern f32 D_80112E94;
extern f32 D_80109DF0;
extern void *D_80133FB8;
f64 func_80069920(f64, f64);
f32 func_80057E70(f32);
f32 func_800566A0(f32);
void func_8003086C(Obj *);
void func_800375A8(Obj *, s32);
void func_80023308(void);
void func_8004BD1C(void *, s32);
/* faces the object toward a fixed point and computes a cross product, or switches its update handler */
void func_800386E0(Obj *o) {
    register f32 dx;
    register f32 dz;
    register Obj *p;
    if (D_80115520 == 0) {
        dx = D_80112E94 - o->x;
        dz = D_80109DF0 - o->z;
        dx = func_80069920(dz, dx);
        p = o; p->unk_30 = func_800566A0(dx);
        p = o; p->unk_38 = func_80057E70(dx);
        o->unk_48 = o->unk_34 * o->unk_44 - o->unk_38 * o->unk_40;
        o->unk_4C = o->unk_38 * o->unk_3C - o->unk_30 * o->unk_44;
        o->unk_50 = o->unk_30 * o->unk_40 - o->unk_34 * o->unk_3C;
    } else {
        o->unk_EC = func_8003086C;
        if (D_80115520 < 0) {
            func_800375A8(o, 0x16);
            func_80023308();
            func_8004BD1C(D_80133FB8, 0);
        }
    }
}
