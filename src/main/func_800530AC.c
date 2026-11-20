typedef struct { u32 w0; u32 w1; } Acmd;
typedef struct {
    u8 unk_00[0x11D4];
    u32 pos;
    u8 unk_11D8[5];
    u8 state;
    u8 unk_11DE[2];
    u8 *buf;
    void *rsState;
    u16 id;
    u8 flags;
    u8 unk_11EB;
    f32 frac;
    f32 pitch;
    u32 pending;
    u16 volL;
    u16 volR;
    u8 unk_11FC[4];
} Stream;
extern Stream *D_800A07C0;
extern u16 D_800A07C4;
void func_8006B1EC(Stream *, u32);
u32 func_80059460(void *);

#define K0_TO_PHYS(x) ((u32)(x) & 0x7FFFFFFF)
#define _SHIFTL(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))
#define aClearBuffer(pkt, d, c) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(2, 24, 8) | _SHIFTL(d, 0, 24); _a->w1 = (u32)(c); }
#define aLoadBuffer(pkt, c, d, s) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(4, 24, 8) | (((c) & 0xFF8) << 12) | ((d) & 0xFF8); _a->w1 = (u32)(s); }
#define aSaveBuffer(pkt, c, d, s) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(6, 24, 8) | _SHIFTL(c, 12, 12) | _SHIFTL(d, 0, 12); _a->w1 = (u32)(s); }
#define aResample(pkt, f, p, i, s) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(5, 24, 8) | _SHIFTL(s, 0, 24); _a->w1 = _SHIFTL(f, 30, 2) | _SHIFTL(p, 14, 16) | _SHIFTL(i, 2, 12); }
#define aMix(pkt, g, i, o) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(0xC, 24, 8) | _SHIFTL(g, 0, 16); _a->w1 = _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16); }
#define aInterleave(pkt) { Acmd *_a = (Acmd *)pkt; _a->w0 = _SHIFTL(0xD, 24, 8); }

/* build audio commands mixing all active streams into the output buffer */
Acmd *func_800530AC(Acmd *cmdList, s32 *cmdLen, s32 *outBuf, s32 outLen) {
    s32 left = outLen;
    s32 *out = outBuf;
    Acmd *ptr = cmdList;
    s32 i;
    s32 n;
    s32 nSamp;
    u32 pos;
    s32 end;
    u32 addr;
    s32 align;
    s32 len;
    u32 pitch;
    f32 f;
    Stream *st;
    s32 dmem;
    s32 mask;
    u32 hi;

    mask = ~7;
    hi = 0x04000000;
    nSamp = 0;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].id != 0xFFFF && D_800A07C0[i].state == 3) {
            nSamp++;
            if (D_800A07C0[i].pending != 0) {
                func_8006B1EC(&D_800A07C0[i], D_800A07C0[i].pending);
                D_800A07C0[i].pending = 0;
            }
        }
    }
    if (nSamp == 0) {
        return cmdList;
    }
    while (left > 0) {
        n = left;
        if (n > 184) n = 184;
        aClearBuffer(ptr++, 0x4E0, 0x2E0);
        for (i = 0; i < D_800A07C4; i++) {
            u32 mc = 0x0C000000;
            if (D_800A07C0[i].id == 0xFFFF || D_800A07C0[i].state != 3) continue;
            st = &D_800A07C0[i];
            f = st->frac + st->pitch * n;
            nSamp = f;
            st->frac = f - nSamp;
            pos = (st->pos + st->pending) % 2560;
            end = pos + nSamp;
            if (end > 2560) {
                addr = (u32)(D_800A07C0[i].buf + pos * 2) - 0x80000000;
                align = addr & 7;
                addr -= align;
                len = (2560 - pos) * 2 + align + 7;
                aLoadBuffer(ptr++, len, 0x170, addr);
                dmem = (len & mask) + 0x170;
                addr = (u32)D_800A07C0[i].buf - 0x80000000;
                { Acmd *_a = ptr++; _a->w0 = (((end - 2560) * 2 + 15) & 0xFF8) << 12 | ((dmem & 0xFF8) | hi); _a->w1 = addr; }
            } else {
                addr = (u32)(D_800A07C0[i].buf + pos * 2) - 0x80000000;
                align = addr & 7;
                addr -= align;
                aLoadBuffer(ptr++, (nSamp * 2 + align + 15) & ~7, 0x170, addr);
            }
            pitch = D_800A07C0[i].pitch * 32768.0f;
            aResample(ptr++, D_800A07C0[i].flags, pitch, align | 0x170, func_80059460(D_800A07C0[i].rsState));
            D_800A07C0[i].flags = 0;
            { Acmd *_a = ptr++; _a->w0 = D_800A07C0[i].volL | mc; _a->w1 = 0x4E0; }
            { Acmd *_a = ptr++; _a->w0 = D_800A07C0[i].volR | mc; _a->w1 = 0x650; }
            D_800A07C0[i].pending += nSamp;
        }
        aInterleave(ptr++);
        aLoadBuffer(ptr++, 0x2E0, 0x4E0, out);
        aMix(ptr++, 0x7FFF, 0x4E0, 0);
        aMix(ptr++, 0x7FFF, 0x650, 0x170);
        aSaveBuffer(ptr++, 0x2E0, 0, out);
        out += n;
        left -= n;
    }
    *cmdLen += ptr - cmdList;
    return ptr;
}
