#include "common.h"

extern void *D_80077BC4;

s32 func_80056514(s32 arg0) {
    return (s32) (f32) (((f64) ((f32) arg0 * (f32) (*(s32 *)((s8 *)(D_80077BC4) + (0x40)))) / D_8007B6E0) + D_8007B6E8) & ~0xF;
}
