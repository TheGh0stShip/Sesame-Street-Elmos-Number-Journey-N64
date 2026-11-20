struct gd_1E8_s32 { u8 pad[0x1E8]; s32 v; };
s32 func_80042300();

void f(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    if (((struct gd_1E8_s32 *)sp10)->v != 0) {
        func_80042300(((struct gd_1E8_s32 *)sp10)->v);
    }
    func_80042300(sp10);
}
