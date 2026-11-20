struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_e8 { u8 b[8]; };
extern struct n64dk_e8 *D_8013EC7C;
s32 func_8001F0CC();

void f(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;

    sp10 = 0;
    while (!((*(s32 *)((sp10 * 4) + arg2)) == 0)) {
        if (func_8001F0CC(arg0, arg1) != 0) {
            sp14 = (s32)D_8013EC7C++;
            (*(s32 *)sp14) = 0x6000000;
            ((struct gd_4_s32 *)sp14)->v = *(s32 *)((sp10 * 4) + arg2);
        }
        arg1 = arg1 + 24;
        sp10++;
    }
}
