#include "common.h"

typedef struct { u32 w0, w1; } Gwords; typedef union { Gwords words; s64 force_structure_alignment; } Gfx;
typedef struct { s32 unk_00; Gfx *(*unk_04)(s32, Gfx *); } Vt;
typedef struct { u8 pad[0x30]; Vt *unk_30; } Ctx;
extern Ctx *D_80077BC4;
/* Wraps a vtable display-list call with fixed GBI commands before and after. */
Gfx *func_80056610(s32 arg0, Gfx *gfx) {
    { Gfx *_g = gfx++; _g->words.w0 = 0x020004E0; _g->words.w1 = 0x2E0; }
    { Gfx *g2 = D_80077BC4->unk_30->unk_04(arg0, gfx);
    { Gfx *_g = g2++; _g->words.w0 = 0x0C007FFF; _g->words.w1 = 0x07C004E0; }
    { Gfx *_g = g2++; _g->words.w0 = 0x0C007FFF; _g->words.w1 = 0x09300650; }
    return g2; }
}
