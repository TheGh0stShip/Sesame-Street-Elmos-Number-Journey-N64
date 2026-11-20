#include "common.h"

typedef struct OSScTask {
    struct OSScTask *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask_t list;
    void *msgQ;
    void *msg;
    u64 startTime;
    u64 totalTime;
} OSScTask;
typedef struct {
    OSScTask task;
    Gfx unk_68[8];
    Gfx unk_A8[0x1000];
    u8 unk_80A8[0x20];
    void *unk_80C8;
} GfxFrame;
extern u8 D_80179E00[], D_80179C00[];
extern u8 D_80154200[], D_80154000[];
extern u8 D_80167000[], D_80166E00[];
extern u8 D_801A9020[], D_801A8E20[];
extern u8 D_8018CC00[], D_8018CA00[];
extern u8 D_8006CD00[], D_8006CDD0[], D_80079210[], D_8013E800[], D_80112EA0[], D_80114EA0[], D_8012F1E0[];

/* Builds one frame's display list and sends the graphics task to the scheduler. */
s32 func_8000BA3C(GfxFrame *arg0) {
    OSScTask *task;
    s32 saved;
    char buf[0x68];
    Gfx *g;

    (*(Gfx *(*))&D_8013EF28) = arg0->unk_68;
    (*(Gfx *(*))&D_8013EC7C) = arg0->unk_A8;
    func_800013B0();
    addActorDL(func_80001AB4, 0, 2, 0, 0);
    if (D_80076B60 == 1) {
        addActorDL(func_8000ADBC, 0, 6, 0, 0);
    }
    D_800AD03A = 1;
    func_80041F04(D_80115408);
    func_80001818(arg0);
    func_800016C0();
    func_80041F84(D_80115408);
    if (D_8012F154 == 0x15) {
        switch (D_80075094) {
            case 0:
                func_8001D3AC(D_80179E00, D_80179C00);
                break;
            case 1:
                func_8001D3AC(D_80154200, D_80154000);
                break;
            case 2:
                func_8001D3AC(D_80167000, D_80166E00);
                break;
            case 3:
                func_8001D3AC(D_801A9020, D_801A8E20);
                break;
            case 4:
                func_8001D3AC(D_8018CC00, D_8018CA00);
                if (!(D_80106C7C & 0x40)) {
                    saved = func_8000E030();
                    func_8000E2D0(1);
                    func_8000E264(255, 255, 255, 255);
                    func_8000E1EC(0x74, 0xC8);
                    sprintf(buf, D_80079F18);
                    func_8000E32C(buf);
                    g = (*(Gfx *(*))&D_8013EC7C)++;
                    g->words.w0 = 0x06000000;
                    g->words.w1 = (u32)(*(Gfx (*)[])&D_80075580);
                    func_8000E16C(saved);
                }
                break;
            default:
                func_8001D3AC(D_8018CC00, D_8018CA00);
                break;
        }
    }
    if (D_8012F154 == 0x16) {
        func_8001AC1C();
    }
    if (D_80109E28 == 0) {
        func_800125D4(0);
    }
    if (D_800A91E4 == 0) {
        func_80003E18(0);
        func_80003B10();
    } else {
        func_80003B10();
        func_80003E18(0);
    }
    if (D_80109E28 != 0) {
        func_800125D4(0);
    }
    func_80001F60();
    if (D_80109E30 != 0) {
        saved = func_8000E030();
        func_8000E2D0(1);
        func_8000E264(255, 255, 255, 255);
        func_8000E1EC(0x6C, 0x70);
        sprintf(buf, D_80079F24);
        func_8000E32C(buf);
        g = (*(Gfx *(*))&D_8013EC7C)++;
        g->words.w0 = 0x06000000;
        g->words.w1 = (u32)(*(Gfx (*)[])&D_80075580);
        func_8000E16C(saved);
    }
    g = (*(Gfx *(*))&D_8013EC7C)++;
    g->words.w0 = 0xE9000000;
    g->words.w1 = 0;
    g = (*(Gfx *(*))&D_8013EC7C)++;
    g->words.w0 = 0xB8000000;
    g->words.w1 = 0;
    ((*(Gfx *(*))&D_8013EC7C) - ((*(Gfx *(*))&D_8013EF28) + 8) < 0x1000) ? (void)0 : func_80059540(D_80079F34, D_80079F38, 4858);
    task = &arg0->task;
    task->list.data_ptr = (*(Gfx *(*))&D_8013EF28) + 8;
    task->list.data_size = ((*(Gfx *(*))&D_8013EC7C) - ((*(Gfx *(*))&D_8013EF28) + 8)) * sizeof(Gfx);
    task->list.type = 1;
    task->list.flags = 0;
    task->list.ucode_boot = D_8006CD00;
    task->list.ucode_boot_size = D_8006CDD0 - D_8006CD00;
    task->list.ucode = D_8006CDD0;
    task->list.ucode_data = D_80079210;
    task->list.ucode_data_size = 0x800;
    task->list.dram_stack = D_8013E800;
    task->list.dram_stack_size = 0x400;
    task->list.output_buff = D_80112EA0;
    task->list.output_buff_size = D_80114EA0;
    task->list.yield_data_ptr = D_8012F1E0;
    task->list.yield_data_size = 0xC00;
    task->flags = D_80108D24 ? 0x63 : 0x23;
    task->next = NULL;
    task->msgQ = (*(u8 (*)[])&D_8010E650);
    task->msg = arg0->unk_80A8;
    task->framebuffer = arg0->unk_80C8;
    task->totalTime = 0;
    osWritebackDCacheAll();
    osSendMesg(D_800A7F14, task, 1);
    D_8007D7A0++;
    return 1;
}
