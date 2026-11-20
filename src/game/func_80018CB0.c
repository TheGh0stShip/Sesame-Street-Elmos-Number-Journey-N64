#include "common.h"

/* on first call, picks the entry pair for the current mode and starts a sequence */
typedef struct { s32 unk_00; s32 unk_04; } Entry;
extern Entry D_80074914[];
extern Entry D_8007491C[];
extern Entry *D_8013E7F0;

void func_80018CB0(void) {
    switch (D_80075090) {
    case 0:
        if (D_8011551C == NULL) {
            D_80115404 = &D_80074914[(*(Sel (*))&D_8006FE5C).unk_00];
            D_8011551C = &D_80074D1C;
            osSendMesg(&D_8013EEF8, 0, 1);
        }
        break;
    case 1:
    case 2:
        if (D_8011551C == NULL) {
            D_80115404 = &D_8007491C[(*(Sel (*))&D_8006FE5C).unk_00];
            D_8013E7F0 = &D_8007491C[(*(Sel (*))&D_8006FE5C).unk_01];
            if ((*(Sel (*))&D_8006FE5C).unk_03 == 0) {
                D_8011551C = &D_80074DEC;
            } else {
                D_8011551C = &D_80074D6C;
            }
            osSendMesg(&D_8013EEF8, 0, 1);
        }
        break;
    }
}
