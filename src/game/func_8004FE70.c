#include "common.h"
#include "libmus/player.h"

/* advances a ramping 7-bit value (unk_BD) up and down until unk_94 catches up to unk_C */
void func_8004FE70(channel_t *p) {
    u32 v;

    do {
        v = p->sweep_timer + p->sweep_speed;
        (*(s32 *)&p->sweep_frame) += 0x100;
        if (v < 0x40) {
            p->sweep_timer = v;
        } else {
            p->sweep_timer = v & 0x3F;
            v >>= 6;
            if (p->sweep_dir == 0) {
                p->pan += v;
                if (p->pan >= 0x80) {
                    p->pan = 0x7F;
                    p->sweep_dir = 1;
                }
            } else {
                p->pan -= v;
                if (p->pan >= 0x80 || p->pan == 0) {
                    p->pan = 0;
                    p->sweep_dir = 0;
                }
            }
        }
    } while ((*(s32 *)&p->sweep_frame) - (*(s32 *)&p->channel_frame) < 0);
}
