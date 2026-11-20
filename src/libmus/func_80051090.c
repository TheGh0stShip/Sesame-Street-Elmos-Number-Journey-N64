#include "common.h"
#include "libmus/player.h"

/* sequence player: read three bytes, derive two float rates */
u8 *Fvibup(channel_t *c, u8 *p)
{
    c->vib_delay = *p++;
    c->vib_speed = *p++;
    c->vib_amount = (f32)*p++ / 50.0;
    c->vib_precalc = 6.2831852 / (f32)c->vib_speed;
    return p;
}
