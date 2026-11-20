struct n64dk_r16 { s32 w[4]; };
struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_4_s32 { u8 pad[0x4]; s32 v; };
struct gd_6_s16 { u8 pad[0x6]; s16 v; };
struct gd_8_s32 { u8 pad[0x8]; s32 v; };
struct gd_28_s16 { u8 pad[0x28]; s16 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
s32 func_8000DFA0();

void f(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;

    ((struct gd_4_s16 *)arg0)->v = arg3 * 8;
    ((struct gd_6_s16 *)arg0)->v = arg4 * 16;
    sp24 = ((struct gd_34_s32 *)arg0)->v;
    sp10 = 0;
    sp14 = 0;
    sp1C = 0;
    while (!(sp1C >= arg4)) {
        sp18 = 0;
        while (!(sp18 >= arg3)) {
            if ((*(u8 *)(arg1 + sp14)) == 0) {
                (*(struct n64dk_r16 *)((sp10 * 16) + sp24)) = (*(struct n64dk_r16 *)((struct gd_4_s32 *)arg2)->v);
                (*(s16 *)((sp10 * 16) + sp24)) = -1;
                ((struct gd_28_s16 *)arg0)->v = sp10;
                return;
            }
            if ((*(u8 *)(arg1 + sp14)) == 10) {
                (*(struct n64dk_r16 *)((sp10 * 16) + sp24)) = (*(struct n64dk_r16 *)((struct gd_4_s32 *)arg2)->v);
                ((struct gd_8_s32 *)((sp10 * 16) + sp24))->v = 0;
                sp14--;
            } else {
                sp20 = func_8000DFA0((*(s32 *)arg2), (*(u8 *)(arg1 + sp14)));
                if (sp20 != 0) {
                    (*(struct n64dk_r16 *)((sp10 * 16) + sp24)) = (*(struct n64dk_r16 *)(((struct gd_4_s32 *)arg2)->v + ((sp20 - (*(s32 *)arg2)) * 16)));
                } else {
                    (*(struct n64dk_r16 *)((sp10 * 16) + sp24)) = (*(struct n64dk_r16 *)((struct gd_4_s32 *)arg2)->v);
                    ((struct gd_8_s32 *)((sp10 * 16) + sp24))->v = 0;
                }
            }
            sp18++;
            sp10++;
            sp14++;
        }
        if ((*(u8 *)(arg1 + sp14)) == 10) {
            sp14++;
        }
        sp1C++;
    }
    ((struct gd_28_s16 *)arg0)->v = sp10;
    return;
}
