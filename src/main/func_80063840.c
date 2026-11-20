/* osSpTaskLoad: translate task pointers to physical, write back, DMA the boot ucode and task to the RSP */
typedef struct {
    u32 type; u32 flags; u64 *ucode_boot; u32 ucode_boot_size;
    u64 *ucode; u32 ucode_size; u64 *ucode_data; u32 ucode_data_size;
    u64 *dram_stack; u32 dram_stack_size; u64 *output_buff; u64 *output_buff_size;
    u64 *data_ptr; u32 data_size; u64 *yield_data_ptr; u32 yield_data_size;
} OSTask_t;
typedef union { OSTask_t t; long long force_structure_alignment; } OSTask;

extern OSTask D_800A2490; /* tmp_task */
void func_800675F0(void *src, void *dst, s32 len); /* bcopy */
u32 func_80059460(void *p); /* osVirtualToPhysical */
void func_80062D90(void *p, s32 len); /* osWritebackDCache */
void func_80068470(u32 status); /* __osSpSetStatus */
s32 func_80068480(u32 pc); /* __osSpSetPc */
s32 func_80068500(s32 dir, u32 devAddr, void *dramAddr, u32 size); /* __osSpRawStartDma */
s32 func_80068440(void); /* __osSpDeviceBusy */

#define _osVirtualToPhysical(ptr) \
    if (ptr != NULL) { ptr = (void *)func_80059460(ptr); }

static OSTask *_VirtualToPhysicalTask(OSTask *intp)
{
    OSTask *tp = &D_800A2490;
    func_800675F0(intp, tp, sizeof(OSTask));
    _osVirtualToPhysical(tp->t.ucode);
    _osVirtualToPhysical(tp->t.ucode_data);
    _osVirtualToPhysical(tp->t.dram_stack);
    _osVirtualToPhysical(tp->t.output_buff);
    _osVirtualToPhysical(tp->t.output_buff_size);
    _osVirtualToPhysical(tp->t.data_ptr);
    _osVirtualToPhysical(tp->t.yield_data_ptr);
    return tp;
}

void func_80063840(OSTask *intp)
{
    OSTask *tp = _VirtualToPhysicalTask(intp);

    if (tp->t.flags & 1) { /* OS_TASK_YIELDED */
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;
        intp->t.flags &= ~1;
        if (tp->t.flags & 4) { /* OS_TASK_LOADABLE */
            tp->t.ucode = (u64 *)*(volatile u32 *)(((u32)intp->t.yield_data_ptr + 0xC00 - 4) | 0xA0000000);
        }
    }
    func_80062D90(tp, sizeof(OSTask));
    func_80068470(0x2B00);
    while (func_80068480(0x04001000) == -1) {
    }
    while (func_80068500(1, 0x04001000 - sizeof(*tp), tp, sizeof(OSTask)) == -1) {
    }
    while (func_80068440()) {
    }
    while (func_80068500(1, 0x04001000, tp->t.ucode_boot, tp->t.ucode_boot_size) == -1) {
    }
}
