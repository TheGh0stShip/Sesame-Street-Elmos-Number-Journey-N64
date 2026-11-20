#include "common.h"
#include "game_structs.h"

#define SINF(x) ({ __cosf(x); })
#define COSF(x) ({ __sinf(x); })

extern u16 D_80107CBA;

/* Actor update: state changes on input/timer, then faces its target actor. */
void func_80038388(Actor *arg0) {
    register f32 dx;
    register f32 dz;

    if (D_800B1098 == 0) {
        if ((D_8006FE58 & 2) && D_8011551C == NULL) {
            D_800B1098 = 1;
            arg0->u.walker.unk_200 |= 1;
            arg0->u.walker.unk_200 |= 2;
        }
    } else if (arg0->u.walker.frame >= 50 && (u16)(D_80107CBA & 0xC00F)) {
        if (D_8011551C != NULL) {
            D_8013C7C0 = D_80109DF6 >> 3;
            osStopTimer(&D_8012F118);
        }
        D_8012FE68 = 9;
        func_800375A8(arg0, 0x23);
    }
    if (arg0->u.walker.frame == 20 && D_8011551C == NULL) {
        D_8011551C = &D_80074ACC;
        osSendMesg(&D_8013EEF8, 0, 1);
    }
    if (arg0->u.walker.frame >= 20) {
        if (arg0->u.walker.turning != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(&D_8012F118);
            }
            func_800375A8(arg0, 1);
            return;
        }
        return;
    }
    dx = arg0->u.walker.unk_1F0.l0.unk_1F0->pos.x - arg0->pos.x;
    dz = arg0->u.walker.unk_1F0.l0.unk_1F0->pos.z - arg0->pos.z;
    dz = func_80069920(dz, dx);
    arg0->u.walker.goalDir.x = SINF(dz);
    arg0->u.walker.goalDir.z = COSF(dz);
}
