typedef struct {
    s32 unk_0;
    s32 unk_4;
    s16 unk_8;
} Msg;

typedef struct {
    u8 unk_0[0x88];
    s32 unk_88;
} Target;

typedef struct {
    u8 unk_0[0x8];
    Target *unk_8;
} Owner;

typedef struct {
    u8 unk_0[0x1C];
    s32 unk_1C;
} Ctx;

extern Ctx *D_80077BC4;
Msg *func_800563E8(void);
void func_80054288(Target *t, s32 kind, Msg *m);

/* allocates a message (kind 0xF) for the owner's target and posts it with type 3 */
void func_80055F90(Owner *o) {
    Msg *m;
    s32 t;

    if (o->unk_8 != NULL) {
        m = func_800563E8();
        if (m != NULL) {
            t = D_80077BC4->unk_1C + o->unk_8->unk_88;
            m->unk_8 = 0xF;
            m->unk_0 = 0;
            m->unk_4 = t;
            func_80054288(o->unk_8, 3, m);
        }
    }
}
