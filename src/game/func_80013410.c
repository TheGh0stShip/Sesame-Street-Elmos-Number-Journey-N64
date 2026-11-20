#include "common.h"

extern SndCmd *D_8013E7F0;

/* sound sequencing thread: plays a command list, waiting on a timer between entries */
void func_80013410(void *arg0) {
    void *msg;

    osCreateMesgQueue(D_8013EEF8, D_80109D60, 1);
    (*(SndCmd *(*))&D_8011551C) = NULL;
    while (1) {
        osRecvMesg(D_8013EEF8, NULL, 1);
        if ((*(SndCmd *(*))&D_8011551C) != NULL) {
            if ((*(SndCmd *(*))&D_8011551C)->unk_04 == 0) {
                (*(SndCmd *(*))&D_8011551C) = NULL;
            } else {
                if ((*(SndCmd *(*))&D_8011551C)->unk_00 == -1) {
                    osSetTimer(D_8012F118, (s64)(*(SndCmd *(*))&D_8011551C)->unk_04 * 46875, 0, D_8013EEF8, NULL);
                } else if ((*(SndCmd *(*))&D_8011551C)->unk_04 == -1) {
                    if ((*(SndCmd *(*))&D_80115404) != NULL) {
                        func_80052D88(D_800A7F68);
                        D_800A7F68 = func_80052C84((*(SndCmd *(*))&D_80115404)->unk_00, 0x40, 0x7FFF);
                        D_80109DF6 = 0x7FFF;
                        osSetTimer(D_8012F118, (s64)(*(SndCmd *(*))&D_80115404)->unk_04 * 46875, 0, D_8013EEF8, NULL);
                    }
                } else if ((*(SndCmd *(*))&D_8011551C)->unk_04 == -2) {
                    if (D_8013E7F0 != NULL) {
                        func_80052D88(D_800A7F68);
                        D_800A7F68 = func_80052C84(D_8013E7F0->unk_00, 0x40, 0x7FFF);
                        D_80109DF6 = 0x7FFF;
                        osSetTimer(D_8012F118, (s64)D_8013E7F0->unk_04 * 46875, 0, D_8013EEF8, NULL);
                    }
                } else {
                    func_80052D88(D_800A7F68);
                    D_800A7F68 = func_80052C84((*(SndCmd *(*))&D_8011551C)->unk_00, 0x40, 0x7FFF);
                    D_80109DF6 = 0x7FFF;
                    osSetTimer(D_8012F118, (s64)(*(SndCmd *(*))&D_8011551C)->unk_04 * 46875, 0, D_8013EEF8, NULL);
                }
                (*(SndCmd *(*))&D_8011551C)++;
            }
        }
    }
}
