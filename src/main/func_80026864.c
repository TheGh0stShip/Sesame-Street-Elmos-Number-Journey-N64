struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_9C_f32 { u8 pad[0x9C]; f32 v; };
struct gd_A0_f32 { u8 pad[0xA0]; f32 v; };
struct gd_A4_f32 { u8 pad[0xA4]; f32 v; };
struct gd_A8_f32 { u8 pad[0xA8]; f32 v; };
struct gd_AC_f32 { u8 pad[0xAC]; f32 v; };
struct gd_B0_f32 { u8 pad[0xB0]; f32 v; };
extern s32 D_80076800;
extern s32 D_8012F154;
s32 func_80026494();

s32 f(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    s32 sp20;
    f32 sp24;

    if ((D_8012F154 == 11) || (D_8012F154 == 19)) {
        return 1;
    }
    sp20 = D_80076800 + 84;
    func_80026494(((s32)&sp10), sp20, arg0);
    sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_9C_f32 *)D_80076800)->v;
    if (sp24 < 0.0f) {
        sp20 = D_80076800 + 96;
        func_80026494(((s32)&sp10), sp20, arg0);
        sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_A0_f32 *)D_80076800)->v;
        if (sp24 < 0.0f) {
            sp20 = D_80076800 + 132;
            func_80026494(((s32)&sp10), sp20, arg0);
            sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_AC_f32 *)D_80076800)->v;
            if (sp24 < 0.0f) {
                sp20 = D_80076800 + 144;
                func_80026494(((s32)&sp10), sp20, arg0);
                sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_B0_f32 *)D_80076800)->v;
                if (sp24 < 0.0f) {
                    sp20 = D_80076800 + 120;
                    func_80026494(((s32)&sp10), sp20, arg0);
                    sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_A8_f32 *)D_80076800)->v;
                    if (sp24 < 0.0f) {
                        sp20 = D_80076800 + 108;
                        func_80026494(((s32)&sp10), sp20, arg0);
                        sp24 = (((sp10 * (*(f32 *)sp20)) + (sp14 * ((struct gd_4_f32 *)sp20)->v)) + (sp18 * ((struct gd_8_f32 *)sp20)->v)) - ((struct gd_A4_f32 *)D_80076800)->v;
                        if (sp24 < 0.0f) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
