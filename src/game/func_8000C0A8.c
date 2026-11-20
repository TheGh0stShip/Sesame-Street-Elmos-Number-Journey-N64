#include "common.h"

 /* osInitRdb */
/* rmonMain */

extern OSThread D_80081B10;
extern OSThread D_8007D7B0;
void func_8000C0A8(void *arg) {
    osCreatePiManager(150, D_80082FE0, D_80082CC0, 200);
    osInitRdb(D_80128448, 0x100);
    osCreateThread(&D_80081B10, 0, rmonMain, 0, D_80082CC0, 250);
    osStartThread(&D_80081B10);
    osCreateThread(&D_8007D7B0, 6, func_8000C188, arg, D_8007FB10, 10);
    osStartThread(&D_8007D7B0);
    osSetThreadPri(0, 0);
    for (;;) {
    }
}
