typedef struct { s32 m[16]; } Mtx;
typedef struct { u8 unk_00[0x24]; f32 x; f32 y; f32 z; } Target;
typedef struct Cam {
    u8 unk_00[0x24];
    f32 eyeX, eyeY, eyeZ;
    f32 atX, atY, atZ;
    f32 upX, upY, upZ;
    f32 velX, velY, velZ;
    u8 timer;
} Cam;

void func_80001424(void *, Cam *, s32, s32, s32);
f32 func_80056690(f32);
void func_800567F0(Mtx *, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_80056F00(Mtx *, s32);
void func_80057180(Mtx *, Mtx *, Mtx *);
void func_80057410(Mtx *, u16 *, f32, f32, f32, f32, f32);
extern u8 D_80032894[];
extern u16 D_800A7F00;
extern u8 D_800AD03A;
extern Target *D_80106C84;
extern s32 D_8013EF28;

/* Camera update: builds the view matrices, or follows the player while easing in. */
void func_80032DB0(Cam *arg0) {
    register Cam *c = arg0;
    Mtx persp;
    Mtx look;
    Mtx view;
    register f32 dx;
    register f32 dz;
    register f32 s;
    register f64 d;

    if (D_800AD03A != 0) {
        func_80057410(&persp, &D_800A7F00, 40.0f, 1.3333334f, 50.0f, 10000.0f, 1.0f);
        func_800567F0(&look, c->eyeX, c->eyeY, c->eyeZ, c->atX, c->atY, c->atZ, c->upX, c->upY, c->upZ);
        func_80057180(&look, &persp, &view);
        func_80056F00(&view, D_8013EF28);
        func_80001424(D_80032894, c, 1, 0, 0);
    } else {
        if (c->timer != 0) {
            c->timer--;
            c->atX += c->velX;
            c->atY += c->velY;
            c->atZ += c->velZ;
        }
        dx = D_80106C84->x - c->atX;
        dz = D_80106C84->z - c->atZ;
        d = 844.0 - (f32)((0x80 - c->timer) * 6);
        s = d / func_80056690(dx * dx + dz * dz);
        dx *= s;
        dz *= s;
        s = c->timer;
        s *= 3.8125;
        c->eyeX = dx + D_80106C84->x - (0x80 - c->timer);
        c->eyeY = s + 25.0;
        c->eyeZ = D_80106C84->z + dz;
    }
}
