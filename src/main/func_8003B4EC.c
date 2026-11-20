struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1EC_s8 { u8 pad[0x1EC]; s8 v; };
struct gd_1EC_u8 { u8 pad[0x1EC]; u8 v; };
struct gd_1ED_s8 { u8 pad[0x1ED]; s8 v; };
struct gd_224_u16 { u8 pad[0x224]; u16 v; };
struct gd_226_s16 { u8 pad[0x226]; s16 v; };
extern s16 D_800A7F6A;
extern s16 D_800A9148;
extern u16 D_80107CBA;
extern u8 D_80107CF0;
extern s8 D_8012F150;
s32 func_800375A8();

void f(s32 arg0) {
    if (((struct gd_1EC_s8 *)arg0)->v != 0) {
        ((struct gd_1EC_s8 *)arg0)->v = ((struct gd_1EC_u8 *)arg0)->v + 8;
    }
    if (((struct gd_224_u16 *)arg0)->v == 264) {
        ((struct gd_226_s16 *)arg0)->v = 0;
        ((struct gd_1ED_s8 *)arg0)->v = 1;
    }
    if (D_800A9148 >= 401) {
        if (((struct gd_1EC_s8 *)arg0)->v == 0) {
            ((struct gd_1EC_s8 *)arg0)->v = 8;
        }
    }
    if (((struct gd_1ED_s8 *)arg0)->v != 0) {
        if (D_800A9148 < 170) {
            ((struct gd_226_s16 *)arg0)->v = 16;
        }
    }
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        func_800375A8(arg0, 27);
    }
    if (((D_80107CBA & 0x1000) & 0xFFFF) != 0) {
        if (D_80107CF0 != 0) {
            D_8012F150 = 1;
            D_800A7F6A = 0;
        } else {
        }
    }
}
