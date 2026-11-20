/* posts a type-0 message carrying one argument */
typedef struct {
    u8 type;
    s32 arg;
} Msg;
void func_8004F044(Msg *m);
void func_8004ECD4(s32 a) {
    Msg m;
    m.type = 0;
    m.arg = a;
    func_8004F044(&m);
}
