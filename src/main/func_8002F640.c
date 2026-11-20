typedef struct { u8 pad0[0x1AC]; f32 unk_1AC; f32 pad1B0; f32 unk_1B4; u8 pad1B8[0x1D8-0x1B8]; f32 unk_1D8; f32 unk_1DC; f32 unk_1E0; f32 pad1E4; f32 unk_1E8; } Obj;
extern Obj *D_80106C84;
f64 func_80069920(f64, f64);
f32 func_80057E70(f32);
f32 func_800566A0(f32);
/* turns the object toward an angle: wraps the difference to [-pi, pi] and sets velocity from speed */
void func_8002F640(register f32 ang) {
    register Obj *o = D_80106C84;
    register f32 d;
    d = func_80069920(-o->unk_1AC, -o->unk_1B4);
    d -= ang;
    if (d > 3.14159) {
        d = d - 6.2831853;
    } else if (d < -3.14159) {
        d = d + 6.2831853;
    }
    ang -= d;
    o->unk_1E0 = func_80057E70(ang) * o->unk_1D8;
    o->unk_1E8 = func_800566A0(ang) * o->unk_1D8;
    o->unk_1D8 = 0;
    if (d >= 0) {
        o->unk_1DC = 0.15f;
    } else {
        o->unk_1DC = -0.15f;
    }
}
