#include "common.h"

void MusFxBankInitialize(RelHdr *h) {
    s32 i;
    RelHdr *hdr = h;
    if (hdr->flags & 1) return;
    if (D_800A0578 == NULL) D_800A0578 = hdr;
    hdr->flags = 1;
    hdr->unk_10 = 0;
    hdr->off_14 = (s32)h + hdr->off_14;
    for (i = 0; i < hdr->count; i++) hdr->ent[i].off = (s32)h + hdr->ent[i].off;
}
