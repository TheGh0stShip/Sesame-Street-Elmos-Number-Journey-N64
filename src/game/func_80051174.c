#include "common.h"
#include "libmus/player.h"

/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */


s32 func_80051174(struct channel_s *arg0, s32 arg1) {
    arg0->vib_speed = 0;
    (*(s32 *)&arg0->vibrato) = 0;
    return arg1;
}
