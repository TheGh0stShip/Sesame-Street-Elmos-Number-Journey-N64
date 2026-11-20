typedef struct { s32 unk_0, unk_4, unk_8, unk_C; } Arg;
typedef struct {
    s32 unk_00, unk_04, unk_08, unk_0C, unk_10, unk_14;
    u8 *unk_18; s32 unk_1C, unk_20, unk_24, unk_28, unk_2C;
    s32 unk_30, unk_34, unk_38, unk_3C, unk_40, unk_44, unk_48, unk_4C;
    s32 unk_50; void *unk_54; s32 unk_58, unk_5C, unk_60, unk_64;
} Task;
extern s32 D_800A05E0;
extern u8 *D_800A05E4;
extern u8 D_8006CD00[];
extern u8 D_8006CDD0[];
s32 func_800614C0(s32, Arg *);
void func_80058BA0(s32, Task *, s32);
void func_80058A70(void *, s32, s32);
/* builds a task descriptor from arg and submits it */
void func_80052530(Arg *a) {
    Task t;
    s32 w;
    s32 buf[8];
    t.unk_00 = 0;
    t.unk_54 = buf;
    t.unk_08 = 2;
    t.unk_50 = (s32)(D_800A05E4 + 0x30);
    t.unk_40 = a->unk_0;
    t.unk_44 = a->unk_4;
    t.unk_10 = 2;
    t.unk_18 = D_8006CD00;
    t.unk_1C = D_8006CDD0 - D_8006CD00;
    t.unk_14 = 0;
    t.unk_20 = a->unk_8;
    w = a->unk_C;
    t.unk_24 = 0x1000;
    t.unk_2C = 0x800;
    t.unk_30 = 0;
    t.unk_34 = 0;
    t.unk_38 = 0;
    t.unk_3C = 0;
    t.unk_48 = 0;
    t.unk_4C = 0;
    t.unk_28 = w;
    func_80058BA0(func_800614C0(D_800A05E0, a), &t, 1);
    func_80058A70(D_800A05E4 + 0x30, 0, 1);
}
