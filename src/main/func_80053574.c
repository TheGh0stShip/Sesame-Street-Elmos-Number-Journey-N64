typedef struct { u8 unk_00[0x18]; } Mtx18;
typedef struct { u8 unk_00[0x8]; } Buf8;
void func_800585B0(Mtx18 *, Buf8 *, s32);
void func_800583D0(s32, s32);
void func_80060780(Mtx18 *, s32, s32, s32, s32, s32, Mtx18 *);
void func_80058A70(Mtx18 *, s32, s32);
void func_80058BA0(s32, s32, s32);

void func_80053574(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Mtx18 a;
    Mtx18 b;
    Buf8 c;
    func_800585B0(&a, &c, 1);
    func_800583D0(arg1, arg2);
    func_80060780(&b, 0, 0, arg0, arg1, arg2, &a);
    func_80058A70(&a, 0, 1);
    func_80058BA0(arg3, 0, 1);
}
