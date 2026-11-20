#include "common.h"
/* reads one byte from a stream, forwards it when mode 1 is active, returns the advanced pointer */


u8 *Fchangefx(void *unused, u8 *p) {
    u8 c = *p++;
    if (D_800A0570 == 1) {
        ChangeCustomEffect(c);
    }
    return p;
}
