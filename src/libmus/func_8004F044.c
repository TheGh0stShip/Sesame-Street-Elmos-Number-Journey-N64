#include "common.h"
#include "libmus/player.h"

/* pushes an 8-byte message onto a ring buffer; returns 0 when full */
/* head */
/* tail */
 /* size */
/* buffer */
 /* memcpy */
s32 func_8004F044(mus_msg_t *m) {
    s32 next = (D_800A0588 + 1) % D_800A058C;
    if (next == D_800A0584) {
        return 0;
    }
    func_800529BC(&D_800A0590[D_800A0588], m, sizeof(mus_msg_t));
    D_800A0588 = next;
    return 1;
}
