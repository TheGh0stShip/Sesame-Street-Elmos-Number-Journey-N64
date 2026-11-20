/* stores the pointer in a global and posts a type-2 message carrying it */
typedef struct { u8 type; void *ptr; } Msg;
void func_8004F044(Msg *m);
extern void *D_800A0574;
void func_8004ED20(void *p) {
    Msg m;
    m.type = 2;
    m.ptr = p;
    D_800A0574 = p;
    func_8004F044(&m);
}
