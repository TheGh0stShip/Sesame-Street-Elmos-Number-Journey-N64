/* alHeapDBAlloc: 16-byte aligned bump allocation from an ALHeap */
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap;
void *func_8005C810(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size) {
    s32 bytes;
    u8 *ptr = 0;
    bytes = (num * size + 15) & ~15;
    if ((hp->cur + bytes) <= (hp->base + hp->len)) {
        ptr = hp->cur;
        hp->cur += bytes;
    }
    return ptr;
}
