struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct n64dk_f20_s32 { u8 pad[0x20]; s32 m[1]; };
s32 func_80034804();

void f(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 sp14;

    sp14 = arg1;
    sp10 = ((struct gd_4_s32 *)sp14)->v;
    if ((sp10 <= 0) || (sp10 >= 31)) {
        sp10 = 1;
    }
    if ((*(u8 *)(arg0 + sp10)) != 0) {
        func_80034804(arg1, (*(u8 *)(arg0 + sp10)), ((struct n64dk_f20_s32 *)arg0)->m[sp10]);
    }
}
