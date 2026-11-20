typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
} Env;

typedef struct {
    u8 pad00[0x18];
    Env *unk_18;
    u8 pad1C[4];
    u16 *unk_20;
} Bank;

typedef struct {
    u8 pad00[0x14];
    u16 *unk_14;
} Bank2;

typedef struct {
    u8 pad00[0x28];
    f32 *unk_28;
    s32 *unk_2C;
} Inst;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04;
    u8 unk_05;
} NoteMap;

typedef struct {
    u8 pad[0x1C];
} Voice;

typedef struct Track {
    u8 pad00[4];
    u8 *unk_04;
    s32 unk_08;
    s32 unk_0C;
    u8 pad10[0x2C - 0x10];
    f32 unk_2C;
    u8 pad30[0x3C - 0x30];
    s32 unk_3C;
    s32 unk_40;
    u8 pad44[0x4C - 0x44];
    f32 unk_4C;
    f32 unk_50;
    s32 unk_54;
    f32 unk_58;
    f32 unk_5C;
    f32 unk_60;
    s32 unk_64;
    u8 pad68[0x74 - 0x68];
    Bank *unk_74;
    Bank2 *unk_78;
    Inst *unk_7C;
    u8 pad80[4];
    NoteMap *unk_84;
    u8 pad88[0x9A - 0x88];
    u16 unk_9A;
    u8 pad9C[0xA0 - 0x9C];
    u16 unk_A0;
    u8 padA2[0xAA - 0xA2];
    s16 unk_AA;
    u16 unk_AC;
    u16 unk_AE;
    u8 padB0[0xB7 - 0xB0];
    u8 unk_B7;
    u8 padB8;
    u8 unk_B9;
    u8 unk_BA;
    u8 unk_BB;
    u8 padBC;
    u8 unk_BD;
    u8 padBE;
    u8 unk_BF;
    u8 unk_C0;
    u8 unk_C1;
    u8 unk_C2;
    u8 unk_C3;
    u8 unk_C4;
    s8 unk_C5;
    u8 unk_C6;
    u8 unk_C7;
    u8 unk_C8;
    u8 unk_C9;
    u8 unk_CA;
    u8 unk_CB;
    u8 unk_CC;
    u8 unk_CD;
    u8 padCE;
    u8 unk_CF;
    u8 unk_D0;
    s8 unk_D1;
    u8 unk_D2;
    u8 unk_D3;
    u8 unk_D4;
    u8 padD5;
    u8 unk_D6;
    u8 unk_D7;
} Track;

extern u8 *(*D_80077620[])(Track *, u8 *);
extern Voice *D_800A0548;
extern s32 D_800A0558;
void func_8004F914(Track *, s32);
void func_8004FBF0(Track *);
void func_8004FE54(Track *);
void func_80055C90(Voice *, u8);
void func_80055E50(Voice *, s16, s32);
void func_80055F90(Voice *);

