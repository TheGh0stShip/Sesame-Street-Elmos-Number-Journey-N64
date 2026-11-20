#include "common.h"
#include "libmus/player.h"

/* parses an envelope/rate header from a byte stream into a voice state; returns the advanced pointer */

u8 *Fdefa(channel_t *v, u8 *p)
{
    u8 n;
    u8 t;

    n = *p++;
    if (n == 0)
        n = 1;
    v->env_speed = n;
    v->env_speed_calc = 0x400 / n;
    v->env_init_vol = *p++;
    n = *p++;
    v->env_attack_time = n;
    t = *p++;
    v->env_attack_calc = (1.0 / (f32)n) * (f32)(t - v->env_init_vol);
    v->env_max_vol = t;
    n = *p++;
    v->env_decay_time = n;
    t = *p++;
    v->env_decay_calc = (1.0 / (f32)n) * (f32)(t - v->env_max_vol);
    v->env_sustain_vol = t;
    n = *p++;
    v->env_release_time = n;
    v->env_release_calc = 1.0 / (f32)n;
    return p;
}
