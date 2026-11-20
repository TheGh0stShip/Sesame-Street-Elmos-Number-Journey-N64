#include "common.h"
#include "libmus/player.h"

/* sequence command: read a byte flag; when set, copy field 0x2C to 0x50 */
u8 *Fport(channel_t *p, u8 *s)
{
    u8 c = *s++;
    p->port = c;
    if (c != 0)
        p->port_current = p->base_note;
    return s;
}
