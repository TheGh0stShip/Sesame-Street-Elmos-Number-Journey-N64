typedef struct { u8 pad[0xC]; s32 unk_C; s32 unk_10; } Obj;
Obj *func_800522CC(s32);
void func_80059460(s32, Obj *);
void func_80052260(s32 a) {
    Obj *o = func_800522CC(a);
    s32 v;
    if (o == NULL) {
        v = a;
    } else {
        if ((a & 0xFF000000) == 0xFF000000) { a &= 0xFFFFFF; a += 0x140000; }
        v = o->unk_10; v += a; v -= o->unk_C;
    }
    func_80059460(v, o);
}
