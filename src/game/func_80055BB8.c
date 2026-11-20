#include "common.h"

typedef struct Voice { struct Voice *next; s32 unk_04; Info *info; u8 unk_0C[0x7C]; s32 unk_88; } Voice;
typedef struct { s32 unk_00; Voice *unk_04; s32 unk_08; Voice *unk_0C; s32 unk_10; Voice *unk_14; } Pool;
extern Pool *D_80077BC4;
void alLink(Voice *, Voice **);

/* pick a free voice, else steal the lowest-priority idle one */
s32 func_80055BB8(Voice **out, s16 prio) {
    Voice *v;
    s32 found = 0;
    v = D_80077BC4->unk_14;
    if (v == NULL) v = D_80077BC4->unk_04;
    if (v != NULL) {
        *out = v;
        alUnlink(v);
        alLink(v, &D_80077BC4->unk_0C);
    } else {
        for (v = D_80077BC4->unk_0C; v != NULL; v = v->next) {
            if (prio >= v->info->unk_16 && v->unk_88 == 0) {
                *out = v;
                found = 1;
                prio = v->info->unk_16;
            }
        }
    }
    return found;
}
