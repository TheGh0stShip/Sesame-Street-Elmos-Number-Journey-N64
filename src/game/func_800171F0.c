#include "common.h"


/* Picks three distinct entries and a random digit pair for each, after resetting the quiz state. */
void func_800171F0(void) {
    s32 i;
    s32 r;
    s32 digit;
    s32 n;

    ((void (*)(Quiz, s32, s32))func_80069C70)(D_80133FC0, 0, sizeof(Quiz));
    for (i = 0; i < 3; i++) {
        D_80133FC0.unk_4[i] = -1;
    }
    D_80133FC0.unk_4[func_800217C4(3)] = D_80108D20;
    for (i = 0; i < 3; i++) {
        if (D_80133FC0.unk_4[i] == -1) {
            r = func_800217C4(D_80106C9C);
            while (r == D_80133FC0.unk_4[0] || r == D_80133FC0.unk_4[1] || r == D_80133FC0.unk_4[2]) {
                r = (r + 1) % D_80106C9C;
            }
            D_80133FC0.unk_4[i] = r;
        }
        digit = D_8012FE90[D_80133FC0.unk_4[i]] - '0';
        n = func_800217C4(10);
        if (n == digit) {
            n++;
            if (n >= 10) {
                n = 1;
            }
        }
        D_80133FC0.unk_14[i] = n;
        D_80133FC0.unk_20[i] = digit - n;
    }
    D_80133FC0.unk_0 = -1;
}
