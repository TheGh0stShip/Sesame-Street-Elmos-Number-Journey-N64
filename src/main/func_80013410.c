typedef struct SndCmd {
    s32 unk_00;
    s16 unk_04;
} SndCmd;
extern u8 D_8013EEF8[];
extern u8 D_80109D60[];
extern u8 D_8012F118[];
extern SndCmd *D_8011551C;
extern SndCmd *D_80115404;
extern SndCmd *D_8013E7F0;
extern u16 D_800A7F68;
extern s16 D_80109DF6;
void func_800585B0(void *, void *, s32);
s32 func_80058A70(void *, void *, s32);
void func_80058E00(void *, u64, u64, void *, void *);
void func_80052D88(u16);
u16 func_80052C84(s32, s32, s32);

/* sound sequencing thread: plays a command list, waiting on a timer between entries */
void func_80013410(void *arg0) {
    void *msg;

    func_800585B0(D_8013EEF8, D_80109D60, 1);
    D_8011551C = NULL;
    while (1) {
        func_80058A70(D_8013EEF8, NULL, 1);
        if (D_8011551C != NULL) {
            if (D_8011551C->unk_04 == 0) {
                D_8011551C = NULL;
            } else {
                if (D_8011551C->unk_00 == -1) {
                    func_80058E00(D_8012F118, (s64)D_8011551C->unk_04 * 46875, 0, D_8013EEF8, NULL);
                } else if (D_8011551C->unk_04 == -1) {
                    if (D_80115404 != NULL) {
                        func_80052D88(D_800A7F68);
                        D_800A7F68 = func_80052C84(D_80115404->unk_00, 0x40, 0x7FFF);
                        D_80109DF6 = 0x7FFF;
                        func_80058E00(D_8012F118, (s64)D_80115404->unk_04 * 46875, 0, D_8013EEF8, NULL);
                    }
                } else if (D_8011551C->unk_04 == -2) {
                    if (D_8013E7F0 != NULL) {
                        func_80052D88(D_800A7F68);
                        D_800A7F68 = func_80052C84(D_8013E7F0->unk_00, 0x40, 0x7FFF);
                        D_80109DF6 = 0x7FFF;
                        func_80058E00(D_8012F118, (s64)D_8013E7F0->unk_04 * 46875, 0, D_8013EEF8, NULL);
                    }
                } else {
                    func_80052D88(D_800A7F68);
                    D_800A7F68 = func_80052C84(D_8011551C->unk_00, 0x40, 0x7FFF);
                    D_80109DF6 = 0x7FFF;
                    func_80058E00(D_8012F118, (s64)D_8011551C->unk_04 * 46875, 0, D_8013EEF8, NULL);
                }
                D_8011551C++;
            }
        }
    }
}
