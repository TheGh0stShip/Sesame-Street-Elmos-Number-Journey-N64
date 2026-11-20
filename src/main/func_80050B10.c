/* Song start: relocates the song header's offsets once, then allocates a master channel and one channel per track */
typedef struct {
    u8 pad0[4];
    s32 unk_4;
    u8 pad8[4];
    s32 *unk_C[7];
    s32 unk_28;
} Song;
typedef struct {
    u32 unk_0;
    s32 unk_4;
    u8 pad8[0x34 - 0x8];
    s32 unk_34;
    s32 unk_38;
    u8 pad3C[0x44 - 0x3C];
    s32 unk_44;
    u8 pad48[0x74 - 0x48];
    Song *unk_74;
    u8 pad78[0x80 - 0x78];
    s32 unk_80;
    u8 pad84[4];
    s32 unk_88;
    s32 unk_8C;
    u8 pad90[0xD2 - 0x90];
    u8 unk_D2;
    u8 padD3[0x13C - 0xD3];
} Chan;
extern s32 D_800A0560;
extern s32 D_800A0568;
extern Chan *D_800A054C;
s32 func_800506B8(Song *s, s32 track);
void func_8005057C(Chan *c);

static inline void func_reloc(s32 *p, s32 n, s32 b) {
    s32 i;
    s32 base = b;
    for (i = 0; i < n; i++) {
        if (p[i] != 0) {
            p[i] += base;
        }
    }
}


s32 func_80050B10(Song *s) {
    s32 count = s->unk_4;
    s32 i;
    s32 id;
    Chan *c;
    s32 base = (s32)s;

    if (s->unk_28 == 0) {
        s->unk_28 = 1;
        func_reloc((s32 *)s->unk_C, 7, (s32)s);
        func_reloc(s->unk_C[0], count, base);
        func_reloc(s->unk_C[1], count, base);
        func_reloc(s->unk_C[2], count, base);
    }
    id = D_800A0560++;
    c = &D_800A054C[func_800506B8(s, -1)];
    func_8005057C(c);
    c->unk_D2 = 1;
    c->unk_74 = s;
    c->unk_0 |= 3;
    c->unk_4 = c->unk_80 = (s32)s->unk_C[6];
    c->unk_44 = id;
    for (i = 0; i < count; i++) {
        if (s->unk_C[0][i] != 0) {
            c = &D_800A054C[func_800506B8(s, i)];
            func_8005057C(c);
            c->unk_D2 = 1;
            c->unk_74 = s;
            c->unk_0 |= 1;
            c->unk_38 = c->unk_8C = s->unk_C[1][i];
            c->unk_34 = c->unk_88 = s->unk_C[2][i];
            c->unk_4 = c->unk_80 = s->unk_C[0][i];
            c->unk_44 = id;
        }
    }
    D_800A0568 = 0;
    return id;
}
