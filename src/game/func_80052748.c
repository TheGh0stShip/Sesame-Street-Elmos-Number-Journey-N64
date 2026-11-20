#include "common.h"

typedef struct {
    s32 unk_0;
    s32 unk_4;
    void *unk_8;
    void *unk_C;
} Req;

typedef struct {
    void (*unk_0)(void *);
    void (*unk_4)(void);
    void (*unk_8)(Req *);
} Ops;

typedef struct {
    s32 unk_0;
    s32 unk_4;
} Slot;
s32 func_800530AC(s32, s32 *, s32, s32);
s32 func_80056280(s32, s32 *, s32, s32);
void osAiSetNextBuffer(s32, s32);
extern Ops *D_800778FC;
extern Slot *D_80077900;
extern Slot *D_800A07A4;
extern s32 D_800A07A8;

/* main frame loop: waits for a frame, builds it into one of three slots and submits it */
void func_80052748(void) {
    Req req;
    s32 done;
    s32 ready;
    u32 len;
    u32 i;
    Slot *slot;
    s32 end;

    req.unk_8 = D_8006E200;
    req.unk_C = D_80079A10;
    req.unk_0 = D_800A07A8;
    i = 0;
    D_800778FC->unk_0(D_800778FC);
    for (;;) {
        do {
            D_800778FC->unk_4();
            ready = func_8005F240();
            len = func_8005F230() >> 2;
        } while (ready < 0);
        func_80052184();
        if ((D_80077900 != NULL) & (done != 0)) {
            osAiSetNextBuffer(D_80077900->unk_0, D_80077900->unk_4 * 4);
        }
        slot = &D_800A07A4[i];
        slot->unk_4 = func_800536C4(len);
        end = func_80056280(D_800A07A8, &done, osVirtualToPhysical(slot->unk_0), slot->unk_4);
        func_80052B44();
        end = func_800530AC(end, &done, osVirtualToPhysical(slot->unk_0), slot->unk_4);
        if (done != 0) {
            req.unk_4 = ((end - D_800A07A8) >> 3) * 8;
            D_800778FC->unk_8(&req);
            D_80077900 = slot;
        }
        i = (i + 1) % 3;
    }
}
