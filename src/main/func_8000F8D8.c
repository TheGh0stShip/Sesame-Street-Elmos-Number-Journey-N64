typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx; /* size 0x10 */

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    u8 unk_0[0xC];
    s16 unk_C;
    u8 unk_E[0x2];
    union {
        s32 w;
        struct {
            s16 hi;
            u16 lo;
        } h;
    } unk_10;
    s16 unk_14;
    u8 unk_16[0x2];
    u8 unk_18;
    u8 unk_19;
    u8 unk_1A[0x2];
    u8 unk_1C;
    u8 unk_1D[0x7];
    u16 unk_24;
} Actor;

extern f32 D_800741E0[];
extern s16 D_800BD87C;
extern s16 D_80106C6E;
extern s32 D_80106C7C;
extern Vtx *D_80128578;
extern Gfx *D_8013EC7C;
void func_80012F14(Actor *a);
f32 func_80056690(f32 x);

#define GFX_W(w0_, w1_)              \
    {                                \
        g = D_8013EC7C++;            \
        g->w0 = (w0_);               \
        g->w1 = (u32) (w1_);         \
    }

/* bobbing actor: moves it, then draws a camera-facing triangle of height unk_24 */
void func_8000F8D8(register Actor *a) {
    register s16 x;
    register u16 y;
    register s16 z;
    register s32 dx;
    register s32 dz;
    register s32 d2;
    register s32 t;
    register f32 f;
    Gfx *g;

    if (!(D_80106C7C & 1)) {
        a->unk_19++;
    }
    if (--a->unk_1C == 0) {
        func_80012F14(a);
        return;
    }
    a->unk_10.w += 0xC0000;
    x = a->unk_C;
    z = a->unk_14;
    y = a->unk_10.h.hi;
    f = D_800741E0[(u8) (a->unk_18++ & 0x1F)] * 30.0;
    x += (s16) (f * D_800741E0[(u8) (a->unk_19 & 0x1F)]);
    z += (s16) (f * D_800741E0[(a->unk_19 + 8) & 0x1F]);
    dx = D_800BD87C;
    dx <<= 2;
    dx -= x;
    dz = D_80106C6E;
    dz <<= 2;
    dz -= z;
    d2 = dx * dx;
    t = dz * dz;
    d2 += t;
    if (d2 < 0) {
        return;
    }
    f = func_80056690(d2);
    d2 = a->unk_24;
    if (f != 0.0f) {
        f = d2 / f;
        dx = dx * f;
        dz = dz * f;
        GFX_W(0x04000C2F, D_80128578);
        D_80128578->ob[0] = x;
        D_80128578->ob[1] = y - d2;
        D_80128578->ob[2] = z;
        D_80128578->tc[0] = 0x400;
        D_80128578->tc[1] = 0xC00;
        d2 >>= 1;
        D_80128578++;
        D_80128578->ob[0] = x + dz;
        D_80128578->ob[1] = y + d2;
        D_80128578->ob[2] = z - dx;
        D_80128578->tc[0] = -0x400;
        D_80128578->tc[1] = 0;
        D_80128578++;
        D_80128578->ob[0] = x - dz;
        D_80128578->ob[1] = y + d2;
        D_80128578->ob[2] = z + dx;
        D_80128578->tc[0] = 0xC00;
        D_80128578->tc[1] = 0;
        D_80128578++;
        GFX_W(0xE7000000, 0);
        GFX_W(0xFA000000, -1);
        GFX_W(0xFB000000, -0x100);
        GFX_W(0xBB000101, 0x80008000);
        GFX_W(0xBF000000, 0x204);
    }
}
