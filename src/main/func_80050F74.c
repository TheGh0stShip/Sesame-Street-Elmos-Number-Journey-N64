/* Sequence tempo command: sets tempo from a byte and propagates it to matching channels */
typedef struct {
    u8 pad0[0x74];
    s32 unk_74;
    s32 unk_78;
    u8 pad7C[0x98 - 0x7C];
    s16 unk_98;
    u8 pad9A[2];
    s16 unk_9C;
} Player;
typedef struct {
    u8 pad0[0x74];
    s32 unk_74;
    u8 pad78[0x9C - 0x78];
    s16 unk_9C;
    u8 pad9E[0xA8 - 0x9E];
    s16 unk_A8;
    u8 padAA[0x13C - 0xAA];
} Chan;
extern s32 D_800A0554;
extern s32 D_800A0544;
extern Chan *D_800A054C;

u8 *func_80050F74(Player *p, u8 *cmd) {
    s32 tempo;
    s32 vol;
    s32 i;

    tempo = ((*cmd++ * 3) << 13) / 120 / D_800A0554;
    vol = (tempo * p->unk_98) >> 7;
    if (p->unk_78 != 0) {
        p->unk_9C = tempo;
    } else {
        Chan *c = D_800A054C;
        for (i = 0; i < D_800A0544; i++, c++) {
            if (c->unk_74 == p->unk_74) {
                c->unk_A8 = tempo;
                c->unk_9C = vol;
            }
        }
    }
    return cmd;
}
