typedef struct { s32 unk_00; s32 unk_04; } Prog;
typedef struct { u8 unk_00[0x10]; s32 unk_10; u8 unk_14[4]; Prog p[1]; } Bank;
typedef struct {
    u8 unk_00[4]; s32 unk_04; u8 unk_08[0x44 - 8]; s32 unk_44; s32 unk_48;
    u8 unk_4C[0x78 - 0x4C]; Bank *unk_78; s32 unk_7C; s32 unk_80;
    u8 unk_84[0x9E - 0x84]; s16 unk_9E; u8 unk_A0[6]; s16 unk_A6;
    u8 unk_A8[0xB0 - 0xA8]; s16 unk_B0; u8 unk_B2[0x13C - 0xB2];
} Voice;
extern s32 D_800A0544;
extern Voice *D_800A0550;
extern s32 D_800A0560;
void func_8005057C(Voice *);
/* allocates a voice (free one, else lowest-priority one) and starts it */
s32 func_80050968(Bank *b, s32 prog, s32 key, s32 vel, s32 prio) {
    Voice *v;
    Voice *best;
    s32 i;
    s32 min;
    if (prio == -1) {
        prio = b->p[prog].unk_04;
    }
    min = prio + 1;
    for (i = 4, v = D_800A0550; i < D_800A0544; i++, v++) {
        if (v->unk_04 == 0) {
            func_8005057C(v);
            v->unk_A6 = prog;
            v->unk_78 = b;
            v->unk_9E = key;
            v->unk_B0 = vel;
            v->unk_48 = prio;
            v->unk_44 = D_800A0560++;
            if (b->unk_10 != 0) {
                v->unk_7C = b->unk_10;
            }
            v->unk_04 = v->unk_80 = b->p[prog].unk_00;
            return v->unk_44;
        }
        if (v->unk_78 != NULL && v->unk_48 < min) {
            min = v->unk_48;
            best = v;
        }
    }
    if (min < prio) {
        func_8005057C(best);
        best->unk_A6 = prog;
        best->unk_78 = b;
        best->unk_9E = key;
        best->unk_B0 = vel;
        best->unk_48 = prio;
        best->unk_44 = D_800A0560++;
        if (b->unk_10 != 0) {
            best->unk_7C = b->unk_10;
        }
        best->unk_04 = best->unk_80 = b->p[prog].unk_00;
        return best->unk_44;
    }
    return 0;
}
