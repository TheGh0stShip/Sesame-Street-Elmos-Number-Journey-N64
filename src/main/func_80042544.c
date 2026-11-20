struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct n64dk_a801151C4 { s32 m0; u8 pad4[0xC]; };
extern struct n64dk_a801151C4 D_80115374[];
extern s32 D_8013C724;
s32 func_80042300();

void f(s32 arg0) {
    s32 sp10;
    s32 sp14;
    s32 sp18;

    sp18 = 0;
    while (!((u32)sp18 >= (u32)D_8013C724)) {
        sp10 = D_80115374[sp18].m0;
        while (!(sp10 == 0)) {
            sp14 = *(s32 *)sp10;
            if ((((struct gd_C_s32 *)sp10)->v & -16) == arg0) {
                func_80042300(sp10);
            }
            sp10 = sp14;
        }
        sp18++;
    }
    return;
}
