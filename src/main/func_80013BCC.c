struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct gd_28_s32 { u8 pad[0x28]; s32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
extern s32 D_800A7F30;
extern s32 D_8012C410;
extern s32 D_8012FED0;
extern s32 D_801341F0;

s32 f(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp0;

    sp0 = (s32)&D_800A7F30;
    ((struct gd_24_s32 *)sp0)->v = (s32)&D_8012C410;
    ((struct gd_28_s32 *)sp0)->v = (s32)&D_801341F0;
    ((struct gd_2C_s32 *)sp0)->v = ((struct gd_28_s32 *)sp0)->v + 0x8000;
    (*(s32 *)sp0) = 0;
    ((struct gd_1C_s32 *)sp0)->v = 0;
    ((struct gd_20_s32 *)sp0)->v = 0;
    ((struct gd_30_s32 *)sp0)->v = ((struct gd_34_s32 *)sp0)->v = ((struct gd_28_s32 *)sp0)->v;
    (*(s32 *)arg0) = (s32)&D_8012FED0;
    ((struct gd_4_s32 *)arg0)->v = 0;
    ((struct gd_8_s32 *)arg0)->v = ((struct gd_14_s32 *)arg0)->v = 0;
    ((struct gd_10_s32 *)arg0)->v = arg2;
    ((struct gd_C_s32 *)arg0)->v = arg1;
    ((struct gd_18_s32 *)arg0)->v = sp0;
    return 0;
}
