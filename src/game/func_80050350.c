#include "common.h"
#include "libmus/player.h"

typedef struct {
    s32 unk_0; s32 unk_4; u8 unk_8; u8 unk_9; u8 padA[2]; s32 unk_C; s32 unk_10;
} Item;
static inline f32 SEXT8F(u8 c) { if (c & 0x80) return c - 0x100; return c; }
/* relocates a loaded header in place (once): pointers, number table, items */
void func_80050350(s32 base, s32 arg1) {
    ptr_bank_t *hd = (ptr_bank_t *)base;
    s32 i;
    u8 c;
    f32 f;
    Num *n;
    Item *it;
    if (!(hd->flags & 0x80000000)) {
        hd->flags |= 0x80000000;
        func_80050880(&hd->basenote, hd, 3);
        func_80050880((*(Item * * *)&hd->wave_list), hd, hd->count);
        for (i = 0; i < hd->count; i++) {
            n = &(*(Num * *)&hd->detune)[i];
            c = n->b;
            f = SEXT8F(c);
            n->f = f / 100.0;
            c = hd->basenote[i] - '0';
            f = SEXT8F(c);
            n->f += f;
            n = (Num *)(*(Item * * *)&hd->wave_list)[i];
            if (((Item *)n)->unk_9 == 0) {
                s32 v = ((Item *)n)->unk_0;
                if ((v & 0xFF000000) != 0xFF000000) {
                    v += arg1;
                    ((Item *)n)->unk_0 = v;
                }
                (*(Item * * *)&hd->wave_list)[i]->unk_9 = 1;
                it = (*(Item * * *)&hd->wave_list)[i];
                if (it->unk_C != 0) {
                    it->unk_C += base;
                }
                it = (*(Item * * *)&hd->wave_list)[i];
                if (it->unk_8 == 0) {
                    it->unk_10 += base;
                }
            }
        }
        osWritebackDCacheAll();
    }
}
