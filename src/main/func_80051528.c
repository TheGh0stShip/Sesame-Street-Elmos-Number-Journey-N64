/* sequence player: read a mapped byte plus an offset byte into a channel field */
typedef struct { u8 pad[0xBC]; u8 unk_BC; } Chan;
extern u8 func_800504E8(u8);
u8 *func_80051528(Chan *c, u8 *p)
{
    u8 v = func_800504E8(*p++);
    c->unk_BC = v;
    c->unk_BC = v + *p++;
    return p;
}
