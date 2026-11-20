struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
extern s32 D_8007B350;
extern s32 D_8007B398;
extern s32 D_800A0520;
s32 func_80049920();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (((struct gd_18_s32 *)arg1)->v == 0) {
        func_80049920(((s32)&D_8007B350));
        return;
    }
    if (D_800A0520 == 0) {
        func_80049920(((s32)&D_8007B398));
        return;
    }
    ((struct gd_8_s32 *)D_800A0520)->v = arg2;
    ((struct gd_C_s32 *)D_800A0520)->v = arg3;
    ((struct gd_4_s32 *)D_800A0520)->v = arg0;
    if (((struct gd_1C_s32 *)arg1)->v == 0) {
        ((struct gd_1C_s32 *)arg1)->v = D_800A0520;
    } else {
        (*(s32 *)((struct gd_20_s32 *)arg1)->v) = D_800A0520;
    }
    ((struct gd_20_s32 *)arg1)->v = D_800A0520;
    D_800A0520 = *(s32 *)D_800A0520;
    (*(s32 *)((struct gd_20_s32 *)arg1)->v) = 0;
    return;
}
