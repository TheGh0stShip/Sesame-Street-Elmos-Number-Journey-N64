typedef union { u8 b; f32 f; } Num;
typedef struct {
    s32 unk_0; s32 unk_4; u8 unk_8; u8 unk_9; u8 padA[2]; s32 unk_C; s32 unk_10;
} Item;
typedef struct {
    u8 pad0[0x10]; s32 unk_10; u8 pad14[0x20 - 0x14];
    s32 unk_20; u8 *unk_24; Num *unk_28; Item **unk_2C;
} Hdr;
static inline f32 SEXT8F(u8 c) { if (c & 0x80) return c - 0x100; return c; }
void func_80050880(void *, void *, s32);
void func_80058580(void);
/* relocates a loaded header in place (once): pointers, number table, items */
void func_80050350(s32 base, s32 arg1) {
    Hdr *hd = (Hdr *)base;
    s32 i;
    u8 c;
    f32 f;
    Num *n;
    Item *it;
    if (!(hd->unk_10 & 0x80000000)) {
        hd->unk_10 |= 0x80000000;
        func_80050880(&hd->unk_24, hd, 3);
        func_80050880(hd->unk_2C, hd, hd->unk_20);
        for (i = 0; i < hd->unk_20; i++) {
            n = &hd->unk_28[i];
            c = n->b;
            f = SEXT8F(c);
            n->f = f / 100.0;
            c = hd->unk_24[i] - '0';
            f = SEXT8F(c);
            n->f += f;
            n = (Num *)hd->unk_2C[i];
            if (((Item *)n)->unk_9 == 0) {
                s32 v = ((Item *)n)->unk_0;
                if ((v & 0xFF000000) != 0xFF000000) {
                    v += arg1;
                    ((Item *)n)->unk_0 = v;
                }
                hd->unk_2C[i]->unk_9 = 1;
                it = hd->unk_2C[i];
                if (it->unk_C != 0) {
                    it->unk_C += base;
                }
                it = hd->unk_2C[i];
                if (it->unk_8 == 0) {
                    it->unk_10 += base;
                }
            }
        }
        func_80058580();
    }
}
