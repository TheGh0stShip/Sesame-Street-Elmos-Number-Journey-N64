struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
extern s32 D_80074380;
extern s32 D_80074390;
extern u8 D_800A7A5D;
extern s32 D_8011551C;
extern s32 D_8013EEF8;
s32 func_80037450();
s32 func_800374D0();
s32 func_80058BA0();

void f(s32 arg0) {
    func_800374D0(arg0);
    if (D_800A7A5D == 0) {
        if (D_8011551C == 0) {
            D_8011551C = (s32)&D_80074380;
            func_80058BA0(((s32)&D_8013EEF8), 0, 1);
        }
        func_80037450(arg0, 10);
        D_800A7A5D = 1;
    } else {
        if (D_8011551C == 0) {
            D_8011551C = (s32)&D_80074390;
            func_80058BA0(((s32)&D_8013EEF8), 0, 1);
        }
        func_80037450(arg0, 12);
    }
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v | 128;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
}
