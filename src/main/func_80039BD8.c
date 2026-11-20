struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
extern s32 D_800A7F2C;
extern s32 D_80109D4C;
s32 func_80037450();

void f(s32 arg0) {
    func_80037450(arg0, 2);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v | 128;
    if (D_80109D4C == 0) {
        ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    }
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    D_800A7F2C = 0;
}
