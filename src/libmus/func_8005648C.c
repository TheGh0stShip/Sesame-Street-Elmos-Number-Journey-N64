#include "common.h"

/* extern */
                                 /* extern */
extern s32 D_80077BC4;

void _n_freePVoice(s32 arg0) {
    ((s32 (*)())alUnlink)();
    alLink(arg0, D_80077BC4 + 0x14);
}
