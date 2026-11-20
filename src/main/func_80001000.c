struct gd_2_u8 { u8 pad[0x2]; u8 v; };
struct gd_3_u8 { u8 pad[0x3]; u8 v; };
struct gd_4_u8 { u8 pad[0x4]; u8 v; };
struct n64dk_e6 { u8 b[6]; };
struct n64dk_a801078B8 { s16 m0; u16 m2; s8 m4; s8 m5; };
extern struct n64dk_a801078B8 D_80107CB8[];
extern u8 D_8006EE70;
extern s32 D_8006EE74;
extern s16 D_8007BF60;
extern s16 D_8007BF66;
extern s16 D_8007BF6C;
extern s16 D_8007BF72;
extern u8 D_800A914A;
s32 func_8005F718();

void f(void) {
    void *sp10;
    u16 sp14;

    func_8005F718(((s32)&D_8007BF60));
    if (D_800A914A != 0) {
        D_800A914A--;
        D_8007BF60 = 0;
        D_8007BF66 = 0;
        D_8007BF6C = 0;
        D_8007BF72 = 0;
    }
    for (sp14 = 0; !((u32)sp14 >= 4); sp14++) {
        if (((D_8006EE70 >> sp14) % 2) != 0) {
            sp10 = (void *)&((struct n64dk_e6 *)&D_8007BF60)[sp14];
            switch (((struct gd_4_u8 *)((s32)sp10))->v) {
            case 0:
                D_80107CB8[sp14].m2 = (*(u16 *)((s32)sp10)) ^ (*(u16 *)((sp14 * 2) + ((s32)&D_8006EE74)));
                D_80107CB8[sp14].m2 = D_80107CB8[sp14].m2 & (*(u16 *)((s32)sp10));
                D_80107CB8[sp14].m0 = *(u16 *)((s32)sp10);
                (*(s16 *)((sp14 * 2) + ((s32)&D_8006EE74))) = *(u16 *)((s32)sp10);
                D_80107CB8[sp14].m0 = *(u16 *)((s32)sp10);
                D_80107CB8[sp14].m4 = ((struct gd_2_u8 *)((s32)sp10))->v;
                D_80107CB8[sp14].m5 = ((struct gd_3_u8 *)((s32)sp10))->v;
            }
        }
    }
}
