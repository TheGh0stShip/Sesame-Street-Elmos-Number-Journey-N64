typedef struct { s32 unk_00; s32 unk_04; } Pair;
typedef struct { u8 unk_00[0x10]; s32 unk_10; u8 unk_14[0x4]; Pair unk_18[1]; } Obj;
typedef struct { u8 unk_00[0x78]; Obj *unk_78; u8 unk_7C[0x2A]; u16 unk_A6; u8 unk_A8[0x94]; } Ent;
extern s32 D_800A0544;
extern Ent *D_800A0550;
extern s32 D_800A0568;
extern Obj *D_800A0578;
extern Obj *D_800A057C;
s32 func_800508B8(Ent *, Obj *, s32, s32, s32, s32);
s32 func_80050968(Obj *, s32, s32, s32, s32);

/* play/queue on the pending object, reusing a matching active entry when search is set */
s32 func_8004E620(s32 idx, s32 arg1, s32 arg2, s32 search, s32 arg4) {
    Obj *obj;
    Ent *e;
    s32 i;
    s32 ret;

    if (D_800A057C == NULL) {
        obj = D_800A0578;
        if (obj == NULL) {
            D_800A0568 = 0;
            return 0;
        }
    } else {
        obj = D_800A057C;
        D_800A057C = NULL;
    }
    if (D_800A0568 == 0) {
        D_800A0568 = obj->unk_10;
    }
    if (search != 0) {
        e = D_800A0550;
        for (i = 4; i < D_800A0544; i++, e++) {
            if (e->unk_A6 == idx && e->unk_78 == obj) {
                if (arg4 == -1) arg4 = obj->unk_18[idx].unk_04;
                ret = func_800508B8(e, obj, idx, arg1, arg2, arg4);
                D_800A0568 = 0;
                return ret;
            }
        }
    }
    ret = func_80050968(obj, idx, arg1, arg2, arg4);
    D_800A0568 = 0;
    return ret;
}
