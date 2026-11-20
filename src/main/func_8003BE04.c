struct gd_C_s32 { u8 pad[0xC]; s32 v; };
extern u8 D_8006FE5E;
extern s16 D_800A7F6A;
extern u16 D_80107CBA;
extern u8 D_80107CF0;
extern s8 D_8012F150;
s32 func_800375A8();

void f(s32 arg0) {
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        D_8006FE5E--;
        func_800375A8(arg0, 0);
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
