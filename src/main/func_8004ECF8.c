/* posts a type-1 message carrying one pointer argument (audio player command) */
typedef struct { u8 type; void *ptr; } Msg;
void func_8004F044(Msg *m);
void func_8004ECF8(void *p) {
    Msg m;
    m.type = 1;
    m.ptr = p;
    func_8004F044(&m);
}
