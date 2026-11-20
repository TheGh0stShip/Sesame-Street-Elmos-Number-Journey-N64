typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct {
    u32 type; u32 flags;
    void *ucode_boot; u32 ucode_boot_size;
    void *ucode; u32 ucode_size;
    void *ucode_data; u32 ucode_data_size;
    void *dram_stack; u32 dram_stack_size;
    void *output_buff; void *output_buff_size;
    void *data_ptr; u32 data_size;
    void *yield_data_ptr; u32 yield_data_size;
} OSTask_t;
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

extern Gfx *D_8013EF28;
extern Gfx *D_8013EC7C;
extern u8 D_80076B60;
extern u8 D_800AD03A;
extern s32 D_80115408;
extern s32 D_8012F154;
extern u32 D_80075094;
extern u8 D_80179E00[], D_80179C00[];
extern u8 D_80154200[], D_80154000[];
extern u8 D_80167000[], D_80166E00[];
extern u8 D_801A9020[], D_801A8E20[];
extern u8 D_8018CC00[], D_8018CA00[];
extern s32 D_80106C7C;
extern char D_80079F18[];
extern char D_80079F24[];
extern char D_80079F34[];
extern char D_80079F38[];
extern Gfx D_80075580[];
extern u8 D_80109E28;
extern u8 D_80109E30;
extern u8 D_800A91E4;
extern u8 D_8006CD00[], D_8006CDD0[], D_80079210[], D_8013E800[], D_80112EA0[], D_80114EA0[], D_8012F1E0[];
extern s32 D_80108D24;
extern u8 D_8010E650[];
extern void *D_800A7F14;
extern s32 D_8007D7A0;

void func_800013B0(void);
void func_80001424(void (*f)(void), s32 a, s32 b, s32 c, s32 d);
void func_80001AB4(void);
void func_8000ADBC(void);
void func_80041F04(s32 a);
void func_80041F84(s32 a);
void func_80001818(GfxFrame *f);
void func_800016C0(void);
void func_8001D3AC(void *a, void *b);
s32 func_8000E030(void);
void func_8000E2D0(s32 a);
void func_8000E264(s32 r, s32 g, s32 b, s32 a);
void func_8000E1EC(s32 x, s32 y);
s32 func_80060EB0(char *buf, const char *fmt, ...);
void func_8000E32C(char *s);
void func_8000E16C(s32 a);
void func_8001AC1C(void);
void func_800125D4(s32 a);
void func_80003E18(s32 a);
void func_80003B10(void);
void func_80001F60(void);
void func_80059540(const char *expr, const char *file, s32 line);
void func_80058580(void);
s32 func_80058BA0(void *q, void *msg, s32 flag);

/* Builds one frame's display list and sends the graphics task to the scheduler. */
s32 func_8000BA3C(GfxFrame *arg0) {
    OSScTask *task;
    s32 saved;
    char buf[0x68];
    Gfx *g;

    D_8013EF28 = arg0->unk_68;
    D_8013EC7C = arg0->unk_A8;
    func_800013B0();
    func_80001424(func_80001AB4, 0, 2, 0, 0);
    if (D_80076B60 == 1) {
        func_80001424(func_8000ADBC, 0, 6, 0, 0);
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
                    func_80060EB0(buf, D_80079F18);
                    func_8000E32C(buf);
                    g = D_8013EC7C++;
                    g->w0 = 0x06000000;
                    g->w1 = (u32)D_80075580;
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
        func_80060EB0(buf, D_80079F24);
        func_8000E32C(buf);
        g = D_8013EC7C++;
        g->w0 = 0x06000000;
        g->w1 = (u32)D_80075580;
        func_8000E16C(saved);
    }
    g = D_8013EC7C++;
    g->w0 = 0xE9000000;
    g->w1 = 0;
    g = D_8013EC7C++;
    g->w0 = 0xB8000000;
    g->w1 = 0;
    (D_8013EC7C - (D_8013EF28 + 8) < 0x1000) ? (void)0 : func_80059540(D_80079F34, D_80079F38, 4858);
    task = &arg0->task;
    task->list.data_ptr = D_8013EF28 + 8;
    task->list.data_size = (D_8013EC7C - (D_8013EF28 + 8)) * sizeof(Gfx);
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
    task->msgQ = D_8010E650;
    task->msg = arg0->unk_80A8;
    task->framebuffer = arg0->unk_80C8;
    task->totalTime = 0;
    func_80058580();
    func_80058BA0(D_800A7F14, task, 1);
    D_8007D7A0++;
    return 1;
}
