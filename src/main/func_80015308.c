struct gd_10_s8 { u8 pad[0x10]; s8 v; };
struct gd_11_s8 { u8 pad[0x11]; s8 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
extern s32 D_80133ED0;

s32 f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp0;

    sp0 = (s32)&D_80133ED0;
    (*(s32 *)sp0) = 0;
    ((struct gd_10_s8 *)sp0)->v = arg0;
    ((struct gd_11_s8 *)sp0)->v = arg1;
    ((struct gd_14_s32 *)sp0)->v = arg2;
    ((struct gd_18_s32 *)sp0)->v = arg3;
    return sp0;
}
