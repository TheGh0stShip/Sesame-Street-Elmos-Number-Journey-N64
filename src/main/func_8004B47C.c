typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct Node {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    f32 unk_0C;
    Vec3 unk_10;
    Vec3 unk_1C;
    struct Node *next;
} Node;

typedef struct List {
    u8 unk_00[0xC];
    Node *head;
} List;

void *func_80042080(s32 size);
void func_80069C70(void *p, s32 c, s32 n);

/* allocates a 44-byte node and appends it to the end of list->head */
void func_8004B47C(List *list, Vec3 *arg1, Vec3 *arg2, s32 arg3, s32 arg4, f32 arg5) {
    register Node *it;
    register Node *node;

    if (list == NULL) {
        return;
    }
    node = func_80042080(44);
    if (node == NULL) {
        return;
    }
    func_80069C70(node, 0, 44);
    node->unk_00 = arg3;
    node->unk_04 = arg4;
    node->unk_0C = arg5;
    if (arg1 != NULL) {
        node->unk_10.x = arg1->x;
        node->unk_10.y = arg1->y;
        node->unk_10.z = arg1->z;
    }
    if (arg2 != NULL) {
        node->unk_1C.x = arg2->x;
        node->unk_1C.y = arg2->y;
        node->unk_1C.z = arg2->z;
        node->unk_08 = 1;
    } else {
        node->unk_08 = 0;
    }
    node->next = NULL;
    it = list->head;
    if (it == NULL) {
        list->head = node;
    } else {
        while (it->next != NULL) {
            it = it->next;
        }
        it->next = node;
    }
}
