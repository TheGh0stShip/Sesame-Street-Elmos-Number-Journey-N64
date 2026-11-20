struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_FC_f32 { u8 pad[0xFC]; f32 v; };

void f(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    s32 sp4;

    if (arg0 == 0) {
        return;
    }
    sp0 = arg0;
    sp4 = *(s32 *)sp0;
    ((struct gd_8_s32 *)sp0)->v = arg1;
    ((struct gd_8_s32 *)sp0)->v = arg2;
    if ((arg2 >= 0) || (((arg1 < arg2) && (arg2 < 0)) || (((-arg2) < arg1) && (arg2 < 0)))) {
        ((struct gd_C_s32 *)sp0)->v = 0;
        ((struct gd_FC_f32 *)sp4)->v = ((struct gd_1C_f32 *)sp4)->v;
    }
}
