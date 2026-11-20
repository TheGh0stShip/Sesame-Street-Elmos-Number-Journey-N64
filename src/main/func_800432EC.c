struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };

void f(s32 arg0) {
    s32 sp0;
    f32 sp4;

    sp0 = ((struct gd_3C_s32 *)arg0)->v;
    sp4 = ((struct gd_38_f32 *)arg0)->v;
    if (sp4 > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (sp4 * ((struct gd_1C_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (sp4 * ((struct gd_20_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (sp4 * ((struct gd_24_f32 *)arg0)->v);
    }
    sp4 = (((struct gd_1C_f32 *)sp0)->v + (*(f32 *)arg0)) - ((((*(f32 *)(sp0 + 16)) * (*(f32 *)(arg0 + 16))) + (((struct gd_4_f32 *)(sp0 + 16))->v * ((struct gd_4_f32 *)(arg0 + 16))->v)) + (((struct gd_8_f32 *)(sp0 + 16))->v * ((struct gd_8_f32 *)(arg0 + 16))->v));
    ((struct gd_10_f32 *)arg0)->v = ((struct gd_10_f32 *)arg0)->v + (sp4 * ((struct gd_10_f32 *)sp0)->v);
    ((struct gd_14_f32 *)arg0)->v = ((struct gd_14_f32 *)arg0)->v + (sp4 * ((struct gd_14_f32 *)sp0)->v);
    ((struct gd_18_f32 *)arg0)->v = ((struct gd_18_f32 *)arg0)->v + (sp4 * ((struct gd_18_f32 *)sp0)->v);
}
