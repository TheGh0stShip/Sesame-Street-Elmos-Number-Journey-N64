#include "common.h"
#include "libmus/player.h"

/* step an envelope: stage in unk_C3, level in unk_C4 */
void func_8004FC60(channel_t *e) {
    s32 t;
    if ((s32)(e->release_frame - e->channel_frame) < 0 && e->env_phase < 4) {
        e->env_phase = 4;
        e->env_count = 1;
        e->release_start_vol = e->env_current;
    }
    if (--e->env_count != 0) return;
    e->env_count = e->env_speed;
    switch (e->env_phase) {
    case 1:
        t = (((e->channel_frame - e->note_start_frame) >> 8) * e->env_speed_calc) >> 10;
        if (t < e->env_attack_time) {
            e->env_current = e->env_init_vol + (s32)(e->env_attack_calc * t);
        } else {
            e->env_phase++;
            e->env_current = e->env_max_vol;
        }
        break;
    case 2:
        t = ((((e->channel_frame - e->note_start_frame) >> 8) - e->env_attack_time) * e->env_speed_calc) >> 10;
        if (t < e->env_decay_time) {
            e->env_current = e->env_max_vol + (s32)(e->env_decay_calc * t);
        } else {
            e->env_phase++;
            e->env_current = e->env_sustain_vol;
        }
        break;
    case 3:
        break;
    case 4:
        t = (((e->channel_frame - e->release_frame) >> 8) * e->env_speed_calc) >> 10;
        if (t < e->env_release_time) {
            e->env_current = e->release_start_vol - (s32)(e->env_release_calc * t * e->release_start_vol);
        } else {
            e->env_phase++;
            e->env_current = 0;
        }
        break;
    }
}
