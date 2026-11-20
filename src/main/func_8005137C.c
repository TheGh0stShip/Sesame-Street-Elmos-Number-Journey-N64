/* sequence player: read three command bytes into channel fields */
typedef struct { u8 pad[0xCE]; u8 unk_CE; u8 unk_CF; u8 pad2[0xD8-0xD0]; u8 unk_D8; } Chan;
u8 *func_8005137C(Chan *c, u8 *p)
{
    c->unk_D8 = *p++;
    c->unk_CE = *p++;
    c->unk_CF = *p++;
    return p;
}
