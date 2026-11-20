typedef struct {
    u8 pad0[8];
    void (*unk_8)(void);
    s16 unk_C; s16 padE;
    s16 unk_10; s16 pad12;
    s16 unk_14; s16 pad16;
    u8 pad18;
    u8 unk_19; u8 unk_1A; u8 unk_1B; u8 unk_1C; u8 unk_1D;
    u8 pad1E[6];
    s16 unk_24;
} Obj;
typedef struct {
    u8 pad0[0x24];
    f32 unk_24; f32 unk_28; f32 unk_2C; f32 unk_30; f32 unk_34; f32 unk_38;
} Src;
Obj *func_80012E78(void);
u32 func_80021640(void);
void func_8000F074(void);

/* spawns six objects with randomised position, size and colour fields */
void func_8000EE10(Src *arg0) {
    register u32 r;
    register u32 t;
    register s32 n;
    register Src *p;
    register Obj *o;

    p = arg0;
    n = 6;
    for (;;) {

        if ((o = func_80012E78()) != NULL) {
            o->unk_8 = func_8000F074;
            r = func_80021640();
            t = ((r & 0x3F) + 16) * p->unk_30;
            r >>= 6;
            o->unk_19 = t;
            o->unk_C = (s16)((s16)p->unk_24 + (s16)t) << 2;
            t = r & 0x3F;
            t += t;
            t += t;
            r >>= 6;
            o->unk_10 = (s16)((s16)p->unk_28 + (s16)t) << 2;
            t = ((r & 0x3F) + 16) * p->unk_38;
            r >>= 6;
            o->unk_1B = t;
            o->unk_14 = (s16)((s16)p->unk_2C + (s16)t) << 2;
            t = r & 0xF;
            r >>= 4;
            o->unk_24 = t + 60;
            t = r & 7;
            r >>= 3;
            o->unk_1C = t;
            t = (r & 7) + 4;
            r >>= 3;
            o->unk_1D = t;
            t = ((r & 0xF) << 3) + 127;
            r >>= 4;
            o->unk_1A = t;
        }
        if (--n == 0) {
            break;
        }
    }
}
