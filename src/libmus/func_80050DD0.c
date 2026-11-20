#include "common.h"
#include "libmus/player.h"

/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */


s32 Fstop(struct channel_s *arg0) {
    arg0->pvolume = 0;
    arg0->ppitchbend = 0;
    arg0->song_addr = 0;
    arg0->fx_addr = 0;
    arg0->handle = 0;
    arg0->sample = 0;
    return 0;
}
