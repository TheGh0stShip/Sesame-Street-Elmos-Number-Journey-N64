#include "common.h"
#include "libmus/player.h"
/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */



void func_8004FE54(struct channel_s *arg0) {
    arg0->sweep_timer = 0;
    arg0->sweep_frame = arg0->note_start_frame;
    arg0->sweep_dir = arg0->pan & 0x40;
}
/* Warning: struct Struct_anon_30 is not defined (only forward-declared) */
