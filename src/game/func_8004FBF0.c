#include "common.h"
#include "libmus/player.h"

void func_8004FBF0(channel_t *o) {
    if (o->length != 0x7FFF) {
        if (o->cutoff != 0) {
            o->release_frame = o->note_start_frame + (o->cutoff << 8);
        } else {
            o->release_frame = o->note_end_frame - (o->endit << 8);
        }
    } else {
        o->release_frame = o->note_start_frame + 0x7FFFFFFF;
    }
    o->env_phase = 1;
    o->env_current = o->env_init_vol;
    o->env_count = o->env_speed;
}
