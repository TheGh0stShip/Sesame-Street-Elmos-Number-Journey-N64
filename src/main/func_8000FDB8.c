typedef struct {
    u8 unk_00[0x24];
    f32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    u8 unk_3C[0x1B7];
    s8 unk_1F3;
} Actor;
typedef struct {
    u8 unk_00[0x8];
    void (*unk_08)(void);
    s16 unk_0C;
    s16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s8 unk_18;
    s8 unk_19;
    u8 unk_1A[2];
    s8 unk_1C;
    u8 unk_1D[7];
    s16 unk_24;
} Particle;
extern u32 D_80106C7C;
extern Actor *D_80106C84;
u32 func_80021640(void);
Particle *func_80012E78(void);
void func_8000F8D8(void);

/* periodically spawn a randomised particle at an offset rotated by the actor's facing */
void func_8000FDB8(s32 x, s32 y, s32 z) {
    register Actor *a;
    register Particle *p;
    register u32 r;
    register u32 t;
    register f32 s;
    register f32 c;
    register f32 fx;
    register f32 fz;
    register f32 nx;
    s32 pad;

    a = D_80106C84;
    if (a->unk_1F3 > 0) {
        if (!(D_80106C7C & 1)) {
            a->unk_1F3--;
        }
    } else if (a->unk_1F3 == 0) {
        a->unk_1F3 = (func_80021640() & 7) - 15;
    } else if (D_80106C7C % 3 == 0) {
        if (++a->unk_1F3 == 0) {
            a->unk_1F3 = (func_80021640() & 0x1F) + 50;
        }
        if ((p = func_80012E78()) != NULL) {
            p->unk_08 = func_8000F8D8;
            s = a->unk_30;
            c = a->unk_38;
            fx = x;
            fz = z;
            nx = c * fx + s * fz;
            fz = fz * c - s * fx;
            x = nx;
            z = fz;
            { register s16 i = x + a->unk_24; p->unk_0C = i << 2; }
            { register s16 i = y + a->unk_28; p->unk_10 = i << 2; }
            { register s16 i = z + a->unk_2C; p->unk_14 = i << 2; }
            r = func_80021640();
            t = r & 0x1F;
            t += 18;
            r >>= 5;
            p->unk_24 = t;
            t = r & 0x1F;
            r >>= 5;
            p->unk_18 = t;
            t = r & 0x1F;
            r >>= 5;
            p->unk_19 = t;
            p->unk_1C = 75;
        }
    }
}
