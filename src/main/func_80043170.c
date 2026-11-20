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
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
s32 func_80044348();

void f(s32 arg0) {
    f32 sp10;
    s32 pad0;
    f32 sp18;
    f32 sp1C;
    f32 sp20;

    sp10 = ((struct gd_38_f32 *)arg0)->v;
    if (sp10 > 0.0f) {
        ((struct gd_4_f32 *)arg0)->v = ((struct gd_4_f32 *)arg0)->v + (sp10 * ((struct gd_1C_f32 *)arg0)->v);
        ((struct gd_8_f32 *)arg0)->v = ((struct gd_8_f32 *)arg0)->v + (sp10 * ((struct gd_20_f32 *)arg0)->v);
        ((struct gd_C_f32 *)arg0)->v = ((struct gd_C_f32 *)arg0)->v + (sp10 * ((struct gd_24_f32 *)arg0)->v);
    }
    sp18 = ((struct gd_10_f32 *)arg0)->v - ((struct gd_40_f32 *)arg0)->v;
    sp1C = ((struct gd_14_f32 *)arg0)->v - ((struct gd_44_f32 *)arg0)->v;
    sp20 = ((struct gd_18_f32 *)arg0)->v - ((struct gd_48_f32 *)arg0)->v;
    func_80044348(((s32)&sp18));
    ((struct gd_10_f32 *)arg0)->v = ((struct gd_40_f32 *)arg0)->v + (sp18 * (*(f32 *)arg0));
    ((struct gd_14_f32 *)arg0)->v = ((struct gd_44_f32 *)arg0)->v + (sp1C * (*(f32 *)arg0));
    ((struct gd_18_f32 *)arg0)->v = ((struct gd_48_f32 *)arg0)->v + (sp20 * (*(f32 *)arg0));
}
