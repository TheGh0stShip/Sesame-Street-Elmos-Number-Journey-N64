typedef struct { u8 unk_00[0x1C]; } Chan;
typedef struct { u8 unk_00[8]; s32 unk_08; u8 unk_0C[0xC9 - 0xC]; u8 unk_C9; } Obj;
extern Chan *D_800A0548;
void func_80055F90(Chan *);
void func_80055F00(Chan *, s32);
/* (re)starts channel idx with o->unk_08, stopping it first if active */
void func_8004F914(Obj *o, s32 idx) {
    if (o->unk_C9 != 0) {
        func_80055F90(&D_800A0548[idx]);
    }
    o->unk_C9 = 1;
    func_80055F00(&D_800A0548[idx], o->unk_08);
    o->unk_08 = 0;
}
