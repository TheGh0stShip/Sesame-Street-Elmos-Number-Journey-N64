typedef struct Stream Stream;
struct Stream {
    u8 pad0[0x190];
    void (*unk_190)(u8 *src, void *dst, s32 len, void *mq);
    u8 *unk_194;
    u8 *unk_198;
    u32 unk_19C;
    u32 unk_1A4_pad;
    u32 unk_1A4;
    u32 unk_1A8;
    u8 pad1AC[0x11A0 - 0x1AC];
    u16 unk_11A0;
    u16 unk_11A2;
    u32 unk_11A4;
    u32 unk_11A8;
    u8 unk_11AC[0x11C8 - 0x11AC];
    u32 unk_11C8;
    u32 unk_11CC;
    u32 unk_11D0;
    u32 unk_11D4;
    u32 unk_11D8;
    s8 unk_11DC;
    s8 unk_11DD;
    s8 unk_11DE;
};

s32 func_80058A70(void *, void *, s32);
void func_8006B800(Stream *, u8 *);
void func_80060D80(u8 *, s32);

/* streaming update: finishes a pending DMA, starts the next chunk, then decodes/clears ring-buffer blocks */
void func_8006AE74(Stream *s) {
    s32 n;
    s32 len;
    u32 off;
    u32 wpos;
    u32 size;
    u32 used;

    n = s->unk_11DE;
    if (s->unk_11DC != 0) {
        if (func_80058A70(s->unk_11AC, NULL, 0) >= 0) {
            if (s->unk_11D0 == 0) {
                s->unk_11A0 = (s->unk_1A4 >> 16) & 0xFFFF;
                s->unk_11A2 = s->unk_1A4 & 0xFFFF;
                s->unk_11A4 = s->unk_1A8 & 0xFFFFFF;
                s->unk_11C8 = 0x60;
            }
            s->unk_11D0 += 0x400;
            if (s->unk_11A4 > 0x100) {
                s->unk_11A4 -= 0x100;
            } else {
                s->unk_11A4 = 0;
            }
            s->unk_11DC = 0;
        }
    }
    if (s->unk_11DC == 0) {
        if (s->unk_11DD == 1 || s->unk_11DD == 2 || s->unk_11DD == 3) {
            off = s->unk_11D0;
            if (off - s->unk_11CC <= 0xC00) {
                if (off == 0) {
                    len = 0x400;
                } else if (s->unk_11A4 > 0x100) {
                    len = 0x400;
                } else {
                    len = s->unk_11A4 * 4;
                }
                if (len > 0) {
                    s->unk_11DC = 1;
                    s->unk_190(s->unk_194 + off, (u8 *)s + ((off & 0xFFF) >> 3) * 8 + 0x1A0, len, s->unk_11AC);
                }
            }
        }
    }
    while ((s->unk_11D8 - s->unk_11D4) < s->unk_19C - 0xA0
           && (s->unk_11DD == 1 || s->unk_11A0 > 0)
           && ((s->unk_11D0 != 0 && s->unk_11A4 == 0) || s->unk_11D0 - s->unk_11CC > 0x24)
           && n > 0) {
        func_8006B800(s, s->unk_198 + ((s->unk_11D8 % s->unk_19C) >> 2) * 8);
        s->unk_11CC = (s->unk_11C8 >> 3) & ~3;
        s->unk_11D8 += 0xA0;
        if (--s->unk_11A0 == 0) {
            s->unk_11A8 = s->unk_19C;
        }
        n--;
    }
    if ((s->unk_11DD == 3 || s->unk_11DD == 4) && s->unk_11A0 <= 0) {
        while ((s->unk_11D8 - s->unk_11D4) < s->unk_19C - 0xA0 && n > 0) {
            func_80060D80(s->unk_198 + ((s->unk_11D8 % s->unk_19C) >> 2) * 8, 0x140);
            s->unk_11D8 += 0xA0;
            n--;
        }
    }
    if (s->unk_11DD == 1 && (s->unk_11D8 - s->unk_11D4) >= s->unk_19C - 0x140) {
        s->unk_11DD = 2;
    }
}
