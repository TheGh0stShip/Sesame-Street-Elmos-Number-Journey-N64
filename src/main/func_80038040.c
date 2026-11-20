struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_200_s32 { u8 pad[0x200]; s32 v; };
struct gd_226_s16 { u8 pad[0x226]; s16 v; };
s32 func_80037450();
s32 func_800374D0();

void f(s32 arg0) {
    func_800374D0(arg0);
    func_80037450(arg0, 0);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -5;
    ((struct gd_226_s16 *)arg0)->v = 8;
}
