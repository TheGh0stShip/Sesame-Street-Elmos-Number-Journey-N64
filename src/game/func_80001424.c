#include "common.h"

typedef struct Node {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ struct Node *prev;
    /* 0x14 */ struct Node *next;
} Node;

typedef struct {
    /* 0x00 */ Node *head;
    /* 0x04 */ s32 unk4;
} NodeList;

extern NodeList D_8006F688[];
extern Node D_8007BFA0[];

/* takes a node from the pool and links it into list arg2; lists 4 and 5 are kept sorted by arg4 */
void addActorDL(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    Node *node;
    Node *it;

    if ((u32)arg2 >= 8) {
        return;
    }
    if (D_8006F6C8 >= 256) {
        osSyncPrintf(D_80079D00);
        return;
    }
    node = &D_8007BFA0[D_8006F6C8++];
    node->unk4 = arg4;
    node->unk0 = arg3;
    node->unkC = arg1;
    node->unk8 = arg0;
    node->prev = NULL;
    node->next = NULL;
    if ((it = D_8006F688[arg2].head) == NULL) {
        D_8006F688[arg2].head = node;
        return;
    }
    if (arg2 == 4 || arg2 == 5) {
        while (it->next != NULL && (arg4 == 0.0 || it->unk4 < arg4)) {
            it = it->next;
        }
        if (it->next == NULL) {
            node->prev = it;
            it->next = node;
        } else {
            node->next = it;
            node->prev = it->prev;
            it->prev = node;
            if (node->prev == NULL) {
                D_8006F688[arg2].head = node;
            } else {
                node->prev->next = node;
            }
        }
    } else {
        while (it->next != NULL) {
            it = it->next;
        }
        it->next = node;
        node->prev = it;
    }
}
