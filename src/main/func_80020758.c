typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s32 m[16]; } Mtx;
typedef struct Node { u8 unk_00[0x18]; s32 count; struct Node *children; Gfx *dl; } Node;

void func_8001FDA0(s32, Node *, s32);
void func_80056F00(s32, Mtx *);
extern Gfx *D_8013EC7C;

/* Recursively emits matrix load, display list and pop matrix for each node of a tree. */
Mtx *func_80020758(Node *node, s32 count, s32 arg2, s32 arg3, Mtx *mtx) {
    s32 i;
    Gfx *g;

    for (i = 0; i < count; i++) {
        func_8001FDA0(arg2, node, arg3);
        func_80056F00(arg2, mtx);
        g = D_8013EC7C++;
        g->w0 = 0x01040040;
        g->w1 = (u32) mtx;
        mtx++;
        g = D_8013EC7C++;
        g->w0 = 0x06000000;
        g->w1 = (u32) node->dl;
        if (node->count != 0) {
            mtx = func_80020758(node->children, node->count, arg2, arg3, mtx);
        }
        g = D_8013EC7C++;
        g->w0 = 0xBD000000;
        g->w1 = 0;
        node++;
    }
    return mtx;
}
