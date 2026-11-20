

extern s32 D_80074340;
extern s32 D_80074350;
extern s32 D_80074360;
extern s32 D_80075090;
extern s32 D_80075098;
extern u8 D_8007509D;
extern u8 D_8007509E;
extern s32 D_80075234;
extern u16 D_80109DF6;
extern s32 D_8011551C;
extern s32 D_8012F118;
extern s16 D_8013C7C0;
extern s32 D_8013EEF8;
s32 func_80058BA0();
s32 func_80058FC0();

void f(void) {
    s8 pad0;

    if (D_8007509D == 1) {
        if (D_8011551C == 0) {
            switch (D_80075090) {
            case 0:
                D_8011551C = (s32)&D_80074340;
                break;
            case 1:
                D_8011551C = (s32)&D_80074350;
                break;
            case 2:
                D_8011551C = (s32)&D_80074360;
                break;
            default:
                D_8011551C = (s32)&D_80074340;
                break;
            }
            func_80058BA0(((s32)&D_8013EEF8), 0, 1);
        }
        D_80075098++;
        if (D_80075098 >= 2) {
            if ((D_8007509E >> 1) < (*(s32 *)((D_80075090 * 4) + ((s32)&D_80075234)))) {
                D_8007509E = D_8007509E + 2;
                D_80075098 = 0;
            }
        }
        if ((D_8007509E >> 1) == (*(s32 *)((D_80075090 * 4) + ((s32)&D_80075234)))) {
            if (D_8011551C != 0) {
                D_8013C7C0 = (u32)D_80109DF6 >> 3;
                func_80058FC0(((s32)&D_8012F118));
            }
            D_8007509D = 0;
            D_8007509E = 0;
        }
    }
}
