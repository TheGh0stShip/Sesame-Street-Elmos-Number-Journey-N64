#include "common.h"
#include "codec/stream.h"

extern stream_t *D_800A07C0;
/* per-voice update: restart voices in state 2, free voices in state 4 */
void func_80052B44(void) {
    s32 i;
    stream_t *p;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].id != 0xFFFF) {
            func_8006AE74(&D_800A07C0[i]);
            switch ((*(u8 *)&D_800A07C0[i].state)) {
            case 2:
                p = &D_800A07C0[i];
                p->first = 1;
                p->frac = 0;
                D_800A07C0[i].pending = 0;
                D_800A07C0[i].pitch = (f32)((D_800A07C0[i].sample_rate << 15) / D_800A07C8) / 32768.0f;
                func_8006B258(&D_800A07C0[i]);
                break;
            case 4:
                D_800A07C0[i].id = 0xFFFF;
                break;
            }
        }
    }
}
