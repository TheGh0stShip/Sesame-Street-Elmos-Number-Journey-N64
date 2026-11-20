/* set up entry idx of p->unk_34 (its sub-object at 0x20), register it, and return it */
typedef struct { char pad0[0x2C]; } Sub;
typedef struct { char pad0[0x20]; Sub sub; } Elem; /* 0x4C bytes */
typedef struct { char pad0[0x30]; void *unk_30; Elem *unk_34; } P;
void func_80059614(Sub *, s32, s32);
void func_8005BE24(Sub *, s32, Elem *);
void func_8005B7D0(void *, s32, Sub *);
Sub *func_8005CEF0(P *p, s16 idx, s32 a, s32 b)
{
    func_80059614(&p->unk_34[idx].sub, a, b);
    func_8005BE24(&p->unk_34[idx].sub, 1, &p->unk_34[idx]);
    func_8005B7D0(p->unk_30, 2, &p->unk_34[idx].sub);
    return &p->unk_34[idx].sub;
}
