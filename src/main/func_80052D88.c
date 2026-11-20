typedef struct {
    u8 unk_0[0x11E8];
    u16 unk_11E8;
    u8 unk_11EA[0x16];
} Entry; /* size 0x1200 */

extern Entry *D_800A07C0;
extern u16 D_800A07C4;

/* clears the first entry whose id equals the given id */
void func_80052D88(u16 id) {
    s32 i;
    Entry *e;

    if (id != 0xFFFF) {
        for (i = 0; i < D_800A07C4; i++) {
            if (D_800A07C0[i].unk_11E8 == id) {
                D_800A07C0[i].unk_11E8 = 0xFFFF;
                return;
            }
        }
    }
}
