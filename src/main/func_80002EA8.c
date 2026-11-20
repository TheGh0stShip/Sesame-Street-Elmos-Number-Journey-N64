typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct Vtx {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx;

typedef struct Mtx {
    s32 m[4][4];
} Mtx;

typedef struct Camera {
    u8 pad0[0x24];
    f32 eyeX;  /* 0x24 */
    f32 eyeY;  /* 0x28 */
    f32 eyeZ;  /* 0x2C */
    f32 atX;   /* 0x30 */
    f32 atY;   /* 0x34 */
    f32 atZ;   /* 0x38 */
} Camera;

extern u32 D_80106C90;
extern u32 D_800A7F28;
extern u32 D_8006EE80;
extern u32 D_8006EE84;
extern u8 D_8006EE88[];
extern Camera *D_80076800;
extern f32 D_8006F758;
extern f32 D_8006F75C;
extern s32 D_800717E0;
extern Mtx D_8013C730[];
extern Gfx *D_8013EC7C;
extern Vtx D_8006F6D8[4];
extern Vtx D_8006F718[4];
extern f64 func_80069920(f64, f64);
extern void func_800581E8(Mtx *, f32, f32, f32, f32, f32, f32, f32);
extern f32 func_80057E70(f32);
extern f32 func_800566A0(f32);

/* Draws the scrolling sky/backdrop quad, rotated to follow the camera's heading. */
void func_80002EA8(void) {
    Gfx *g;
    s32 unused[4];
    register f32 c;
    register f32 s;
    register f32 x;
    register f32 y;
    register f32 z;
    register f32 dx;
    register f32 dz;
    register f32 ang;

    D_80106C90 += D_8006EE80;
    D_800A7F28 += D_8006EE84;
    D_80106C90 &= 0x7FFF;
    D_800A7F28 &= 0x7FFF;
    x = D_80076800->eyeX;
    y = D_80076800->eyeY;
    z = D_80076800->eyeZ;
    dx = D_80076800->atX - x;
    dz = D_80076800->atZ - z;
    ang = func_80069920(dx, dz);
    D_8006F758 = ang * 57.2957;
    if (D_8006F758 < 0.0) {
        D_8006F758 += 360.0;
    }
    D_8006F758 += 180.0f;
    func_800581E8(&D_8013C730[D_800717E0], D_8006F75C, D_8006F758, 0.0f, 1.0f, x, y, z);
    g = D_8013EC7C++;
    g->w0 = 0x01020040;
    g->w1 = (u32)(D_800717E0 * sizeof(Mtx) + (u8 *)D_8013C730 - 0x80000000);
    c = func_80057E70(-ang);
    s = func_800566A0(-ang);
    if (D_800717E0 == 0) {
        D_8006F6D8[0].tc[0] = -c * -4096.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[0].tc[1] = s * -4096.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[1].tc[0] = -c * -4096.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[1].tc[1] = s * -4096.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[2].tc[0] = -c * 0.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[2].tc[1] = s * 0.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F6D8[3].tc[0] = -c * 0.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F6D8[3].tc[1] = s * 0.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
    } else {
        D_8006F718[0].tc[0] = -c * -4096.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[0].tc[1] = s * -4096.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[1].tc[0] = -c * -4096.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[1].tc[1] = s * -4096.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[2].tc[0] = -c * 0.0f + s * 2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[2].tc[1] = s * 0.0f + c * 2048.0f + (f32)(D_800A7F28 >> 4);
        D_8006F718[3].tc[0] = -c * 0.0f + s * -2048.0f + (f32)(D_80106C90 >> 4);
        D_8006F718[3].tc[1] = s * 0.0f + c * -2048.0f + (f32)(D_800A7F28 >> 4);
    }
    g = D_8013EC7C++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xBA001402;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xB6000000;
    g->w1 = 0x009F3207;
    g = D_8013EC7C++;
    g->w0 = 0xB900031D;
    g->w1 = 0x0F0A4000;
    g = D_8013EC7C++;
    g->w0 = 0xFCFFFFFF;
    g->w1 = 0xFFFCF87C;
    g = D_8013EC7C++;
    g->w0 = 0xBA000C02;
    g->w1 = 0x2000;
    g = D_8013EC7C++;
    g->w0 = 0xFD10001F;
    g->w1 = (u32)D_8006EE88;
    g = D_8013EC7C++;
    g->w0 = 0xBB000001;
    g->w1 = 0xFFFFFFFF;
    g = D_8013EC7C++;
    g->w0 = 0xF5101000;
    g->w1 = 0x07000000;
    g = D_8013EC7C++;
    g->w0 = 0xE6000000;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xF3000000;
    g->w1 = 0x07400000;
    g = D_8013EC7C++;
    g->w0 = 0xF5101000;
    g->w1 = 0x00014050;
    g = D_8013EC7C++;
    g->w0 = 0xF2000000;
    g->w1 = 0x0007C07C;
    if (D_800717E0 == 0) {
        g = D_8013EC7C++;
        g->w0 = 0x0400103F;
        g->w1 = (u32)D_8006F6D8;
    } else {
        g = D_8013EC7C++;
        g->w0 = 0x0400103F;
        g->w1 = (u32)D_8006F718;
    }
    g = D_8013EC7C++;
    g->w0 = 0xB1000402;
    g->w1 = 0x604;
    g = D_8013EC7C++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
}
