struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
extern s32 D_80076800;
extern f64 D_8007A0D8;
extern f64 D_8007A0E0;
extern f64 D_8007A0E8;
extern f64 D_8007A0F0;
extern f64 D_8007A0F8;
extern f64 D_8007A100;
extern f32 D_800AD02C;
extern f32 D_800AD030;
extern f32 D_800AD034;
extern f32 D_8012FEB8;
extern f32 D_8012FEBC;
extern f32 D_8012FEC0;
f32 func_80056690(f32);

void f(void) {
    f64 sp10;

    D_8012FEB8 = *(f32 *)(D_80076800 + 48);
    D_8012FEBC = ((struct gd_4_f32 *)(D_80076800 + 48))->v;
    D_8012FEC0 = ((struct gd_8_f32 *)(D_80076800 + 48))->v;
    D_8012FEB8 = D_8012FEB8 - (*(f32 *)(D_80076800 + 36));
    D_8012FEBC = D_8012FEBC - ((struct gd_4_f32 *)(D_80076800 + 36))->v;
    D_8012FEC0 = D_8012FEC0 - ((struct gd_8_f32 *)(D_80076800 + 36))->v;
    sp10 = (f64)func_80056690((((D_8012FEB8 * D_8012FEB8) + (D_8012FEBC * D_8012FEBC)) + (D_8012FEC0 * D_8012FEC0)));
    if (!(sp10 == 0.0)) {
        D_8012FEB8 = (f32)(((f64)D_8012FEB8) * (D_8007A0D8 / sp10));
        D_8012FEBC = (f32)(((f64)D_8012FEBC) * (D_8007A0E0 / sp10));
        D_8012FEC0 = (f32)(((f64)D_8012FEC0) * (D_8007A0E8 / sp10));
    }
    D_800AD02C = D_8012FEB8;
    D_800AD030 = D_8012FEBC;
    D_800AD034 = D_8012FEC0;
    D_800AD02C = (f32)(((f64)D_800AD02C) * D_8007A0F0);
    D_800AD030 = (f32)(((f64)D_800AD030) * D_8007A0F8);
    D_800AD034 = (f32)(((f64)D_800AD034) * D_8007A100);
}
