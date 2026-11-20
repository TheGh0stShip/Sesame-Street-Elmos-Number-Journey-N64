/* Sequence player: run each matching player's command stream up to a 0xAB marker equal to `marker` */
typedef struct SeqPlayer {
    u8 unk00[4];
    u8 *seq;            /* 0x04 */
    u8 unk08[4];
    s32 time;           /* 0x0C */
    u8 unk10[0x24];
    s32 unk34;          /* 0x34 */
    s32 unk38;          /* 0x38 */
    s32 unk3C;          /* 0x3C */
    s32 unk40;          /* 0x40 */
    s32 handle;         /* 0x44 */
    u8 unk48[0x2C];
    s32 id;             /* 0x74 */
    u8 unk78[0x22];
    u16 delta;          /* 0x9A */
    u8 unk9C[0xE];
    u16 unkAA;          /* 0xAA */
    u16 fixedDelta;     /* 0xAC */
    u8 unkAE[9];
    u8 unkB7;           /* 0xB7 */
    u8 unkB8[3];
    u8 unkBB;           /* 0xBB */
    u8 unkBC[0x16];
    u8 unkD2;           /* 0xD2 */
    u8 unkD3;           /* 0xD3 */
    u8 unkD4[0x68];
} SeqPlayer;

typedef u8 *(*SeqCmdFunc)(SeqPlayer *p, u8 *s);

extern SeqPlayer *D_800A054C;
extern s32 D_800A0544;
extern SeqCmdFunc D_80077620[];

s32 func_80050B10(void);
void func_8004FFE4(SeqPlayer *p);
void func_800500A8(SeqPlayer *p);
void func_8004ECF8(s32 h);

s32 func_8004E350(s32 id, s32 marker) {
    s32 h = func_80050B10();
    SeqPlayer *p = D_800A054C;
    s32 i;
    u8 *s;
    u8 *q;
    u8 c;

    for (i = 0; i < D_800A0544; i++, p++) {
        if (p->handle != h || p->id != id || p->seq == NULL) {
            continue;
        }
    loop:
        {
            s = p->seq;
            c = *s;
            if (c >= 0x80) {
                if (c == 0xAB && s[1] == marker) {
                    goto out;
                }
                p->seq = (*(SeqCmdFunc *)((u8 *)D_80077620 + ((c & 0x7F) << 2)))(p, s + 1);
            } else {
                p->seq = s + 1;
                if (p->unkD2) {
                    p->seq = s + 2;
                    p->unkBB = s[1];
                    if (p->unkBB >= 0x80) {
                        p->unkBB &= 0x7F;
                        p->unkD2 = 0;
                        p->unkD3 = p->unkBB;
                    }
                } else {
                    p->unkBB = p->unkD3;
                }
                if (p->fixedDelta == 0 || p->unkB7 != 0) {
                    u8 *t = p->seq;
                    u8 e;
                    p->unkB7 = 0;
                    p->seq = t + 1;
                    e = *t;
                    if (e < 0x80) {
                        p->delta = e;
                    } else {
                        p->seq = t + 2;
                        p->delta = t[1] + ((e & 0x7F) << 8);
                    }
                } else {
                    p->delta = p->fixedDelta;
                }
                p->time += p->delta << 8;
            }
        }
        if (p->seq != NULL) goto loop;
    out:

        p->unk3C = p->time;
        q = p->seq;
        if (q != NULL) {
            s32 d = q[2];
            u8 *n = q + 3;
            if (d >= 0x80) {
                d &= 0x7F;
                d <<= 8;
                d |= q[3];
                n = q + 4;
            }
            p->unkAA = 0;
            p->delta = d;
            p->seq = n;
            p->time -= d << 8;
        }
        p->unk40 = p->time;
        if (p->unk38) {
            func_8004FFE4(p);
        }
        if (p->unk34) {
            func_800500A8(p);
        }
    }
    func_8004ECF8(h);
    return h;
}
