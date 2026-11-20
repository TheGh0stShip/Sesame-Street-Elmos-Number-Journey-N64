typedef struct { s32 unk_0; s32 unk_4; } T;
extern T *D_80078064;
s32 func_80062C60(void);
void func_80062C80(s32);
/* returns current thread field 4 with interrupts disabled */
s32 func_80063C40(void) {
    s32 mask = func_80062C60();
    s32 r = D_80078064->unk_4;
    func_80062C80(mask);
    return r;
}
