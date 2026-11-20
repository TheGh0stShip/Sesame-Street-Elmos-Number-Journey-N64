#include "common.h"

typedef struct Node {
    struct Node *unk_00;
    struct Node *unk_04;
    s32 unk_08;
    s32 unk_0C;
    void *unk_10;
} Node;

void *__MusIntMemMalloc(s32);
s32 osCartRomInit(void);
extern Node *D_800A05A4;
extern Node *D_800A05A8;

/* Allocates a pool of count nodes, each with a size-byte buffer, links them and returns a callback. */
void *func_80052010(s32 count, s32 size) {
    s32 i;

    D_800A05D8 = osCartRomInit();
    D_800A05AC = __MusIntMemMalloc(count * 0x30);
    D_800A05B0 = __MusIntMemMalloc(count * 8);
    D_800A05A8 = __MusIntMemMalloc(count * sizeof(Node));
    func_80052990(D_800A05A8, 0, count * sizeof(Node));
    for (i = 0; i < count - 1; i++) {
        D_800A05A8[i].unk_04 = &D_800A05A8[i] + 1;
        D_800A05A8[i].unk_04->unk_00 = &D_800A05A8[i];
        D_800A05A8[i].unk_10 = __MusIntMemMalloc(size);
        D_800A05A8[i].unk_0C = -1;
    }
    D_800A05A8[i].unk_10 = __MusIntMemMalloc(size);
    D_800A05A8[i].unk_0C = -1;
    D_800A05B4 = size;
    D_800A05B8 = 0;
    D_800A05A0 = 0;
    D_800A05A4 = D_800A05A8;
    osCreateMesgQueue(D_800A05C0, D_800A05B0, count * 2);
    return func_80052254;
}
