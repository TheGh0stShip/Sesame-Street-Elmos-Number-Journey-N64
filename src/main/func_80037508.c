struct gd_1C8_s32 { u8 pad[0x1C8]; s32 v; };
struct gd_1CC_s32 { u8 pad[0x1CC]; s32 v; };
extern s32 D_80076C3C;
extern s32 D_8007AFB0;
extern s32 D_8007AFB4;
s32 func_80059540();

void f(s32 arg0) {
    if ((u32)((struct gd_1C8_s32 *)arg0)->v < 39) {
        goto L11;
    }
    func_80059540(((s32)&D_8007AFB0), ((s32)&D_8007AFB4), 524);
L11: ;
    ((s32 (*)())(*(s32 *)((((struct gd_1C8_s32 *)arg0)->v * 4) + ((s32)&D_80076C3C))))(arg0);
    ((struct gd_1CC_s32 *)arg0)->v++;
}
