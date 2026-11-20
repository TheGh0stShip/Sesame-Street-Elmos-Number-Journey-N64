typedef struct { u8 pad[8]; s32 unk_8; } Sub;
typedef struct {
    u8 pad0[0x44];
    s32 unk_44;
    u8 pad48[0x74 - 0x48];
    Sub *unk_74;
    Sub *unk_78;
    u8 pad7C[0x13C - 0x7C];
} Ent;
extern s32 D_800A0544;
extern Ent *D_800A054C;

/* Finds the entry whose unk_44 equals id and returns unk_8 of its unk_74 (or unk_78 if NULL). */
s32 func_8004EE78(s32 id) {
    s32 i;
    Sub *s;
    Ent *e;
    s32 n;

    if (id != 0) {
        i = 0;
        n = D_800A0544;
        e = D_800A054C;
        /* empty asm: an extra use of the table pseudo so global alloc ranks it above the count */
        __asm__("" :: "r"(e));
        if (i < n) {
            do {
                if (e->unk_44 == id) {
                    s = e->unk_74;
                    if (s == NULL) {
                        s = e->unk_78;
                    }
                    return s->unk_8;
                }
                e++;
            } while (++i < D_800A0544);
        }
    }
    return 0;
}
