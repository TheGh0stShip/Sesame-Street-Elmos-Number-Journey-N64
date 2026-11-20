struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
extern s32 D_80106C84;

s32 f(s32 arg0, f32 arg1) {
    f32 sp0;
    f32 sp4;

    sp0 = ((struct gd_24_f32 *)arg0)->v - ((struct gd_24_f32 *)D_80106C84)->v;
    sp4 = ((struct gd_2C_f32 *)arg0)->v - ((struct gd_2C_f32 *)D_80106C84)->v;
    sp0 = sp0 * sp0;
    sp4 = sp4 * sp4;
    sp4 = sp4 + sp0;
    if (sp4 < arg1) {
        return 1;
    }
    return 0;
}
