typedef struct {
    u8 unk_00[0x24];
    f32 eye[3];
    f32 at[3];
    f32 up[3];
    f32 vel[3];
    u8 timer;
} Cam;
typedef struct {
    u8 unk_00[0x24];
    f32 x;
    f32 y;
    f32 z;
} Actor;
extern u8 D_800AD03A;
extern u16 D_800A7F00;
extern s32 D_8013EF28;
extern Actor *D_80106C84;
void func_80032894(void);
void func_80057410(f32 m[4][4], u16 *persp, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void func_800567F0(f32 m[4][4], f32 ex, f32 ey, f32 ez, f32 ax, f32 ay, f32 az, f32 ux, f32 uy, f32 uz);
void func_80057180(f32 a[4][4], f32 b[4][4], f32 out[4][4]);
void func_80056F00(f32 m[4][4], s32 dst);
void func_80001424(void (*fn)(void), void *arg, s32 a, s32 b, s32 c);
f32 func_80056690(f32);
/* sqrt wrapper is a statement-expression macro returning double */
#define SQRT(x) ({ (f64)func_80056690(x); })

/* camera update: builds view matrices, or moves the camera to follow the player */
void func_80032B28(Cam *arg0) {
    register Cam *c = arg0;
    register f32 dx;
    register f32 dz;
    register f32 s;
    f32 persp[4][4];
    f32 view[4][4];
    f32 mtx[4][4];

    if (D_800AD03A != 0) {
        func_80057410(persp, &D_800A7F00, 40.0f, 1.3333334f, 50.0f, 10000.0f, 1.0f);
        func_800567F0(view, c->eye[0], c->eye[1], c->eye[2], c->at[0], c->at[1], c->at[2], c->up[0], c->up[1], c->up[2]);
        func_80057180(view, persp, mtx);
        func_80056F00(mtx, D_8013EF28);
        func_80001424(func_80032894, c, 1, 0, 0);
        return;
    }
    if (c->timer != 0) {
        c->timer--;
        c->at[0] += c->vel[0];
        c->at[1] += c->vel[1];
        c->at[2] += c->vel[2];
    }
    dx = D_80106C84->x - c->at[0];
    dz = D_80106C84->z - c->at[2];
    s = ((f32)(c->timer * 2) + 844.0) / SQRT(dx * dx + dz * dz);
    dx *= s;
    dz *= s;
    s = 0;
    s = 0x80 - c->timer;
    s = s * 2.25;
    c->eye[0] = D_80106C84->x + dx;
    c->eye[1] = s + 225.0;
    c->eye[2] = D_80106C84->z + dz;
}
