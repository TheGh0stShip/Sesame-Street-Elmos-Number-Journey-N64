/* sequence player: read three bytes, derive two float rates */
typedef struct {
    u8 pad0[0x20];
    f32 unk_20;
    u8 pad24[0xB6 - 0x24];
    u8 unk_B6;
    u8 padB7[0xD5 - 0xB7];
    u8 unk_D5;
    u8 padD6[0xDC - 0xD6];
    f32 unk_DC;
} Chan;
u8 *func_80051090(Chan *c, u8 *p)
{
    c->unk_B6 = *p++;
    c->unk_D5 = *p++;
    c->unk_20 = (f32)*p++ / 50.0;
    c->unk_DC = 6.2831852 / (f32)c->unk_D5;
    return p;
}
