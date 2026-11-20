#include "common.h"

extern S800773B4 *D_80077BC4;
/* scales arg by unk_40 / 1e6, rounded */
s32 func_800564C4(s32 arg0) {
    return (f32)((f32)arg0 * (f32)D_80077BC4->unk_40 / 1000000.0 + 0.5);
}
