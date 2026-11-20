#include "common.h"
#include "libmus/player.h"

/* resets a music track entry: clears it, then sets default field values */
void func_8005057C(channel_t *e) {
    u8 keep = e->playing; u8 *p = (u8 *)e; u32 i; s32 v;
    e->pdata = 0;
    for (i = 0; i < sizeof(channel_t); i++) { *p++ = 0; }
    e->old_reverb = 0xFF;
    e->old_pan = 0xFF;
    e->channel_tempo = e->tempo = 0x6000 / D_800A0554;
    e->default_velocity = 0x7F;
    e->volume = 0x7F;
    e->env_max_vol = 0x7F;
    e->pan = 0x40;
    e->env_speed = 1;
    e->env_attack_time = 1;
    e->env_release_time = 0xF;
    e->old_volume = 0xFFFF;
    e->length = 1;
    e->volume_count = 1;
    e->pitchbend_count = 1;
    e->stopping = -1;
    e->volscale = 0x80;
    e->panscale = 0x80;
    e->temscale = 0x80;
    e->env_decay_time = 0xFF;
    e->env_sustain_vol = 0x7F;
    v = D_800A0568;
    e->old_frequency = 99.9f;
    e->bendrange = 1.0f / 32;
    e->env_attack_calc = 1.0f;
    e->env_decay_calc = 1.0f / 255;
    e->env_release_calc = 1.0f / 15;
    if (v == 0) v = D_800A056C;
    e->sample_bank = v;
    e->playing = keep;
}
