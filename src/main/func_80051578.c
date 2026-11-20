/* sequence player: read a mapped byte plus an offset byte into a channel field */
typedef struct { u8 pad[0xBD]; u8 unk_BD; } Chan;
extern u8 func_800504E8(u8);
u8 *func_80051578(Chan *c, u8 *p)
{
    u8 v = func_800504E8(*p++);
    c->unk_BD = v;
    c->unk_BD = v + *p++;
    return p;
}
