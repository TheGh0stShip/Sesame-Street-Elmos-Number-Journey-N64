#include "common.h"
#include "libmus/player.h"

/* Sequence player: run each matching player's command stream up to a 0xAB marker equal to `marker` */


extern SeqCmdFunc D_80077620[];

s32 MusStartSongFromMarker(s32 id, s32 marker) {
    s32 h = ((s32 (*)(void))func_80050B10)();
    channel_t *p = mus_channels;
    s32 i;
    u8 *s;
    u8 *q;
    u8 c;

    for (i = 0; i < max_channels; i++, p++) {
        if (p->handle != h || p->song_addr != id || p->pdata == NULL) {
            continue;
        }
    loop:
        {
            s = p->pdata;
            c = *s;
            if (c >= 0x80) {
                if (c == 0xAB && s[1] == marker) {
                    goto out;
                }
                p->pdata = (*(SeqCmdFunc *)((u8 *)D_80077620 + ((c & 0x7F) << 2)))(p, s + 1);
            } else {
                p->pdata = s + 1;
                if (p->velocity_on) {
                    p->pdata = s + 2;
                    p->velocity = s[1];
                    if (p->velocity >= 0x80) {
                        p->velocity &= 0x7F;
                        p->velocity_on = 0;
                        p->default_velocity = p->velocity;
                    }
                } else {
                    p->velocity = p->default_velocity;
                }
                if (p->fixed_length == 0 || p->ignore != 0) {
                    u8 *t = p->pdata;
                    u8 e;
                    p->ignore = 0;
                    p->pdata = t + 1;
                    e = *t;
                    if (e < 0x80) {
                        p->length = e;
                    } else {
                        p->pdata = t + 2;
                        p->length = t[1] + ((e & 0x7F) << 8);
                    }
                } else {
                    p->length = p->fixed_length;
                }
                p->channel_frame += p->length << 8;
            }
        }
        if (p->pdata != NULL) goto loop;
    out:

        p->note_end_frame = p->channel_frame;
        q = p->pdata;
        if (q != NULL) {
            s32 d = q[2];
            u8 *n = q + 3;
            if (d >= 0x80) {
                d &= 0x7F;
                d <<= 8;
                d |= q[3];
                n = q + 4;
            }
            p->ticks = 0;
            p->length = d;
            p->pdata = n;
            p->channel_frame -= d << 8;
        }
        p->note_start_frame = p->channel_frame;
        if (p->pvolume) {
            func_8004FFE4(p);
        }
        if (p->ppitchbend) {
            func_800500A8(p);
        }
    }
    MusHandleUnPause(h);
    return h;
}
