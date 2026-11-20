#include "common.h"
#include "libmus/player.h"

typedef struct { u8 unk_00[0x1C]; } Chan;
extern Chan *D_800A0548;
void func_80055F90(Chan *);
void func_80055F00(Chan *, s32);
/* (re)starts channel idx with o->unk_08, stopping it first if active */
void func_8004F914(channel_t *o, s32 idx) {
    if (o->playing != 0) {
        func_80055F90(&D_800A0548[idx]);
    }
    o->playing = 1;
    func_80055F00(&D_800A0548[idx], o->sample);
    o->sample = 0;
}
