#include "common.h"

void func_8000BA3C(Frame80D0 *);

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
    osCreateThread(&D_8013ED48, 0, func_80013410, NULL, &D_800AF040, 12);
    osStartThread(&D_8013ED48);
    osCreateThread(&D_800A91F0, 0, func_80027D90, NULL, &D_8013E7D0, 1);
    osStartThread(&D_800A91F0);
    while (1) {
        osRecvMesg(&D_8010E650, &msg, 1);
        switch ((s16)*msg) {
        case 1:
            if (D_800AD038 == 0) {
                if ((*(s32 (*))&D_8006EE70) != 0 && started == 0) {
                    started = 1;
                    osContStartReadData(&D_8010E650);
                }
                D_80106C7C++;
                D_800A7F18++;
                if ((*(u16 (*))&D_8013C7C0) != 0) {
                    D_80109DF6 -= (*(u16 (*))&D_8013C7C0);
                    if (D_80109DF6 < 100) {
                        (*(u16 (*))&D_8013C7C0) = 0;
                        func_80052D88(D_800A7F68);
                        D_8011551C = 0;
                    }
                }
                func_80021640();
                D_800AD03A = 0;
                t = osGetTime();
                func_80041F04(D_80115408);
                D_800A7A68 = osGetTime() - t;
            }
            if (pending < 2) {
                t = osGetTime();
                func_8000BA3C(&D_801182A0[D_800717E0]);
                D_80140F50 = osGetTime() - t;
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
            osStopTimer(&D_8012F118);
            osViBlack(1);
            func_80052D88(D_800A7F68);
            func_8004E738(3, 0);
            break;
        }
    }
}
