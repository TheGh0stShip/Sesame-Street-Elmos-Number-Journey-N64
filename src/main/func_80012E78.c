struct gd_4_s32 { u8 pad[0x4]; s32 v; };
extern s32 D_800A7F74;
extern s32 D_80128548;
extern s32 D_8013C72C;

s32 f(void) {
    register s32 reg_v1;

    reg_v1 = D_80128548;
    if (reg_v1 != 0) {
        D_80128548 = *(s32 *)reg_v1;
        if (D_800A7F74 != 0) {
            ((struct gd_4_s32 *)D_800A7F74)->v = reg_v1;
        }
        ((struct gd_4_s32 *)reg_v1)->v = 0;
        (*(s32 *)reg_v1) = D_800A7F74;
        D_800A7F74 = reg_v1;
        D_8013C72C++;
        return reg_v1;
        return;
    }
    return 0;
}
