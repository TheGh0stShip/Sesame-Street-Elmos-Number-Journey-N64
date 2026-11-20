typedef struct { u32 w0; u32 w1; } Acmd;
typedef s32 (*DmaFn)(s32 addr, s32 len, void *state);
typedef struct {
    u8 pad0[0x14]; s32 unk_14; s32 unk_18; u8 pad1C[0x14];
    DmaFn dma; void *dmaState; u8 pad38[8]; s32 unk_40; s32 unk_44;
} Voice;
/* emit audio commands: load sample data via dma, optional state reset, decode */
Acmd *func_8005A72C(Acmd *p, Voice *v, s32 count, s32 len, u16 out, s16 in, s32 flags) {
    s32 addr;
    s32 pad;

    if (len > 0) {
        addr = v->dma(v->unk_44, len, v->dmaState);
        pad = addr & 7;
        len += pad;
        { Acmd *c = p++;
        c->w0 = 0x08000000 | (in & 0xFFFF);
        c->w1 = (len + 8 - (len & 7)) & 0xFFFF; }
        { Acmd *c = p++;
        c->w0 = 0x04000000;
        c->w1 = addr - pad; }
    } else {
        pad = 0;
    }
    if (flags & 2) {
        Acmd *c = p++;
        c->w0 = 0x0F000000;
        c->w1 = v->unk_18 & 0x1FFFFFFF;
    }
    { Acmd *c = p++;
    c->w0 = 0x08000000 | ((in + pad) & 0xFFFF);
    c->w1 = (out << 16) | ((count * 2) & 0xFFFF); }
    { Acmd *c = p++;
    c->w0 = 0x01000000 | ((flags & 0xFF) << 16);
    c->w1 = v->unk_14 & 0x1FFFFFFF; }
    v->unk_40 = 0;
    return p;
}
