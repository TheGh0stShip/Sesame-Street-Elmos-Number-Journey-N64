#include "common.h"
#include "libmus/player.h"




typedef struct {
    u8 pad[0x1C];
} Voice;


extern u8 *(*D_80077620[])(channel_t *, u8 *);
extern Voice *D_800A0548;
void func_80055C90(Voice *, u8);
void func_80055E50(Voice *, s16, s32);
void func_80055F90(Voice *);

static inline env_t *env_at(u32 b, s32 i) { return (env_t *) (i * 7 + b); }
/* Sequencer: runs command bytes on track t, then starts its next note (channel ch). */
void func_8004F3CC(channel_t *t, s32 ch) {
    u8 *p;
    s32 note;
    u8 c;
    ptr_bank_t *inst;
    env_t *env;
    u16 key;
    s32 n;
    s32 rate;
    s32 bend;
    u8 b;
    f32 *tune;
    f32 f;
    u8 vol;

    p = t->pdata;
    if (p != NULL) {
    next:
        c = *p;
        if (c >= 0x80) {
            p = D_80077620[c & 0x7F](t, p + 1);
            if (p != NULL) {
                goto next;
            }
        }
    }
    t->pdata = p;
    if (p != NULL) {
    t->port_base = t->port_current;
    note = *t->pdata++;
    if (t->velocity_on != 0) {
        t->velocity = *t->pdata++;
        if (t->velocity >= 0x80) {
            t->velocity &= 0x7F;
            t->velocity_on = 0;
            t->default_velocity = t->velocity;
        }
    } else {
        t->velocity = t->default_velocity;
    }

    if (t->fixed_length != 0) {
        if (t->ignore != 0) {
            t->ignore = 0;
            c = *t->pdata++;
            if (c < 0x80) {
                t->length = c;
            } else {
                t->length = ((c & 0x7F) << 8) + *t->pdata++;
            }
        } else {
            t->length = t->fixed_length;
        }
    } else {
        c = *t->pdata++;
        if (c < 0x80) {
            t->length = c;
        } else {
            t->length = ((c & 0x7F) << 8) + *t->pdata++;
        }
    }

    t->note_start_frame = t->note_end_frame;
    t->note_end_frame += t->length << 8;
    t->ticks = 0;
    t->wobble_current = 0;
    t->wobble_count = t->wobble_off_speed;
    if (t->song_addr != NULL && t->pdrums == NULL && t->song_addr->WaveTable[t->wave] == 0xFFFF) {
        note = 0x60;
    }

    if (note != 0x60) {
        inst = t->sample_bank;
        if (t->pdrums != NULL) {
            t->wave = t->pdrums[note].wave;
            t->pan = t->pdrums[note].pan >> 1;
            env = env_at((u32) t->song_addr->EnvelopeData, t->pdrums[note].env);
            rate = env->speed;
            if (rate == 0) {
                rate = 1;
            }
            t->env_speed = rate;
            t->env_speed_calc = 0x400 / rate;
            t->env_init_vol = env->init_vol;
            rate = env->attack_time;
            t->env_attack_time = rate;
            b = env->peak_vol;
            t->env_max_vol = b;
            t->env_attack_calc = 1.0 / (f32) rate * (f32) (b - t->env_init_vol);
            rate = env->decay_time;
            t->env_decay_time = rate;
            b = env->sustain_vol;
            t->env_sustain_vol = b;
            t->env_decay_calc = 1.0 / (f32) rate * (f32) (b - t->env_max_vol);
            rate = env->release_time;
            t->env_release_time = rate;
            t->env_release_calc = 1.0 / (f32) rate;
            note = t->pdrums[note].note;
        }
        if (t->env_trigger_off == 0) {
            func_8004FBF0(t);
        }
        if (t->sweep_speed != 0) {
            func_8004FE54(t);
        }
        key = t->wave;
        if (t->song_addr != NULL) {
            key = t->song_addr->WaveTable[key];
        } else {
            key = t->fx_addr->wave_table[key];
        }
        if (t->trigger_off == 0) {
            t->sample = (*(s32 * *)&inst->wave_list)[key];
            if (t->playing != 0 && t->old_volume != 0) {
                t->old_volume = 0;
                func_80055E50(&D_800A0548[ch], 0, D_800A0558);
            } else {
                func_8004F914(t, ch);
            }
        }
        tune = inst->detune;
        f = (f32) note + tune[key];
        bend = t->transpose * (1 - t->ignore_transpose);
        t->base_note = f;
        vol = bend;
        t->base_note = f + ((bend & 0x80) ? vol - 0x100 : vol);
        vol = t->reverb;
        if (vol != t->old_reverb) {
            t->old_reverb = vol;
            func_80055C90(&D_800A0548[ch], t->reverb_base + (((0x80 - t->reverb_base) * vol) >> 7));
        }
    } else if (t->env_phase < 4) {
        t->env_phase = 4;
        t->env_count = 1;
        t->release_frame = t->channel_frame;
        t->release_start_vol = t->env_current;
    }
    } else {
        if (t->playing != 0) {
            t->playing = 0;
            func_80055E50(&D_800A0548[ch], 0, D_800A0558);
            func_80055F90(&D_800A0548[ch]);
        }
    }
}
