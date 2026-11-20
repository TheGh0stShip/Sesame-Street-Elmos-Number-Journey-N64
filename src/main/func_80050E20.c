/* sequence command: read a byte flag; when set, copy field 0x2C to 0x50 */
typedef struct { char pad0[0x2C]; f32 unk_2C; char pad30[0x50-0x30]; f32 unk_50; char pad54[0xB8-0x54]; u8 unk_B8; } S;
u8 *func_80050E20(S *p, u8 *s)
{
    u8 c = *s++;
    p->unk_B8 = c;
    if (c != 0)
        p->unk_50 = p->unk_2C;
    return s;
}
