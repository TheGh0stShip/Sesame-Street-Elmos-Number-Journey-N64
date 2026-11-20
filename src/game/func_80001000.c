#include "common.h"

extern struct n64dk_a801078B8 D_80107CB8[];

void func_80001000(void) {
    void *sp10;
    u16 sp14;

    osContGetReadData(((s32)&D_8007BF60));
    if ((*(u8 (*))&D_800A914A) != 0) {
        (*(u8 (*))&D_800A914A)--;
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
