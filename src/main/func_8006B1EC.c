typedef struct { u8 unk_00[0x11A8]; s32 unk_11A8; u8 unk_11AC[0x28]; u32 unk_11D4; u32 unk_11D8; s8 unk_11DC; s8 unk_11DD; } Stream;
/* advance a stream by up to n units; returns amount consumed */
u32 func_8006B1EC(Stream *s, u32 n) {
    u32 avail;
    u32 left;
    if (s->unk_11DD == 3) {
        if (s->unk_11A8 > 0) {
            s->unk_11A8 -= n;
            if (s->unk_11A8 <= 0) { s->unk_11DD = 4; s->unk_11A8 = 0; }
        }
        left = s->unk_11D8 - s->unk_11D4;
        avail = (n < left) ? n : left;
        s->unk_11D4 += avail;
        return avail;
    }
    return 0;
}
