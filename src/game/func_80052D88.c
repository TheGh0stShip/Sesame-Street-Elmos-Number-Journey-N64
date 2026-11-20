#include "common.h"
#include "codec/stream.h"

/* size 0x1200 */

extern stream_t *D_800A07C0;

/* clears the first entry whose id equals the given id */
void func_80052D88(u16 id) {
    s32 i;
    stream_t *e;

    if (id != 0xFFFF) {
        for (i = 0; i < D_800A07C4; i++) {
            if (D_800A07C0[i].id == id) {
                D_800A07C0[i].id = 0xFFFF;
                return;
            }
        }
    }
}
