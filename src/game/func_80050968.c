#include "common.h"
#include "libmus/player.h"

extern channel_t *D_800A0550;
void func_8005057C(channel_t *);
/* allocates a voice (free one, else lowest-priority one) and starts it */
s32 func_80050968(fx_t *b, s32 prog, s32 key, s32 vel, s32 prio) {
    channel_t *v;
    channel_t *best;
    s32 i;
    s32 min;
    if (prio == -1) {
        prio = b->effects[prog].priority;
    }
    min = prio + 1;
    for (i = 4, v = D_800A0550; i < max_channels; i++, v++) {
        if (v->pdata == 0) {
            func_8005057C(v);
            v->fx_number = prog;
            v->fx_addr = b;
            v->volscale = key;
            v->panscale = vel;
            v->priority = prio;
            v->handle = D_800A0560++;
            if (b->ptr_addr != 0) {
                v->sample_bank = b->ptr_addr;
            }
            v->pdata = v->pbase = b->effects[prog].fxdata;
            return v->handle;
        }
        if (v->fx_addr != NULL && v->priority < min) {
            min = v->priority;
            best = v;
        }
    }
    if (min < prio) {
        func_8005057C(best);
        best->fx_number = prog;
        best->fx_addr = b;
        best->volscale = key;
        best->panscale = vel;
        best->priority = prio;
        best->handle = D_800A0560++;
        if (b->ptr_addr != 0) {
            best->sample_bank = b->ptr_addr;
        }
        best->pdata = best->pbase = b->effects[prog].fxdata;
        return best->handle;
    }
    return 0;
}
