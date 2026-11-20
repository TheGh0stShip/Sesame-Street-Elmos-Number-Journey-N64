typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } Vtx;
typedef union { s32 w; struct { u16 hi; u16 lo; } h; } Fix;
typedef struct {
    u8 pad0[0xC]; Fix x; Fix y; Fix z;
    s8 unk_18; s8 vx; s8 vy; s8 vz; s16 unk_1C; u8 pad1E[6]; u16 unk_24;
} Obj;
extern s16 D_800BD87C;
extern s16 D_80106C6E;
extern Vtx *D_80128578;
extern Gfx *D_8013EC7C;
void func_80012F14(Obj *);
s32 func_80021640(void);
f32 func_80056690(f32);

/* move a falling particle and draw it as a camera-facing triangle */
void func_80010E40(register Obj *o) {
    register u16 x;
    register u16 y;
    register u16 z;
    register s32 dx;
    register s32 dz;
    register s32 t;
    register s32 r;
    register f32 f;
    Gfx *g;

    o->x.w = o->x.w + (o->vx << 15);
    o->y.w = o->y.w + (o->vy << 16);
    o->z.w = o->z.w + (o->vz << 15);
    if (o->vy >= -0x76) {
        o->vy = o->vy - 8;
    }
    x = o->x.h.hi;
    y = o->y.h.hi;
    z = o->z.h.hi;
    if (o->unk_1C > (s16)y) {
        func_80012F14(o);
        return;
    }
    dx = (D_800BD87C << 2) - (s16)x;
    dz = (D_80106C6E << 2) - (s16)z;
    t = dx * dx;
    r = dz * dz;
    t = t + r;
    if (t < 0) {
        return;
    }
    f = func_80056690((f32)t);
    t = o->unk_24;
    if (f != 0.0f) {
    f = (f32)t / f;
    dx = dx * f;
    dz = dz * f;
    g = D_8013EC7C++;
    g->w0 = 0x04000C2F;
    g->w1 = (u32)D_80128578;
    D_80128578->ob[0] = x;
    D_80128578->ob[1] = y - t;
    D_80128578->ob[2] = z;
    D_80128578->tc[0] = 0x400;
    D_80128578->tc[1] = 0xC00;
    t = t >> 1;
    D_80128578++;
    D_80128578->ob[0] = x + dz;
    D_80128578->ob[1] = y + t;
    D_80128578->ob[2] = z - dx;
    D_80128578->tc[0] = -0x400;
    D_80128578->tc[1] = 0;
    D_80128578++;
    D_80128578->ob[0] = x - dz;
    D_80128578->ob[1] = y + t;
    D_80128578->ob[2] = z + dx;
    D_80128578->tc[0] = 0xC00;
    D_80128578->tc[1] = 0;
    D_80128578++;
    g = D_8013EC7C++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    r = func_80021640();
    dx = r & 7;
    if (dx != 0) {
        if (o->unk_18 >= 9) {
            g = D_8013EC7C++;
            g->w0 = 0xFA000000;
            g->w1 = 0xFFFF00FF;
            g = D_8013EC7C++;
            g->w0 = 0xFB000000;
            g->w1 = 0xFFFF0000;
        } else if (o->unk_18 >= 7) {
            g = D_8013EC7C++;
            g->w0 = 0xFA000000;
            g->w1 = 0xFFFF00FF;
            g = D_8013EC7C++;
            g->w0 = 0xFB000000;
            g->w1 = 0xFF800000;
        } else if (o->unk_18 >= 4) {
            g = D_8013EC7C++;
            g->w0 = 0xFA000000;
            g->w1 = 0xFF8000FF;
            g = D_8013EC7C++;
            g->w0 = 0xFB000000;
            g->w1 = 0xFF000000;
        } else {
            g = D_8013EC7C++;
            g->w0 = 0xFA000000;
            g->w1 = 0xFF0000FF;
            g = D_8013EC7C++;
            g->w0 = 0xFB000000;
            g->w1 = 0xFF000000;
        }
    } else {
        g = D_8013EC7C++;
        g->w0 = 0xFA000000;
        g->w1 = 0xFFFFFFFF;
        g = D_8013EC7C++;
        g->w0 = 0xFB000000;
        g->w1 = 0xFFFFFF00;
    }
    g = D_8013EC7C++;
    g->w0 = 0xBB000001;
    g->w1 = 0x80008000;
    g = D_8013EC7C++;
    g->w0 = 0xBF000000;
    g->w1 = 0x204;
    }
}
