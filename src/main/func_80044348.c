struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
s32 func_80042780();

void f(s32 arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    func_80042780(arg0, ((s32)&sp10));
    (*(f32 *)arg0) = sp10;
    ((struct gd_4_f32 *)arg0)->v = sp14;
    ((struct gd_8_f32 *)arg0)->v = sp18;
}
