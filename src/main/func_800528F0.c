/* Audio heap setup: clear a block, then alHeapInit on the global heap. */
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap;
extern ALHeap D_800A07B0;
void func_80052990(void *dst, s32 val, s32 len);
void func_8005C7D0(ALHeap *hp, u8 *base, s32 len); /* alHeapInit */
void func_800528F0(u8 *base, s32 len) {
    func_80052990(base, 0, len);
    func_8005C7D0(&D_800A07B0, base, len);
}
