/* on first call, picks the entry pair for the current mode and starts a sequence */
typedef struct { s32 unk_00; s32 unk_04; } Entry;
typedef struct { u8 unk_00; u8 unk_01; u8 unk_02; u8 unk_03; } Sel;
extern Sel D_8006FE5C;
extern Entry D_80074914[];
extern Entry D_8007491C[];
extern s32 D_80074D1C;
extern s32 D_80074D6C;
extern s32 D_80074DEC;
extern s32 D_80075090;
extern Entry *D_80115404;
extern s32 *D_8011551C;
extern Entry *D_8013E7F0;
extern s32 D_8013EEF8;
void func_80058BA0(void *, s32, s32);

void func_80018CB0(void) {
    switch (D_80075090) {
    case 0:
        if (D_8011551C == NULL) {
            D_80115404 = &D_80074914[D_8006FE5C.unk_00];
            D_8011551C = &D_80074D1C;
            func_80058BA0(&D_8013EEF8, 0, 1);
        }
        break;
    case 1:
    case 2:
        if (D_8011551C == NULL) {
            D_80115404 = &D_8007491C[D_8006FE5C.unk_00];
            D_8013E7F0 = &D_8007491C[D_8006FE5C.unk_01];
            if (D_8006FE5C.unk_03 == 0) {
                D_8011551C = &D_80074DEC;
            } else {
                D_8011551C = &D_80074D6C;
            }
            func_80058BA0(&D_8013EEF8, 0, 1);
        }
        break;
    }
}
