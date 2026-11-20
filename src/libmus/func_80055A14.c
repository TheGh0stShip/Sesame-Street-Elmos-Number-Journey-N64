#include "common.h"
/* if active, shuts down via func_80055C80 and clears the two state words */

extern s32 D_80077BC4;
void n_alClose(void) {
    if (D_80077BC0 != 0) {
        func_80055C80();
        D_80077BC0 = 0;
        D_80077BC4 = 0;
    }
}
