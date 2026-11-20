struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_204_s32 { u8 pad[0x204]; s32 v; };
extern s16 D_800A7F6A;
extern u16 D_80107CBA;
extern u8 D_80107CF0;
extern s8 D_8012F150;
s32 func_800375A8();

void f(s32 arg0) {
    s32 pad0[2];
    s8 pad1;

    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        if (((struct gd_204_s32 *)arg0)->v != 0) {
            func_800375A8(arg0, 1);
        } else {
            func_800375A8(arg0, 0);
        }
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
