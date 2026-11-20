#include "common.h"

/* spSetAttribute: sp->attr |= attr */
typedef struct { s16 x, y, width, height; f32 scalex, scaley; s16 expx, expy; u16 attr; } Sprite;
void func_8005D050(Sprite *sp, s32 attr) { sp->attr |= attr; }
