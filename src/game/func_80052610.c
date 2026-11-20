#include "common.h"

typedef struct { void *unk_0; s32 unk_4; } Pair;
extern void *__MusIntMemMalloc(s32);
extern void osCreateThread(void *, s32, void *, void *, void *, s32);
extern void osStartThread(void *);
extern s32 osAiSetFrequency(s32);
extern Pair *D_800A07A4;
extern void *D_800A07A8;
/* audio setup: build config, allocate buffers, start audio thread */
void func_80052610(Params *p, s32 arg1, u8 arg2) {
    Cfg c;
    s32 n;
    u32 i;
    c.unk_0 = c.unk_4 = p->unk_4;
    c.unk_8 = p->unk_28;
    c.unk_10 = func_80052010(p->unk_38, p->unk_3C);
    c.unk_1C = arg2;
    c.unk_18 = osAiSetFrequency(p->unk_2C);
    c.unk_14 = func_80052984();
    func_800517C0(D_8012FDF0, &c);
    n = func_80053620(p->unk_34, c.unk_18, arg1, 0xF);
    D_800A07A8 = __MusIntMemMalloc(p->unk_30 * 8);
    D_800A07A4 = __MusIntMemMalloc(0x18);
    for (i = 0; i < 3; i++) {
        D_800A07A4[i].unk_0 = __MusIntMemMalloc(n * 4);
    }
    D_800A07A0 = __MusIntMemMalloc(0x2000);
    osCreateThread(D_800A05F0, 3, D_80052748, NULL, D_800A07A0 + 0x2000, p->unk_C);
    osStartThread(D_800A05F0);
}
