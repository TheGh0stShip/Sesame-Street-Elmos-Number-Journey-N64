struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
extern s32 D_800218EC;
extern s32 D_800219DC;

void f(s32 arg0) {
    s8 pad0;

    ((struct gd_8_s16 *)arg0)->v = 2;
    ((struct gd_A_s16 *)arg0)->v = 0;
    ((struct gd_C_s32 *)arg0)->v = 0;
    ((struct gd_10_s32 *)arg0)->v = (s32)&D_800218EC;
    ((struct gd_14_s32 *)arg0)->v = (s32)&D_800219DC;
    ((struct gd_18_s32 *)arg0)->v = 0;
    ((struct gd_1C_s32 *)arg0)->v = 0;
    ((struct gd_20_s32 *)arg0)->v = 0;
    ((struct gd_24_s32 *)arg0)->v = 0;
}
