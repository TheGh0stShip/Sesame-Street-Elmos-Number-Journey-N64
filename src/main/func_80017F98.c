struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_30_f32 { u8 pad[0x30]; f32 v; };
struct gd_34_f32 { u8 pad[0x34]; f32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_44_s32 { u8 pad[0x44]; s32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_60_s32 { u8 pad[0x60]; s32 v; };
struct gd_64_s32 { u8 pad[0x64]; s32 v; };
struct gd_68_s32 { u8 pad[0x68]; s32 v; };
struct gd_70_s32 { u8 pad[0x70]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
extern s32 D_800717E0;
extern struct n64dk_e8 *D_8013EC7C;
s32 func_8001E74C();
s32 func_80056F00();
s32 func_80057180();
s32 func_80057D90();

void f(s32 arg0) {
    s32 sp18[16];
    s32 sp58[16];
    s32 sp98[16];
    s32 spD8;

    (*(f32 *)(arg0 + 84)) = *(f32 *)(arg0 + 48);
    ((struct gd_4_f32 *)(arg0 + 84))->v = ((struct gd_4_f32 *)(arg0 + 48))->v;
    ((struct gd_8_f32 *)(arg0 + 84))->v = ((struct gd_8_f32 *)(arg0 + 48))->v;
    ((struct gd_3C_s32 *)arg0)->v = 0;
    ((struct gd_40_f32 *)arg0)->v = 1.0f;
    ((struct gd_44_s32 *)arg0)->v = 0;
    ((struct gd_48_f32 *)arg0)->v = (((struct gd_34_f32 *)arg0)->v * ((struct gd_44_f32 *)arg0)->v) - (((struct gd_38_f32 *)arg0)->v * ((struct gd_40_f32 *)arg0)->v);
    ((struct gd_4C_f32 *)arg0)->v = (((struct gd_38_f32 *)arg0)->v * ((struct gd_3C_f32 *)arg0)->v) - (((struct gd_30_f32 *)arg0)->v * ((struct gd_44_f32 *)arg0)->v);
    ((struct gd_50_f32 *)arg0)->v = (((struct gd_30_f32 *)arg0)->v * ((struct gd_40_f32 *)arg0)->v) - (((struct gd_34_f32 *)arg0)->v * ((struct gd_3C_f32 *)arg0)->v);
    func_8001E74C(((s32)&sp18), (arg0 + 36), (arg0 + 84), (arg0 + 60), (arg0 + 72));
    func_80057D90(((s32)&sp58), ((struct gd_60_s32 *)arg0)->v, ((struct gd_64_s32 *)arg0)->v, ((struct gd_68_s32 *)arg0)->v);
    func_80057180(((s32)&sp58), ((s32)&sp18), ((s32)&sp98));
    func_80056F00(((s32)&sp98), (((struct gd_70_s32 *)arg0)->v + (D_800717E0 * 64)));
    spD8 = (s32)D_8013EC7C++;
    (*(s32 *)spD8) = 0x1020040;
    ((struct gd_4_s32 *)spD8)->v = ((struct gd_70_s32 *)arg0)->v + (D_800717E0 * 64);
}
