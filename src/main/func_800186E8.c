struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
extern s32 D_80074FA0;
extern s32 D_80074FB8;
extern s32 D_80075010;
extern struct n64dk_e8 *D_8013EC7C;
extern s32 D_8024B008;
s32 func_80017F98();

void f(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = arg0;
    func_80017F98(sp10);
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_80074FA0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xFD180000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_8024B008;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xF5180000;
    ((struct gd_4_s32 *)sp14)->v = 0x7080200;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xE6000000;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xF3000000;
    ((struct gd_4_s32 *)sp14)->v = 0x73FF080;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xE7000000;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xF5181000;
    ((struct gd_4_s32 *)sp14)->v = 0x80200;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xF2000000;
    ((struct gd_4_s32 *)sp14)->v = 0x7C07C;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_80075010;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = (s32)&D_80074FB8;
}
