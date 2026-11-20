#include "common.h"

typedef struct { s32 unk_0; s32 unk_4; } Entry8;
extern Entry8 *D_8013E7F0;
extern Entry8 D_80074914[];
extern Entry8 D_8007491C[];

/* Picks entry tables by mode and, if none set yet, a handler and starts it. */
void func_80018E08(void) {
    switch (D_80075090) {
    case 0:
        if (D_8011551C == NULL) {
            D_80115404 = &D_80074914[D_8006FE5C];
            D_8011551C = &D_80074D3C;
            osSendMesg(&D_8013EEF8, 0, 1);
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
            osSendMesg(&D_8013EEF8, 0, 1);
        }
        break;
    }
}
