typedef struct Node { struct Node *next; } Node;
typedef struct { u8 unk_00[0x2C]; Node *freeList; } Pool;
extern Pool *D_80077BC4;

/* push a node onto the pool's free list */
void func_80056414(Node *n) {
    n->next = D_80077BC4->freeList;
    D_80077BC4->freeList = n;
}