static inline Env *env_at(u32 b, s32 i) { return (Env *) (i * 7 + b); }
/* Sequencer: runs command bytes on track t, then starts its next note (channel ch). */
void func_8004F3CC(Track *t, s32 ch) {
    u8 *p;
    s32 note;
    u8 c;
    Inst *inst;
    Env *env;
    u16 key;
    s32 n;
    s32 rate;
    s32 bend;
    u8 b;
    f32 *tune;
    f32 f;
    u8 vol;

    p = t->unk_04;
    if (p != NULL) {
    next:
        c = *p;
        if (c >= 0x80) {
            p = D_80077620[c & 0x7F](t, p + 1);
            if (p != NULL) {
                goto next;
            }
        }
    }
    t->unk_04 = p;
    if (p != NULL) {
    t->unk_4C = t->unk_50;
    note = *t->unk_04++;
    if (t->unk_D2 != 0) {
        t->unk_BB = *t->unk_04++;
        if (t->unk_BB >= 0x80) {
            t->unk_BB &= 0x7F;
            t->unk_D2 = 0;
            t->unk_D3 = t->unk_BB;
        }
    } else {
        t->unk_BB = t->unk_D3;
    }

    if (t->unk_AC != 0) {
        if (t->unk_B7 != 0) {
            t->unk_B7 = 0;
            c = *t->unk_04++;
            if (c < 0x80) {
                t->unk_9A = c;
            } else {
                t->unk_9A = ((c & 0x7F) << 8) + *t->unk_04++;
            }
        } else {
            t->unk_9A = t->unk_AC;
        }
    } else {
        c = *t->unk_04++;
        if (c < 0x80) {
            t->unk_9A = c;
        } else {
            t->unk_9A = ((c & 0x7F) << 8) + *t->unk_04++;
        }
    }

    t->unk_40 = t->unk_3C;
    t->unk_3C += t->unk_9A << 8;
    t->unk_AA = 0;
    t->unk_D1 = 0;
    t->unk_D0 = t->unk_CF;
    if (t->unk_74 != NULL && t->unk_84 == NULL && t->unk_74->unk_20[t->unk_AE] == 0xFFFF) {
        note = 0x60;
    }

    if (note != 0x60) {
        inst = t->unk_7C;
        if (t->unk_84 != NULL) {
            t->unk_AE = t->unk_84[note].unk_00;
            t->unk_BD = t->unk_84[note].unk_04 >> 1;
            env = env_at((u32) t->unk_74->unk_18, t->unk_84[note].unk_02);
            rate = env->unk_00;
            if (rate == 0) {
                rate = 1;
            }
            t->unk_BF = rate;
            t->unk_64 = 0x400 / rate;
            t->unk_C0 = env->unk_01;
            rate = env->unk_02;
            t->unk_C6 = rate;
            b = env->unk_03;
            t->unk_C1 = b;
            t->unk_58 = 1.0 / (f32) rate * (f32) (b - t->unk_C0);
            rate = env->unk_04;
            t->unk_C7 = rate;
            b = env->unk_05;
            t->unk_C2 = b;
            t->unk_5C = 1.0 / (f32) rate * (f32) (b - t->unk_C1);
            rate = env->unk_06;
            t->unk_C8 = rate;
            t->unk_60 = 1.0 / (f32) rate;
            note = t->unk_84[note].unk_05;
        }
        if (t->unk_D6 == 0) {
            func_8004FBF0(t);
        }
        if (t->unk_D4 != 0) {
            func_8004FE54(t);
        }
        key = t->unk_AE;
        if (t->unk_74 != NULL) {
            key = t->unk_74->unk_20[key];
        } else {
            key = t->unk_78->unk_14[key];
        }
        if (t->unk_D7 == 0) {
            t->unk_08 = inst->unk_2C[key];
            if (t->unk_C9 != 0 && t->unk_A0 != 0) {
                t->unk_A0 = 0;
                func_80055E50(&D_800A0548[ch], 0, D_800A0558);
            } else {
                func_8004F914(t, ch);
            }
        }
        tune = inst->unk_28;
        f = (f32) note + tune[key];
        bend = t->unk_B9 * (1 - t->unk_BA);
        t->unk_2C = f;
        vol = bend;
        t->unk_2C = f + ((bend & 0x80) ? vol - 0x100 : vol);
        vol = t->unk_CA;
        if (vol != t->unk_CC) {
            t->unk_CC = vol;
            func_80055C90(&D_800A0548[ch], t->unk_CB + (((0x80 - t->unk_CB) * vol) >> 7));
        }
    } else if (t->unk_C3 < 4) {
        t->unk_C3 = 4;
        t->unk_C5 = 1;
        t->unk_54 = t->unk_0C;
        t->unk_CD = t->unk_C4;
    }
    } else {
        if (t->unk_C9 != 0) {
            t->unk_C9 = 0;
            func_80055E50(&D_800A0548[ch], 0, D_800A0558);
            func_80055F90(&D_800A0548[ch]);
        }
    }
}
