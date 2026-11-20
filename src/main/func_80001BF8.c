struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
extern struct n64dk_e8 *D_8013EC7C;

void f(s32 arg0) {
    s32 sp0;

    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0xE9000000;
    ((struct gd_4_s32 *)sp0)->v = 0;
    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0xB8000000;
    ((struct gd_4_s32 *)sp0)->v = 0;
}
