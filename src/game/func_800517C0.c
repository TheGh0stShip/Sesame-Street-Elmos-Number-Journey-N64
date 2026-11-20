#include "common.h"

void n_alSynNew(s32);
extern s32 D_80077BC4;
extern s32 D_8013C6F8;

/* first-time setup: stores arg0, then initialises once with arg1 */
void func_800517C0(s32 arg0, s32 arg1) {
    if (D_80077BC0 == 0) {
        D_80077BC0 = arg0;
        if (D_80077BC4 == 0) {
            func_80051F10(arg1);
            D_8013C6F8 = D_80077BC0;
            D_80077BC4 = D_80077BC0;
            n_alSynNew(arg1);
        }
    }
}
