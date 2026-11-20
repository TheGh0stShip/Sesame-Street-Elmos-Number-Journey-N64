/* read a 1-2 byte index from the stream, create an object for it and retarget matching entries */
typedef struct {
    u8 pad0[0x44];
    s32 unk_44;
    s32 unk_48;
    u8 pad4C[0x78 - 0x4C];
    s32 unk_78;
    s32 unk_7C;
    u8 pad80[0x9E - 0x80];
    s16 unk_9E;
    u8 padA0[0xB0 - 0xA0];
    s16 unk_B0;
} Obj;
typedef struct {
    u8 pad0[0x44];
    s32 unk_44;
    u8 pad48[0x7C - 0x48];
    s32 unk_7C;
    u8 pad80[0x13C - 0x80];
} Entry;
extern s32 D_800A0544;
extern Entry *D_800A054C;
s32 func_80050968(s32, s32, s32, s32, s32);

u8 *func_800515D8(Obj *obj, u8 *p)
{
    s32 n;
    s32 r;
    s32 i;

    n = *p++;
    if (n >= 0x80)
        n = ((n & 0x7F) << 8) + *p++;
    obj->unk_48++;
    r = func_80050968(obj->unk_78, n, obj->unk_9E, obj->unk_B0, obj->unk_48);
    obj->unk_48--;
    if (r != 0) {
        Entry *e = D_800A054C;
        for (i = 0; i < D_800A0544; i++, e++) {
            if (e->unk_44 == r) {
                e->unk_44 = obj->unk_44;
                e->unk_7C = obj->unk_7C;
            }
        }
    }
    return p;
}
