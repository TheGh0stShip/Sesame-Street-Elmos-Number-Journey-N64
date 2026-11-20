typedef struct { u8 pad[0x1ED]; s8 unk_1ED; } S;
/* decrements a countdown */
void func_8003BD24(S *p) {
    if (p->unk_1ED > 0) {
        if (--p->unk_1ED == 0) {
        }
    }
}
