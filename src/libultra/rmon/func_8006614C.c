#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"
#include "PRinternal/macros.h"
/* not in the SDK headers */
extern OSMesg D_800A7A00[];
extern u64 D_800A77E0[0x40];
extern u8 D_800A79E0[];
extern OSMesg D_800A39C0[8];

extern OSThread D_800A3810;/* osCreateMesgQueue */
  /* osSetEventMesg */
void osCreateThread(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);/* osStartThread */

void __rmonInit(void) {
    osCreateMesgQueue(&__rmonMQ, D_800A39C0, 8);
    osSetEventMesg(10, &__rmonMQ, (OSMesg)2);
    osSetEventMesg(11, &__rmonMQ, (OSMesg)4);
    osSetEventMesg(12, &__rmonMQ, (OSMesg)8);
    osSetEventMesg(13, &__rmonMQ, (OSMesg)0);
    osCreateThread(&D_800A3810, 0, __rmonIOhandler, NULL, D_800A77E0 + 0x40, 255);
    osCreatePiManager(150, D_800A7A00, D_800A79E0, 8);
    osStartThread(&D_800A3810);
}
