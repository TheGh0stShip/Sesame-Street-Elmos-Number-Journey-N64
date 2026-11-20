struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_24_s32 { u8 pad[0x24]; s32 v; };
struct n64dk_a801078BA { u16 m0; u8 pad2[0x4]; };
extern struct n64dk_a801078BA D_80107CBA[];
extern s32 D_80109D4C;

void f(s32 arg0) {
    s32 sp0;
    s32 pad0;
    s32 pad1[16];
    s32 pad2;
    s32 sp4C;
    s32 pad3[8];
    s32 pad4;
    s32 sp74;
    s32 pad5[14];
    s8 pad6;

    sp0 = arg0;
    sp74 = 0;
    if (D_80109D4C >= 5) {
        if (D_80109D4C >= 8) {
            ((struct gd_C_s32 *)sp0)->v = ((struct gd_C_s32 *)sp0)->v | 1;
        }
        return;
    }
    for (sp4C = 0; !((u32)sp4C >= 4); sp4C++) {
        if ((D_80107CBA[sp4C].m0 & 32) != 0) {
            ((struct gd_24_s32 *)sp0)->v = ((struct gd_24_s32 *)sp0)->v ^ 1;
            sp4C = 4;
        }
    }
}
