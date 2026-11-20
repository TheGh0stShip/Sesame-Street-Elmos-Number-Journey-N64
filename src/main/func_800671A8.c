/* fills a 0x50-byte block from a device and writes it back with byte 4 replaced */
typedef struct { u8 pad0[4]; u8 unk_4; u8 pad5[3]; u8 unk_8; u8 unk_9; u8 padA[2]; s32 unk_C; } S;
typedef struct { u8 pad0[4]; u8 unk_4; u8 pad5; u16 unk_6; u8 rest[0x48]; } Block;
s32 func_80066FD0(u8 a, s32 b, Block *out);
void func_80060898(Block *b, s32 len, s32 c);
s32 func_800671A8(S *s) {
    Block blk;
    if (func_80066FD0(s->unk_9, s->unk_C, &blk) != 0) {
        return -2;
    }
    blk.unk_4 = s->unk_4; blk.unk_6 = 0;
    func_80060898(&blk, 0x4C, 1);
    return 0;
}
