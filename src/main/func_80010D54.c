typedef struct Obj {
    u8 unk_00[0x8];
    void *unk_08;
    s16 unk_0C;
    u8 unk_0E[2];
    s16 unk_10;
    u8 unk_12[2];
    s16 unk_14;
    u8 unk_16[2];
    s8 unk_18;
    s8 unk_19;
    s8 unk_1A;
    s8 unk_1B;
    s16 unk_1C;
    u8 unk_1E[6];
    s16 unk_24;
} Obj;

extern s32 D_800108D4;
Obj *func_80012E78(void);
u32 func_80021640(void);

/* allocates an object at (x, y, z) * 4 with randomised fields */
void func_80010D54(s32 arg0, s32 arg1, s32 arg2) {
    register Obj *obj;
    register u32 r;
    register s32 t;
    s8 pad0;

    if ((obj = func_80012E78()) != NULL) {
        obj->unk_08 = &D_800108D4;
        obj->unk_0C = ((u16)arg0) << 2;
        obj->unk_10 = ((u16)arg1) << 2;
        obj->unk_14 = ((u16)arg2) << 2;
        r = func_80021640();
        t = r & 15;
        t = t + 60;
        r = r >> 4;
        obj->unk_24 = t;
        obj->unk_1C = ((u16)arg1) << 2;
        obj->unk_18 = 0;
        t = r & 127;
        t = t - 64;
        r = r >> 7;
        t = t & 248;
        obj->unk_19 = t;
        t = r & 63;
        t = t + 64;
        r = r >> 7;
        obj->unk_1A = t;
        t = r & 127;
        t = t - 64;
        r = r >> 7;
        t = t & 248;
        obj->unk_1B = t;
    }
}
