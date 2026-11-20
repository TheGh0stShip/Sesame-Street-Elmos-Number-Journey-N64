struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_8_s16 { u8 pad[0x8]; s16 v; };
struct gd_8_u16 { u8 pad[0x8]; u16 v; };
struct gd_A_s16 { u8 pad[0xA]; s16 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_s32 { u8 pad[0x20]; s32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_30_s32 { u8 pad[0x30]; s32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_54_f32 { u8 pad[0x54]; f32 v; };
struct gd_58_f32 { u8 pad[0x58]; f32 v; };
struct gd_5C_f32 { u8 pad[0x5C]; f32 v; };
struct gd_6C_s32 { u8 pad[0x6C]; s32 v; };
struct gd_70_s32 { u8 pad[0x70]; s32 v; };
struct gd_74_s32 { u8 pad[0x74]; s32 v; };
extern s32 D_800181B0;
extern s32 D_80018950;
extern s32 D_80115408;
s32 func_80041A80();
s32 func_80042080();

void f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;

    if (arg0 == 0) {
        return;
    }
    sp14 = arg0;
    sp10 = func_80042080(248);
    if (sp10 == 0) {
        return;
    }
    ((struct gd_70_s32 *)sp10)->v = sp10 + 120;
    ((struct gd_10_s32 *)sp10)->v = (s32)&D_800181B0;
    ((struct gd_14_s32 *)sp10)->v = (s32)&D_80018950;
    ((struct gd_18_s32 *)sp10)->v = 0;
    ((struct gd_1C_s32 *)sp10)->v = ((struct gd_20_s32 *)sp10)->v = 0;
    ((struct gd_8_s16 *)sp10)->v = 6;
    ((struct gd_A_s16 *)sp10)->v = arg1;
    ((struct gd_C_s32 *)sp10)->v = 0x4000;
    ((struct gd_6C_s32 *)sp10)->v = sp14;
    (*(f32 *)(sp10 + 96)) = 4.0f;
    ((struct gd_4_f32 *)(sp10 + 96))->v = 4.0f;
    ((struct gd_8_f32 *)(sp10 + 96))->v = 4.0f;
    switch (((struct gd_8_u16 *)sp14)->v) {
    case 5:
        sp18 = sp14;
        (*(f32 *)(sp10 + 36)) = *(f32 *)(sp18 + 36);
        ((struct gd_4_f32 *)(sp10 + 36))->v = ((struct gd_4_f32 *)(sp18 + 36))->v;
        ((struct gd_8_f32 *)(sp10 + 36))->v = ((struct gd_8_f32 *)(sp18 + 36))->v;
        break;
    case 8:
        sp1C = sp14;
        (*(f32 *)(sp10 + 36)) = *(f32 *)(sp1C + 36);
        ((struct gd_4_f32 *)(sp10 + 36))->v = ((struct gd_4_f32 *)(sp1C + 36))->v;
        ((struct gd_8_f32 *)(sp10 + 36))->v = ((struct gd_8_f32 *)(sp1C + 36))->v;
        break;
    }
    ((struct gd_74_s32 *)sp10)->v = 0;
    ((struct gd_30_s32 *)sp10)->v = 0;
    ((struct gd_34_s32 *)sp10)->v = 0;
    ((struct gd_38_f32 *)sp10)->v = 1.0f;
    ((struct gd_3C_s32 *)sp10)->v = 0;
    ((struct gd_40_f32 *)sp10)->v = 1.0f;
    ((struct gd_44_s32 *)sp10)->v = 0;
    ((struct gd_48_f32 *)sp10)->v = (((struct gd_34_f32 *)sp10)->v * ((struct gd_44_f32 *)sp10)->v) - (((struct gd_38_f32 *)sp10)->v * ((struct gd_40_f32 *)sp10)->v);
    ((struct gd_4C_f32 *)sp10)->v = (((struct gd_38_f32 *)sp10)->v * ((struct gd_3C_f32 *)sp10)->v) - (((struct gd_30_f32 *)sp10)->v * ((struct gd_44_f32 *)sp10)->v);
    ((struct gd_50_f32 *)sp10)->v = (((struct gd_30_f32 *)sp10)->v * ((struct gd_40_f32 *)sp10)->v) - (((struct gd_34_f32 *)sp10)->v * ((struct gd_3C_f32 *)sp10)->v);
    ((struct gd_54_f32 *)sp10)->v = (((struct gd_40_f32 *)sp10)->v * ((struct gd_50_f32 *)sp10)->v) - (((struct gd_44_f32 *)sp10)->v * ((struct gd_4C_f32 *)sp10)->v);
    ((struct gd_58_f32 *)sp10)->v = (((struct gd_44_f32 *)sp10)->v * ((struct gd_48_f32 *)sp10)->v) - (((struct gd_3C_f32 *)sp10)->v * ((struct gd_50_f32 *)sp10)->v);
    ((struct gd_5C_f32 *)sp10)->v = (((struct gd_3C_f32 *)sp10)->v * ((struct gd_4C_f32 *)sp10)->v) - (((struct gd_40_f32 *)sp10)->v * ((struct gd_48_f32 *)sp10)->v);
    func_80041A80(sp10, D_80115408, 4, 0);
}
