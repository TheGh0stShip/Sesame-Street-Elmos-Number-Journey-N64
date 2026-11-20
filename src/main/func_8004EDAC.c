typedef struct { s32 off; s32 unk_4; } RelEnt;
typedef struct { s32 count; s32 unk_4; s32 unk_8; s32 flags; s32 unk_10; s32 off_14; RelEnt ent[1]; } RelHdr;
extern RelHdr *D_800A0578;
void func_8004EDAC(RelHdr *h) {
    s32 i;
    RelHdr *hdr = h;
    if (hdr->flags & 1) return;
    if (D_800A0578 == NULL) D_800A0578 = hdr;
    hdr->flags = 1;
    hdr->unk_10 = 0;
    hdr->off_14 = (s32)h + hdr->off_14;
    for (i = 0; i < hdr->count; i++) hdr->ent[i].off = (s32)h + hdr->ent[i].off;
}
