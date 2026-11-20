#include "common.h"

typedef struct {
    u32 unk_00;
    u32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    s16 pad0E;
    f32 unk_10;
    u8 pad14[0x1C - 0x14];
    f32 unk_1C;
    s16 *unk_20;
    u8 pad24[4];
} Chan;

typedef struct {
    u8 pad[0x20];
    Chan *unk_20;
} Obj;

typedef struct {
    u8 pad[0x40];
    s32 unk_40;
} Ctx;

extern Ctx *D_80077BC4;
void init_lpfilter(s16 *);

/* alFxParamHdl (reverb.c): set one delay-line parameter, selected by (paramID-2)%8, of delay (paramID-2)/8 */
s32 func_800552F0(void *filter, s32 id, void *param) {
    Obj *obj = (Obj *)filter;
    s32 p = (id - 2) % 8;
    s32 ch = (id - 2) / 8;
    s32 v = *(s32 *)param;

    switch (p) {
    case 0:
        obj->unk_20[ch].unk_00 = v & ~7;
        break;
    case 1:
        obj->unk_20[ch].unk_04 = v & ~7;
        break;
    case 3:
        obj->unk_20[ch].unk_08 = v;
        break;
    case 2:
        obj->unk_20[ch].unk_0A = v;
        break;
    case 4:
        obj->unk_20[ch].unk_0C = v;
        break;
    case 5:
        obj->unk_20[ch].unk_10 = 2.0 * ((f32) v / 1000.0f) / D_80077BC4->unk_40;
        break;
    case 6:
        obj->unk_20[ch].unk_1C = (f32) v / 173123.404906676 * (obj->unk_20[ch].unk_04 - obj->unk_20[ch].unk_00);
        break;
    case 7:
        if (obj->unk_20[ch].unk_20 != NULL) {
            *obj->unk_20[ch].unk_20 = v;
            init_lpfilter(obj->unk_20[ch].unk_20);
        }
        break;
    }
    return 0;
}
