struct gd_3C_f32 { u8 pad[0x3C]; f32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
extern s32 D_80076800;
s32 func_8001E74C();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    s32 pad0;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    s32 pad1;
    f32 sp38;
    f32 sp3C;
    f32 sp40;

    sp18 = (((struct gd_40_f32 *)D_80076800)->v * ((struct gd_50_f32 *)D_80076800)->v) - (((struct gd_44_f32 *)D_80076800)->v * ((struct gd_4C_f32 *)D_80076800)->v);
    sp1C = (((struct gd_44_f32 *)D_80076800)->v * ((struct gd_48_f32 *)D_80076800)->v) - (((struct gd_3C_f32 *)D_80076800)->v * ((struct gd_50_f32 *)D_80076800)->v);
    sp20 = (((struct gd_3C_f32 *)D_80076800)->v * ((struct gd_4C_f32 *)D_80076800)->v) - (((struct gd_40_f32 *)D_80076800)->v * ((struct gd_48_f32 *)D_80076800)->v);
    sp28 = -((struct gd_48_f32 *)D_80076800)->v;
    sp2C = -((struct gd_4C_f32 *)D_80076800)->v;
    sp30 = -((struct gd_50_f32 *)D_80076800)->v;
    sp38 = -((struct gd_3C_f32 *)D_80076800)->v;
    sp3C = -((struct gd_40_f32 *)D_80076800)->v;
    sp40 = -((struct gd_44_f32 *)D_80076800)->v;
    func_8001E74C(arg3, ((s32)&arg0), ((s32)&sp38), ((s32)&sp18), ((s32)&sp28));
}
