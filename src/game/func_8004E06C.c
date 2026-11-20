#include "common.h"
#include "libmus/player.h"

typedef struct { u8 pad[0x1C]; } Chan;
extern Chan *D_800A0548;
extern channel_t *D_800A0550;
void *__MusIntMemMalloc(s32);
void func_80055A50(Evt *);
void func_8005057C(channel_t *);
void func_80055AA0(Chan *, ChanCfg *);

/* audio system init from a config block */
void MusInitialize(Config *c) {
    ChanCfg cfg;
    s32 i;

    D_800BD854 = c->unk_40;
    D_8012C408 = c->unk_00;
    max_channels = c->unk_04 + 4;
    if (D_80000300 != 0) {
        D_800A0554 = 60;
    } else {
        D_800A0554 = 50;
    }
    D_800A0558 = 1000000 / D_800A0554;
    func_800528F0(c->unk_10, c->unk_14);
    func_80052460(c->unk_08);
    D_800A0548 = __MusIntMemMalloc((max_channels - 4) * sizeof(Chan));
    mus_channels = __MusIntMemMalloc(max_channels * sizeof(channel_t));
    D_800A0550 = mus_channels + 4;
    func_8004EF60(c->unk_24);
    D_800A0568 = 0;
    D_800A056C = 0;
    if (c->unk_18 != 0 && c->unk_1C != 0) {
        ((void (*)(s32, s32))MusPtrBankInitialize)(c->unk_18, c->unk_1C);
    }
    D_800A057C = 0;
    D_800A0578 = 0;
    if (c->unk_20 != 0) {
        MusFxBankInitialize(c->unk_20);
    }
    D_800A0580 = 0;
    D_800A0574 = 2;
    func_80052610(c, D_800A0554, 2);
    MusSetMasterVolume(3, 0x7FFF);
    D_800A0560 = 1;
    D_800A0564 = 0x12345678;
    D_800A0530.next = NULL;
    D_800A0538 = func_8004F0DC;
    D_800A0534 = &D_800A0530;
    func_80055A50(&D_800A0530);
    for (i = 0; i < max_channels; i++) {
        mus_channels[i].playing = 0;
        func_8005057C(&mus_channels[i]);
        cfg.unk_04 = 0;
        cfg.unk_00 = c->unk_0C;
        cfg.unk_02 = 0;
        if (i >= 4) {
            func_80055AA0(&D_800A0548[i - 4], &cfg);
        }
    }
    func_8005296C();
}
