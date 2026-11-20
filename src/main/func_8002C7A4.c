typedef struct { u8 pad0[0xC]; s32 flags; u8 pad10[0x1B4-0x10]; s32 unk_1B4; s32 pad1B8; s32 unk_1BC; u8 pad1C0[0xC]; s32 unk_1CC; } Obj;
/* sets two fields of an object, clears 0x1CC, sets flag 0x80 and clears flag 0x8 */
void func_8002C7A4(register Obj *o, s32 a, s32 b) {
    o->unk_1B4 = b;
    o->unk_1CC = 0;
    o->unk_1BC = a;
    o->flags |= 0x80;
    o->flags &= ~8;
}
