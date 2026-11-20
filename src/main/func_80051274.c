/* sequence player: push the current loop/return state onto the channel's stack */
typedef struct {
    u8 pad0[0x34];
    s32 unk_34;
    s32 unk_38;
    u8 pad3c[0x70 - 0x3C];
    f32 unk_70;
    u8 pad74[0xA2 - 0x74];
    u16 unk_A2;
    u16 unk_A4;
    u8 padA6[0xBC - 0xA6];
    u8 unk_BC;
    u8 padBD[0xDB - 0xBD];
    u8 depth;
    u8 padDC[4];
    u8 *stkPtr[4];
    s32 stk38[4];
    s32 stk34[4];
    u16 stkA2[4];
    u16 stkA4[4];
    u8 stkCount[4];
    u8 stkBC[4];
    f32 stk70[4];
} Chan;
u8 *func_80051274(Chan *c, u8 *p)
{
    u8 d = c->depth;
    c->stkCount[d] = *p++;
    c->stkPtr[d] = p;
    c->stk38[d] = c->unk_38;
    c->stk34[d] = c->unk_34;
    c->stkBC[d] = c->unk_BC;
    c->stk70[d] = c->unk_70;
    c->stkA2[d] = c->unk_A2;
    c->stkA4[d] = c->unk_A4;
    c->depth++;
    return p;
}
