#include "common.h"

/* matherr: the SVID hook the math error handler calls; returning 0 lets it set errno */
s32 matherr(struct exception *e)
{
    s32 rv = 0;
    return rv;
}
