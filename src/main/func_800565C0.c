typedef struct { u8 pad[0x24]; s32 arr[1]; } Sub;
typedef struct { u8 pad[0x34]; Sub *unk_34; } Obj;
extern Obj *D_80077BC4;
void func_800537F0(s32 *);
s32 func_800565C0(s16 idx) {
    func_800537F0(&D_80077BC4->unk_34->arr[idx]);
    return D_80077BC4->unk_34->arr[idx];
}
