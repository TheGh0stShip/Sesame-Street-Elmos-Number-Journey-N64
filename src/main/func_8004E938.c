typedef struct { u8 unk_00[0x44]; s32 unk_44; u8 unk_48[0x13C - 0x48]; } Ent;
extern s32 D_800A0544;
extern Ent *D_800A054C;
/* counts entries whose unk_44 equals key */
s32 func_8004E938(s32 key) {
    Ent *e;
    s32 n;
    s32 i;
    s32 cnt;
    if (key == 0) {
        return 0;
    }
    i = 0;
    cnt = D_800A0544;
    e = D_800A054C;
    n = 0;
    while (i < cnt) {
        n += e->unk_44 == key;
        i++;
        e++;
    }
    return n;
}
