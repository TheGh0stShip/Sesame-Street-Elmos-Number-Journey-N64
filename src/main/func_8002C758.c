typedef struct { u8 p0[0xC]; s32 unk_C; u8 p1[0x1B4-0x10]; s32 unk_1B4; s32 unk_1B8; s32 unk_1BC; u8 p2[0x1CC-0x1C0]; s32 unk_1CC; } Obj;
/* Sets fields 0x1B4/0x1BC, clears 0x1CC and flag 0x80. */
void func_8002C758(register Obj *o, s32 a, s32 b) {
    o->unk_1B4 = b;
    o->unk_1CC = 0;
    o->unk_1BC = a;
    o->unk_C &= ~0x80;
}
