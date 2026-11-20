typedef struct { u8 unk_00[0x28]; void (*unk_28)(void); } Node;
typedef struct { u8 unk_00[0x20]; s32 unk_20; } Arg1;
void *func_8005C810(s32, s32, s32, s32, s32);
s32 func_80051F28(void);
s32 func_80051F6C(void);
void func_80051C30(s32);
void func_8005BE38(void);
extern void *D_8013C708[6];
/* creates the root node and six child nodes */
void func_80051AE8(Node **out, Arg1 *a, s32 parent) {
    Node *n = func_8005C810(0, 0, parent, 1, 0x2C);
    *out = n;
    n->unk_28 = func_8005BE38;
    D_8013C708[0] = func_8005C810(0, 0, parent, func_80051F28(), 0x28);
    D_8013C708[1] = func_8005C810(0, 0, parent, func_80051F6C(), 2);
    D_8013C708[2] = func_8005C810(0, 0, parent, 1, 0x34);
    D_8013C708[3] = func_8005C810(0, 0, parent, 1, 0x20);
    D_8013C708[4] = func_8005C810(0, 0, parent, 1, 0x30);
    D_8013C708[5] = func_8005C810(0, 0, parent, 1, 8);
    func_80051C30(a->unk_20);
}
