#include "common.h"

/* extern */
                             /* extern */

s32 MusStartSong(void) {
    s32 temp_v0_7;

    temp_v0_7 = player_text_329C();
    MusHandleUnPause(temp_v0_7);
    return temp_v0_7;
}
