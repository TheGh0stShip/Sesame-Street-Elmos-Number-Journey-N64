#include "common.h"
#include "libmus/player.h"

/* size 7 */



/* reads a 1-2 byte envelope index from the stream and loads that envelope's rates */
u8 *func_8004DF20(channel_t *v, u8 *p) {
    u32 idx = *p++;
    env_t *e;
    s32 n;
    s32 m;

    if (idx & 0x80) {
        idx &= 0x7F;
        idx <<= 8;
        idx |= *p++;
    }
    if ((n = v->song_addr->EnvelopeData[idx].speed) == 0) {
        e = &v->song_addr->EnvelopeData[idx];
        n = 1;
    } else {
        e = &v->song_addr->EnvelopeData[idx];
    }
    v->env_speed = n;
    v->env_speed_calc = 0x400 / n;
    v->env_init_vol = e->init_vol;
    n = e->attack_time;
    v->env_attack_time = n;
    m = e->peak_vol;
    v->env_attack_calc = 1.0 / (f32)n * (f32)(m - v->env_init_vol);
    v->env_max_vol = m;
    n = e->decay_time;
    v->env_decay_time = n;
    m = e->sustain_vol;
    v->env_decay_calc = 1.0 / (f32)n * (f32)(m - v->env_max_vol);
    v->env_sustain_vol = m;
    n = e->release_time;
    v->env_release_time = n;
    v->env_release_calc = 1.0 / (f32)n;
    return p;
}
