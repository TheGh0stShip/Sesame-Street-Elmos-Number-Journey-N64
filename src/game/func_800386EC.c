#include "common.h"
#include "game_structs.h"


extern u16 D_80107CBA;

/* Per-frame actor update: input-triggered state changes, then turns toward its target. */
void func_800386EC(Actor *a) {
    register f32 dx;
    register f32 dz;
    register Actor *r;

    if (D_800B1090 == 0) {
        if (D_8006FE58 & 2) {
            if (D_8011551C == NULL) {
                D_800B1090 = 1;
                a->u.walker.unk_200 |= 1;
                a->u.walker.unk_200 |= 2;
            }
        }
    } else if (a->u.walker.frame >= 50) {
        if (D_80107CBA & 0xC00F) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(D_8012F118);
            }
            D_8012FE68 = 7;
            func_800375A8(a, 0x23);
        }
    }
    if (a->u.walker.frame == 20) {
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B1C;
            osSendMesg(D_8013EEF8, 0, 1);
        }
    }
    if (a->u.walker.frame >= 20) {
        if (a->u.walker.turning != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(D_8012F118);
            }
            func_800375A8(a, 1);
            return;
        }
        return;
    }
    dx = a->u.walker.unk_1F0.l0.unk_1F0->pos.x - a->pos.x;
    dz = a->u.walker.unk_1F0.l0.unk_1F0->pos.z - a->pos.z;
    dz = func_80069920(dz, dx);
    r = a;
    r->u.walker.goalDir.x = __cosf(dz);
    r = a;
    r->u.walker.goalDir.z = __sinf(dz);
}
