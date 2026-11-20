typedef struct Node {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
} Node;

typedef struct Pool {
    s32 unk_0;
    s32 unk_4;
    Node *unk_8;
    Node *unk_C;
} Pool;

extern Pool D_80115368[];
extern u32 D_8013C724;

/* registers a 16-byte-aligned buffer as the next pool (up to 8) */
void func_80042648(s32 arg0, s32 arg1) {
    Node *node;

    if ((arg0 & 15) != 0) {
        return;
    }
    if (D_8013C724 >= 8) {
        return;
    }
    node = (Node *)arg0;
    node->unk_0 = 0;
    node->unk_4 = 0;
    node->unk_8 = 0;
    node->unk_C = 0;
    D_80115368[D_8013C724].unk_8 = node;
    D_80115368[D_8013C724].unk_C = node;
    D_80115368[D_8013C724].unk_0 = arg1;
    D_80115368[D_8013C724].unk_4 = (s32)D_80115368[D_8013C724].unk_C + arg1;
    D_8013C724++;
}
