typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s32 m[4][4]; } Mtx;
typedef struct { f32 m[4][4]; } MtxF;
typedef struct { u8 pad[0x18]; s32 unk_18; s32 unk_1C; s32 unk_20; } Node;
extern Gfx *D_8013EC7C;
void func_8001FDA0(MtxF *m, Node *n, s32 c);
void func_80057180(MtxF *m, Mtx *a, Mtx *b);
void func_80056F00(Mtx *a, Mtx *b);
Mtx *func_80020758(s32 a, s32 b, MtxF *m, s32 c, Mtx *d);

/* Builds a node matrix, emits matrix + display list commands, recurses into children. */
Mtx *func_80020638(Mtx *arg0, Node *arg1, s32 arg2, Mtx *arg3) {
    s32 pad[2];
    MtxF mf;
    Gfx *g;

    func_8001FDA0(&mf, arg1, arg2);
    func_80057180(&mf, arg0, arg0);
    func_80056F00(arg0, arg3);
    g = D_8013EC7C++;
    g->w0 = 0x01020040;
    g->w1 = (u32)arg3;
    g = D_8013EC7C++;
    g->w0 = 0x06000000;
    g->w1 = arg1->unk_20;
    arg3++;
    if (arg1->unk_18 != 0) {
        arg3 = func_80020758(arg1->unk_1C, arg1->unk_18, &mf, arg2, arg3);
    }
    return arg3;
}
