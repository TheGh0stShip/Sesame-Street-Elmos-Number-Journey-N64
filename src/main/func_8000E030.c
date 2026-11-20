struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
extern s32 D_80072DC0;
extern s32 D_80072DC4;
extern s32 D_80072DE0;
extern s32 D_80072E08;
extern s8 D_80072E90;
extern s8 D_80072E91;
extern s8 D_80072E92;
extern s8 D_80072E93;
extern s32 D_80072E94;
extern s32 D_80072E98;
extern s32 D_80072E9C;
extern s32 D_80072EA0;
extern f64 D_80072EA8;
extern f64 D_80072EB0;
extern f64 D_8007A020;
extern f64 D_8007A028;
extern struct n64dk_e8 *D_8013EC7C;
s32 func_8005E340();

s32 f(void) {
    s32 sp10;
    s32 sp14;

    sp10 = (s32)D_8013EC7C++;
    func_8005E340(((s32)&sp10));
    D_80072DC4 = D_80072DC0;
    (*(u8 *)&D_80072E90) = 255;
    (*(u8 *)&D_80072E91) = 255;
    (*(u8 *)&D_80072E92) = 255;
    (*(u8 *)&D_80072E93) = 255;
    D_80072E94 = 0;
    D_80072E98 = 0;
    D_80072E9C = 40;
    D_80072EA0 = 1;
    D_80072EA8 = D_8007A020;
    D_80072EB0 = D_8007A028;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_80072E08;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_80072DE0;
    return sp10;
}
