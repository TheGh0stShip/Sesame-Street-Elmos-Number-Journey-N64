typedef struct Node { struct Node *prev; struct Node *next; s32 count; } Node;
extern Node *D_800A05A0;
extern Node *D_800A05A4;
extern s32 D_800A05B8;
extern u8 D_800A05C0[];
s32 func_80058A70(void *mq, void *msg, s32 flag);
/* drain pending messages, then tick down timers moving expired nodes to the free list */
void func_80052184(void) {
    void *msg;
    Node *n;
    Node *next;

    while (D_800A05B8 != 0) {
        func_80058A70(D_800A05C0, &msg, 0);
        D_800A05B8--;
    }
    n = D_800A05A0;
    while (n != NULL) {
        if (--n->count == 0) {
            next = n->next;
            if (next != NULL) next->prev = n->prev;
            if (n->prev != NULL) n->prev->next = n->next;
            else D_800A05A0 = n->next;
            n->prev = NULL;
            n->next = D_800A05A4;
            D_800A05A4 = n;
            n = next;
        } else {
            n = n->next;
        }
    }
}
