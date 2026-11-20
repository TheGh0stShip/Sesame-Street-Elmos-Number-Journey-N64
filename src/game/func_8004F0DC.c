#include "common.h"
#include "libmus/player.h"

typedef struct { u8 pad[0x1C]; } Slot;
extern Slot *D_800A0548;
void func_80055F90(Slot *);
/* drains the event queue, then advances every active entry by one tick */
s32 func_8004F0DC(void) {
    s32 i;
    channel_t *e;
    f32 f;

    while (D_800A0584 != D_800A0588) {
        func_8004EFC4(&D_800A0590[D_800A0584]);
        if (++D_800A0584 == D_800A058C) {
            D_800A0584 = 0;
        }
    }
    for (i = -4, e = mus_channels; i < max_channels - 4; i++, e++) {
        if ((*(s32 *)&e->pdata) == 0 || ((*(s32 *)&e->flags) & 1)) {
            continue;
        }
        if ((*(s32 *)&e->sample) != 0) {
            func_8004F914(e, i);
        }
        e->channel_frame += (*(u16 *)&e->channel_tempo);
        if (e->length != 0x7FFF) {
            while ((s32)(e->note_end_frame - e->channel_frame) < 0) {
                if ((*(s32 *)&e->pdata) == 0) {
                    break;
                }
                func_8004F3CC(e, i);
            }
            if ((*(s32 *)&e->pdata) == 0) {
                continue;
            }
        }
        if ((*(s32 *)&e->pvolume) != 0 && (s32)(e->volume_frame - e->channel_frame) < 0) {
            func_8004FFE4(e);
        }
        if ((*(s32 *)&e->ppitchbend) != 0 && (s32)(e->pitchbend_frame - e->channel_frame) < 0) {
            func_800500A8(e);
        }
        if (e->stopping != -1) {
            if (--e->stopping == -1) {
                (*(s32 *)&e->pvolume) = 0;
                (*(s32 *)&e->ppitchbend) = 0;
                (*(s32 *)&e->song_addr) = 0;
                (*(s32 *)&e->fx_addr) = 0;
                (*(s32 *)&e->handle) = 0;
                (*(s32 *)&e->sample) = 0;
                (*(s32 *)&e->pdata) = 0;
                if (e->playing != 0) {
                    e->playing = 0;
                    func_80055F90(&D_800A0548[i]);
                }
            }
        }
        if (e->playing != 0) {
            if (e->env_phase != 0) {
                func_8004FC60(e);
            }
            if (e->sweep_speed != 0 && (s32)(e->sweep_frame - e->channel_frame) < 0) {
                func_8004FE70(e);
            }
            f = e->freqoffset;
            if (e->vib_speed != 0) {
                f += func_8004FF80(e);
            }
            if (e->wobble_on_speed != 0) {
                f += func_8004FF28(e);
            }
            if ((*(s32 *)&e->sample) == 0) {
                func_8004FAE4(e, i, f);
                func_8004F990(e, i);
            }
        }
        (*(s16 *)&e->ticks) = (e->channel_frame - e->note_start_frame) >> 8;
    }
    return D_800A0558;
}
