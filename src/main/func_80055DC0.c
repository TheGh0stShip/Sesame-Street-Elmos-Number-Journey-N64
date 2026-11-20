typedef struct { s32 unk_00; s32 unk_04; s16 unk_08; s16 pad0A; s32 unk_0C; s32 unk_10; } Ev;
typedef struct { s32 unk_00; s32 unk_04; f32 unk_08_dummy; } Dummy;
typedef struct { u8 pad[0x88]; s32 unk_88; } Tgt;
typedef struct { s32 unk_00; s32 unk_04; Tgt *unk_08; } Owner;
typedef struct { u8 pad[0x1C]; s32 unk_1C; } Ctx;
extern Ctx *D_80077BC4;
Ev *func_800563E8(void);
void func_80054288(Tgt *, s32, Ev *);
s32 func_80054BA4(s32, s32);
/* Queues event type 7 carrying a float for the owner's target. */
void func_80055DC0(Owner *o, f32 val) {
    Ev *e;
    if (o->unk_08 != NULL) {
        e = func_800563E8();
        if (e != NULL) {
            s32 t = D_80077BC4->unk_1C + o->unk_08->unk_88;
            e->unk_08 = 7;
            *(f32 *)&e->unk_0C = val;
            e->unk_00 = 0;
            e->unk_04 = t;
            func_80054288(o->unk_08, 3, e);
        }
    }
}
