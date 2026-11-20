typedef struct { u8 pad[0x1C]; } Chan;
typedef struct { u8 pad[0xC9]; u8 unk_C9; u8 padCA[0x13C - 0xCA]; } Voice;
typedef struct { s16 unk_00; s16 unk_02; s8 unk_04; } ChanCfg;
typedef struct Evt { struct Evt *next; } Evt;
typedef struct {
    s32 unk_00; s32 unk_04; s32 unk_08; s32 unk_0C; s32 unk_10; s32 unk_14;
    s32 unk_18; s32 unk_1C; s32 unk_20; s32 unk_24; u8 pad28[0x18]; s32 unk_40;
} Config;
extern s32 D_80000300;
extern s32 D_800BD854;
extern s32 D_8012C408;
extern s32 D_800A0544;
extern Chan *D_800A0548;
extern Voice *D_800A054C;
extern Voice *D_800A0550;
extern s32 D_800A0554;
extern s32 D_800A0558;
extern s32 D_800A0560;
extern s32 D_800A0564;
extern s32 D_800A0568;
extern s32 D_800A056C;
extern s32 D_800A0574;
extern s32 D_800A0578;
extern s32 D_800A057C;
extern s32 D_800A0580;
extern Evt D_800A0530;
extern Evt *D_800A0534;
extern void (*D_800A0538)(void);
void func_8004F0DC(void);
void func_800528F0(s32, s32);
void func_80052460(s32);
void *func_8005293C(s32);
void func_8004EF60(s32);
void func_8004EBE4(s32, s32);
void func_8004EDAC(s32);
void func_80052610(Config *, s32, s32);
void func_8004E2F4(s32, s32);
void func_80055A50(Evt *);
void func_8005057C(Voice *);
void func_80055AA0(Chan *, ChanCfg *);
void func_8005296C(void);

/* audio system init from a config block */
void func_8004E06C(Config *c) {
    ChanCfg cfg;
    s32 i;

    D_800BD854 = c->unk_40;
    D_8012C408 = c->unk_00;
    D_800A0544 = c->unk_04 + 4;
    if (D_80000300 != 0) {
        D_800A0554 = 60;
    } else {
        D_800A0554 = 50;
    }
    D_800A0558 = 1000000 / D_800A0554;
    func_800528F0(c->unk_10, c->unk_14);
    func_80052460(c->unk_08);
    D_800A0548 = func_8005293C((D_800A0544 - 4) * sizeof(Chan));
    D_800A054C = func_8005293C(D_800A0544 * sizeof(Voice));
    D_800A0550 = D_800A054C + 4;
    func_8004EF60(c->unk_24);
    D_800A0568 = 0;
    D_800A056C = 0;
    if (c->unk_18 != 0 && c->unk_1C != 0) {
        func_8004EBE4(c->unk_18, c->unk_1C);
    }
    D_800A057C = 0;
    D_800A0578 = 0;
    if (c->unk_20 != 0) {
        func_8004EDAC(c->unk_20);
    }
    D_800A0580 = 0;
    D_800A0574 = 2;
    func_80052610(c, D_800A0554, 2);
    func_8004E2F4(3, 0x7FFF);
    D_800A0560 = 1;
    D_800A0564 = 0x12345678;
    D_800A0530.next = NULL;
    D_800A0538 = func_8004F0DC;
    D_800A0534 = &D_800A0530;
    func_80055A50(&D_800A0530);
    for (i = 0; i < D_800A0544; i++) {
        D_800A054C[i].unk_C9 = 0;
        func_8005057C(&D_800A054C[i]);
        cfg.unk_04 = 0;
        cfg.unk_00 = c->unk_0C;
        cfg.unk_02 = 0;
        if (i >= 4) {
            func_80055AA0(&D_800A0548[i - 4], &cfg);
        }
    }
    func_8005296C();
}
