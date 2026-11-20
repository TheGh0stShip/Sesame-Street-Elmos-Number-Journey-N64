struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
extern s32 D_80074BAC;
extern s32 D_800A7F2C;
extern s32 D_8011551C;
extern s32 D_8013EEF8;
s32 func_80037450();
s32 func_80058BA0();

void f(s32 arg0) {
    if (D_8011551C == 0) {
        D_8011551C = (s32)&D_80074BAC;
        func_80058BA0(((s32)&D_8013EEF8), 0, 1);
    }
    func_80037450(arg0, 25);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    D_800A7F2C = 0;
}
