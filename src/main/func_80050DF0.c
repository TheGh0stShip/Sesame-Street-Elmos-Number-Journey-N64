/* sequence player: read a 7/15-bit variable-length value into a channel field */
typedef struct { u8 pad[0xAE]; u16 unk_AE; } Chan;
u8 *func_80050DF0(Chan *c, u8 *p)
{
    u32 v = *p++;
    if (v & 0x80) {
        v = *p++ | ((v & 0x7F) << 8);
    }
    c->unk_AE = v;
    return p;
}
