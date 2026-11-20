typedef struct { s32 unk_0; s32 unk_4; } Entry8;
extern s32 D_80075090;
extern void *D_8011551C;
extern Entry8 *D_80115404;
extern Entry8 *D_8013E7F0;
extern u8 D_8006FE5C;
extern u8 D_8006FE5D;
extern u8 D_8006FE5F;
extern Entry8 D_80074914[];
extern Entry8 D_8007491C[];
extern s32 D_80074D3C;
extern s32 D_80074DA4;
extern s32 D_80074E24;
extern s32 D_8013EEF8;
void func_80058BA0(void *, s32, s32);

/* Picks entry tables by mode and, if none set yet, a handler and starts it. */
void func_80018E08(void) {
    switch (D_80075090) {
    case 0:
        if (D_8011551C == NULL) {
            D_80115404 = &D_80074914[D_8006FE5C];
            D_8011551C = &D_80074D3C;
            func_80058BA0(&D_8013EEF8, 0, 1);
        }
        break;
    case 1:
    case 2:
        if (D_8011551C == NULL) {
            D_80115404 = &D_8007491C[D_8006FE5C];
            D_8013E7F0 = &D_8007491C[D_8006FE5D];
            if (D_8006FE5F == 0) {
                D_8011551C = &D_80074E24;
            } else {
                D_8011551C = &D_80074DA4;
            }
            func_80058BA0(&D_8013EEF8, 0, 1);
        }
        break;
    }
}
