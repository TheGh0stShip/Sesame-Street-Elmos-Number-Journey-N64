struct n64dk_a8006DD18 { s32 m0; u8 pad4[0x4]; };
extern struct n64dk_a8006DD18 D_8006F688[];
extern s32 D_8006F6C8;
extern s32 D_8006F6CC;

void f(void) {
    s32 sp0;

    D_8006F6C8 = 0;
    D_8006F6CC = 0;
    sp0 = 0;
    while (!(sp0 >= 8)) {
        D_8006F688[sp0].m0 = 0;
        sp0++;
    }
}
