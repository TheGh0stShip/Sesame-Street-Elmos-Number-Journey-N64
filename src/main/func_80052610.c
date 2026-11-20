typedef struct {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 unk_1C;
    u8 pad1D[0xB];
} Cfg;
typedef struct {
    s32 unk_0;
    s32 unk_4;
    u8 pad8[0x4];
    s32 unk_C;
    u8 pad10[0x18];
    s32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3C;
} Params;
typedef struct { void *unk_0; s32 unk_4; } Pair;
extern void func_800517C0(void *, Cfg *);
extern s32 func_80052010(s32, s32);
extern void *func_8005293C(s32);
extern s32 func_80052984(void);
extern s32 func_80053620(s32, s32, s32, s32);
extern void func_800585E0(void *, s32, void *, void *, void *, s32);
extern void func_80058EA0(void *);
extern s32 func_8005F250(s32);
extern void D_80052748(void *);
extern u8 D_800A05F0[];
extern u8 *D_800A07A0;
extern Pair *D_800A07A4;
extern void *D_800A07A8;
extern u8 D_8012FDF0[];
/* audio setup: build config, allocate buffers, start audio thread */
void func_80052610(Params *p, s32 arg1, u8 arg2) {
    Cfg c;
    s32 n;
    u32 i;
    c.unk_0 = c.unk_4 = p->unk_4;
    c.unk_8 = p->unk_28;
    c.unk_10 = func_80052010(p->unk_38, p->unk_3C);
    c.unk_1C = arg2;
    c.unk_18 = func_8005F250(p->unk_2C);
    c.unk_14 = func_80052984();
    func_800517C0(D_8012FDF0, &c);
    n = func_80053620(p->unk_34, c.unk_18, arg1, 0xF);
    D_800A07A8 = func_8005293C(p->unk_30 * 8);
    D_800A07A4 = func_8005293C(0x18);
    for (i = 0; i < 3; i++) {
        D_800A07A4[i].unk_0 = func_8005293C(n * 4);
    }
    D_800A07A0 = func_8005293C(0x2000);
    func_800585E0(D_800A05F0, 3, D_80052748, NULL, D_800A07A0 + 0x2000, p->unk_C);
    func_80058EA0(D_800A05F0);
}
