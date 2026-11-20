struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_A_u16 { u8 pad[0xA]; u16 v; };
struct gd_C_s32 { u8 pad[0xC]; s32 v; };
struct gd_10_s32 { u8 pad[0x10]; s32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_f32 { u8 pad[0x2C]; f32 v; };
extern u8 D_800769F0;
extern u8 D_80076B61;
extern s32 D_80076B90;
extern s32 D_80076B94;
extern s32 D_80076B98;
extern s32 D_80076B9C;
extern f64 D_8007AF98;
extern f64 D_8007AFA0;
extern f64 D_8007AFA8;
extern u8 D_800AD03A;
extern s32 D_80106C84;
extern u16 D_80107CBA;
extern s32 D_80109D4C;
extern s32 D_80115408;
extern s32 D_8012FE68;
s32 func_800121C0();
s32 func_80035260();
s32 func_800375A8();
s32 func_80041CAC();
s32 func_8004E5A0();
f32 func_80056690(f32);

void f(s32 arg0) {
    s32 sp10;
    s32 pad0;
    s32 pad1[6];
    s32 pad2;
    s32 sp34;
    s32 pad3[2];
    s32 sp40;
    s32 sp44;
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    s32 pad4;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    s32 pad5;
    s32 sp68;
    s32 sp6C;

    sp10 = arg0;
    sp34 = 0;
    sp10 = arg0;
    sp40 = 0;
    sp44 = 0;
    switch (D_800AD03A) {
    case 0:
        D_80076B98++;
        if (D_80109D4C >= 5) {
            if (D_80109D4C >= 8) {
                ((struct gd_C_s32 *)sp10)->v = ((struct gd_C_s32 *)sp10)->v | 1;
            }
            return;
        }
        switch (((struct gd_A_u16 *)sp10)->v) {
        case 0:
            return;
        case 1:
            return;
            return;
        case 2:
            sp68 = func_80041CAC((*(s32 *)D_80115408), 5, 0xFFFF);
            if (sp68 == 0) {
                return;
            }
            sp48 = *(f32 *)(sp10 + 36);
            sp4C = ((struct gd_4_f32 *)(sp10 + 36))->v;
            sp50 = ((struct gd_8_f32 *)(sp10 + 36))->v;
            sp48 = sp48 - (*(f32 *)(sp68 + 36));
            sp4C = sp4C - ((struct gd_4_f32 *)(sp68 + 36))->v;
            sp50 = sp50 - ((struct gd_8_f32 *)(sp68 + 36))->v;
            if (((f64)func_80056690((((sp48 * sp48) + (sp4C * sp4C)) + (sp50 * sp50)))) < D_8007AF98) {
                if (((D_80107CBA & 0xC00F) & 0xFFFF) != 0) {
                    D_8012FE68 = 22;
                    ((struct gd_10_s32 *)sp10)->v = 0;
                    func_800375A8(D_80106C84, 35);
                    ((struct gd_C_s32 *)sp10)->v = ((struct gd_C_s32 *)sp10)->v | 1;
                    return;
                }
            } else {
                return;
            }
            return;
        case 3:
            switch (D_80076B61) {
            case 0:
                sp6C = func_80041CAC((*(s32 *)D_80115408), 5, 0xFFFF);
                if (sp6C == 0) {
                    return;
                }
                sp58 = *(f32 *)(sp10 + 36);
                sp5C = ((struct gd_4_f32 *)(sp10 + 36))->v;
                sp60 = ((struct gd_8_f32 *)(sp10 + 36))->v;
                sp58 = sp58 - (*(f32 *)(sp6C + 36));
                sp5C = sp5C - ((struct gd_4_f32 *)(sp6C + 36))->v;
                sp60 = sp60 - ((struct gd_8_f32 *)(sp6C + 36))->v;
                return;
                break;
            case 1:
                return;
                break;
            case 2:
                if (D_80076B9C >= 101) {
                    D_80076B61 = 0;
                    D_80076B9C = 0;
                } else {
                    D_80076B9C++;
                }
                break;
            default:
                break;
            }
            return;
            return;
        case 4:
            if (D_800769F0 == 5) {
                func_800121C0(((s32)((struct gd_24_f32 *)sp10)->v), ((s32)(((f64)((struct gd_28_f32 *)sp10)->v) + D_8007AFA0)), ((s32)((struct gd_2C_f32 *)sp10)->v));
                if (D_80076B98 >= 101) {
                    func_8004E5A0(55);
                    func_80035260(((s32)((struct gd_24_f32 *)sp10)->v), ((s32)(((f64)((struct gd_28_f32 *)sp10)->v) + D_8007AFA8)), ((s32)((struct gd_2C_f32 *)sp10)->v));
                    D_80076B94++;
                    D_80076B90++;
                    if (D_80076B90 >= 3) {
                        D_80076B90 = 0;
                    }
                    D_80076B98 = 0;
                }
                if (((D_80107CBA & 0xC00F) & 0xFFFF) != 0) {
                    D_800769F0 = 6;
                    D_80076B94 = 0;
                    D_80076B90 = 0;
                    ((struct gd_C_s32 *)sp10)->v = ((struct gd_C_s32 *)sp10)->v | 1;
                }
            }
            return;
            return;
        case 5:
            return;
            return;
        }
        return;
        return;
    }
}
