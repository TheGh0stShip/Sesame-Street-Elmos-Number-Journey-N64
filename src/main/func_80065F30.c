/* builds a 0x14-byte request on the stack and submits it */
typedef struct { u8 pad[4]; u8 unk_04; u8 pad2[4]; u8 unk_09; u8 pad3[2]; s32 unk_0C; } Src;
typedef struct { s32 unk_00; u8 unk_04; u8 pad; s16 unk_06; s32 unk_08; s32 unk_0C; s32 unk_10; } Req;
extern void func_80060898(Req *, s32, s32);
s32 func_80065F30(Src *s)
{
    Req r;
    r.unk_04 = s->unk_04;
    r.unk_06 = 0;
    r.unk_0C = s->unk_0C;
    r.unk_10 = (s->unk_09 == 1) ? 2 : 5;
    func_80060898(&r, 0x14, 1);
    return 0;
}
