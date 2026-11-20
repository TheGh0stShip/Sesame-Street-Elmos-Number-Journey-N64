typedef struct Vec3f { f32 x; f32 y; f32 z; } Vec3f;
typedef struct Elem40 {
    u8 unk_00[0x40];
} Elem40;
typedef struct Obj {
    u8 pad0[0xA];
    u16 unk_0A;
    s32 unk_0C;
    u8 pad10[0x14];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    u8 pad30[0xB8];
    Elem40 unk_E8[3];
    Vec3f unk_1A8;
    u8 pad1B4[0x20];
    u8 unk_1D4;
    s32 unk_1D8;
    u8 pad1DC[0xC];
    s32 unk_1E8;
    s32 unk_1EC;
    s32 unk_1F0;
} Obj;
extern u8 D_800AD03A;
extern Obj *D_80106C84;
extern s32 D_80109D4C;
extern f32 D_8012FE44;
extern s32 D_800717E0;
void func_800258C0(void);
void func_80001424(void (*)(void), Obj *, s32, s32, f32);
void func_80010D54(s32, s32, s32);
s32 func_80012FD0(f32 *);
void func_8001FAA8(Elem40 *, f32 *, Vec3f);
s32 func_8004E5A0(s32);
void func_8004E884(s32, s32);
void func_8004E990(s32, s32);
f32 func_80056690(f32);

void func_80024F58(Obj *arg0) {
    Obj *o;
    u8 pad1C[0x64];
    s32 unk88;
    s32 pad8C[1];
    s32 pad90;
    s32 flag;
    f64 d;
    s32 padA0;
    register f32 dx;
    register f32 dz;

    o = arg0;
    unk88 = 0;
    flag = 0;
    if (D_800AD03A == 0) {
        if (o->unk_0A == 0x1001) {
            dx = o->unk_24 - D_80106C84->unk_24;
            dz = o->unk_2C - D_80106C84->unk_2C;
            dx = dx * dx;
            dz = dz * dz;
            dz = dz + dx;
            D_8012FE44 = dz;
            if (dz < 30000000.0) {
                func_80010D54(o->unk_24, (s32)o->unk_28 + 10, o->unk_2C);
            }
            if (o->unk_1D4 == 0) {
                if (dz < 12000000.0) {
                    o->unk_1D4 = 1;
                    o->unk_1D8 = func_8004E5A0(0x22);
                }
            } else if (dz > 13000000.0) {
                o->unk_1D4 = 0;
                func_8004E884(o->unk_1D8, 0);
            } else {
                dx = func_80056690(dz);
                dx = 3400.0 - dx;
                if (dx < 0.0f) {
                    dx = 0.0f;
                }
                dx = dx * 0.075294118;
                func_8004E990(o->unk_1D8, dx);
            }
        }
    } else {
        d = 0;
        o = arg0;
        if (D_80109D4C >= 5) {
            if (D_80109D4C >= 8) {
                o->unk_0C = o->unk_0C | 1;
            }
            return;
        }
        if (o->unk_0A == 0x5004 || o->unk_0A == 0x5006 || o->unk_0A == 0x5505 || o->unk_0A == 0x4001 ||
            o->unk_0A == 0x4002 || o->unk_0A == 0x2505 || o->unk_0A == 0x2005 || o->unk_0A == 0x2006 ||
            o->unk_0A == 0x2009 || o->unk_0A == 0x4609 || o->unk_0A == 0x1502) {
            flag = 1;
        }
        if (flag == 0 && func_80012FD0(&o->unk_24) == 0) {
            return;
        }
        if (o->unk_0A != 0x1015 && o->unk_0A != 0x7016 && o->unk_0A != 0x7017 && o->unk_1E8 != 0) {
            func_8001FAA8(&o->unk_E8[D_800717E0], &o->unk_24, o->unk_1A8);
            func_80001424(func_800258C0, o, o->unk_1F0, 1, d);
        }
    }
}
