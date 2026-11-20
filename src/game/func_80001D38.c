#include "common.h"


void func_80001D38(s32 arg0) {
    void *sp18;

    if ((u32)D_8006F6CC < 8) {
        sp18 = (void *)&((struct n64dk_e64 *)&D_80133FF0)[D_8006F6CC];
        strcpy(((s32)sp18), arg0);
        if (D_8006F6CC == 0) {
            addActorDL(((s32)&D_80001DE4), ((s32)sp18), 6, 0, 0);
        }
        D_8006F6CC++;
    }
}
