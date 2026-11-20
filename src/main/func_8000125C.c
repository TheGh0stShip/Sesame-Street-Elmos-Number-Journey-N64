struct n64dk_e6 { u8 b[6]; };
struct n64dk_a801078BA { u16 m0; u8 pad2[0x4]; };
extern struct n64dk_a801078BA D_80107CBA[];
extern u8 D_8006EE70;
extern s32 D_8006EE74;
extern s32 D_8007BF60;

void f(void) {
    s32 sp0;
    void *sp4;

    for (sp0 = 0; !(sp0 >= 4); sp0++) {
        if (((D_8006EE70 >> sp0) % 2) != 0) {
            sp4 = (void *)&((struct n64dk_e6 *)&D_8007BF60)[sp0];
            D_80107CBA[sp0].m0 = (*(u16 *)((s32)sp4)) ^ (*(u16 *)((sp0 * 2) + ((s32)&D_8006EE74)));
            D_80107CBA[sp0].m0 = D_80107CBA[sp0].m0 & (*(u16 *)((s32)sp4));
            (*(s16 *)((sp0 * 2) + ((s32)&D_8006EE74))) = *(u16 *)((s32)sp4);
        }
    }
}
