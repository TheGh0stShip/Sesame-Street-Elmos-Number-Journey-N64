typedef struct { u8 pad[0xB9]; u8 unk_B9; } S;
u8 func_800504E8(u8);
u8 *func_800514D8(S *p, u8 *ptr) {
    p->unk_B9 = func_800504E8(*ptr++);
    p->unk_B9 += *ptr++;
    return ptr;
}
