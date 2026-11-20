typedef struct { u8 pad[0x60]; u64 unk_60; u8 pad68[0x80D0 - 0x68]; } Frame80D0;
void func_8000C60C(void);
void func_800585E0(void *, s32, void (*)(void *), void *, void *, s32);
void func_80058EA0(void *);
void func_80058A70(void *, void *, s32);
void func_8005F690(void *);
void func_80052D88(u16);
void func_80021640(void);
u64 func_800586B0(void);
void func_80041F04(s32);
void func_8000BA3C(Frame80D0 *);
void func_80001000(void);
void func_80058FC0(void *);
void func_8005F590(s32);
void func_8004E738(s32, s32);
void func_80013410(void *);
void func_80027D90(void *);
extern u8 D_800AD038;
extern u8 D_800AD03A;
extern s32 D_8013ED48;
extern s32 D_800AF040;
extern s32 D_800A91F0;
extern s32 D_8013E7D0;
extern s32 D_8010E650;
extern s32 D_8006EE70;
extern s32 D_80106C7C;
extern u16 D_800A7F18;
extern u16 D_8013C7C0;
extern u16 D_80109DF6;
extern u16 D_800A7F68;
extern s32 D_8011551C;
extern s32 D_80115408;
extern u64 D_800A7A68;
extern s32 D_800717E0;
extern Frame80D0 D_801182A0[];
extern u64 D_80140F50;
extern u64 D_80107CE8;
extern s32 D_8012F118;

/* Main game thread: starts the helper threads, then handles messages forever (frame, timing, reset). */
void func_8000C188(void *arg) {
    volatile u32 pending;
    s32 started;
    u16 *msg;
    volatile s32 idle;
    u64 t;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;

    pending = 0;
    started = 0;
    msg = NULL;
    idle = 0;
    D_800AD038 = 0;
    unk_30 = 0;
    unk_34 = 0;
    func_8000C60C();
    func_800585E0(&D_8013ED48, 0, func_80013410, NULL, &D_800AF040, 12);
    func_80058EA0(&D_8013ED48);
    func_800585E0(&D_800A91F0, 0, func_80027D90, NULL, &D_8013E7D0, 1);
    func_80058EA0(&D_800A91F0);
    while (1) {
        func_80058A70(&D_8010E650, &msg, 1);
        switch ((s16)*msg) {
        case 1:
            if (D_800AD038 == 0) {
                if (D_8006EE70 != 0 && started == 0) {
                    started = 1;
                    func_8005F690(&D_8010E650);
                }
                D_80106C7C++;
                D_800A7F18++;
                if (D_8013C7C0 != 0) {
                    D_80109DF6 -= D_8013C7C0;
                    if (D_80109DF6 < 100) {
                        D_8013C7C0 = 0;
                        func_80052D88(D_800A7F68);
                        D_8011551C = 0;
                    }
                }
                func_80021640();
                D_800AD03A = 0;
                t = func_800586B0();
                func_80041F04(D_80115408);
                D_800A7A68 = func_800586B0() - t;
            }
            if (pending < 2) {
                t = func_800586B0();
                func_8000BA3C(&D_801182A0[D_800717E0]);
                D_80140F50 = func_800586B0() - t;
                pending++;
                D_800717E0 ^= 1;
                idle = 0;
            } else {
                idle++;
            }
            break;
        case 6:
            D_80107CE8 = D_801182A0[0].unk_60;
            pending--;
            break;
        case 7:
            D_80107CE8 = D_801182A0[1].unk_60;
            pending--;
            break;
        case 5:
            func_80001000();
            started = 0;
            break;
        case 4:
            pending += 2;
            D_800AD038 = 1;
            func_80058FC0(&D_8012F118);
            func_8005F590(1);
            func_80052D88(D_800A7F68);
            func_8004E738(3, 0);
            break;
        }
    }
}
