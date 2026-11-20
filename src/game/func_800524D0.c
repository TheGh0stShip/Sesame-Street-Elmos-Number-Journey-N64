#include "common.h"

extern Sys *D_800A05E4;
s32 osRecvMesg(void *mq, void *msg, s32 flag);
/* receive messages until one whose first halfword is 1 */
void func_800524D0(void) {
    s16 *msg;
    do {
        osRecvMesg(D_800A05E4->mq, &msg, 1);
        osRecvMesg(D_800A05E4->mq, NULL, 0);
    } while (*msg != 1);
}
