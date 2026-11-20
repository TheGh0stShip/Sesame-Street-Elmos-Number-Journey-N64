#include "common.h"
#include "libmus/player.h"

/* counts down a timer, toggling unk_D1 between 0 and unk_D8; returns unk_D1 */
f32 func_8004FF28(channel_t *b) {
    if (--b->wobble_count == 0) {
        if (b->wobble_current == 0) {
            b->wobble_current = b->wobble_amount;
            b->wobble_count = b->wobble_on_speed;
        } else {
            b->wobble_current = 0;
            b->wobble_count = b->wobble_off_speed;
        }
    }
    return b->wobble_current;
}
