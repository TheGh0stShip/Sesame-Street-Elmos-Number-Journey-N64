#include "common.h"
#include "libmus/player.h"

/* sequence player: read three command bytes into channel fields */
u8 *Fwobble(channel_t *c, u8 *p)
{
    c->wobble_amount = *p++;
    c->wobble_on_speed = *p++;
    c->wobble_off_speed = *p++;
    return p;
}
