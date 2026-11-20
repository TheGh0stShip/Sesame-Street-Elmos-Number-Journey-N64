#include "common.h"
#include "game_structs.h"

extern u16 D_80107CBA;
/* per-frame update: input-triggered state changes, otherwise faces the target object */
void func_80038A50(Actor *o) {
    register f32 dx;
    register f32 dz;
    register Actor *p;
    if (D_80107CD8 == 0) {
        if (D_8006FE58 & 2) {
            if (D_8011551C == NULL) {
                D_80107CD8 = 1;
                o->u.walker.unk_200 = o->u.walker.unk_200 | 1;
                o->u.walker.unk_200 = o->u.walker.unk_200 | 2;
            }
        }
    } else if (o->u.walker.frame >= 50) {
        if ((u16)(D_80107CBA & 0xC00F)) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(D_8012F118);
            }
            D_8012FE68 = 5;
            func_800375A8(o, 0x23);
            return;
        }
    }
    if (o->u.walker.frame == 20) {
        if (D_8011551C == NULL) {
            D_8011551C = D_80074B6C;
            osSendMesg(D_8013EEF8, 0, 1);
        }
    }
    if (o->u.walker.frame >= 20) {
        if (o->u.walker.turning != 0) {
            if (D_8011551C != NULL) {
                D_8013C7C0 = D_80109DF6 >> 3;
                osStopTimer(D_8012F118);
            }
            func_800375A8(o, 1);
            return;
        }
        return;
    }
    dx = o->u.walker.unk_1F0.l0.unk_1F0->pos.x - o->pos.x;
    dz = o->u.walker.unk_1F0.l0.unk_1F0->pos.z - o->pos.z;
    dz = func_80069920(dz, dx);
    p = o; p->u.walker.goalDir.x = __cosf(dz);
    p = o; p->u.walker.goalDir.z = __sinf(dz);
}
