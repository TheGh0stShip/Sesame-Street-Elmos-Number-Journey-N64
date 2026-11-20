/* alHeapInit */
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
void func_8005C7D0(ALHeap *hp, u8 *base, s32 len)
{
    s32 extraAlign = (15+1) - ((s32) base & 15);

    if (extraAlign != 15+1)
        hp->base = base + extraAlign;
    else
        hp->base = base;

    hp->len  = len;
    hp->cur  = hp->base;
    hp->count = 0;
}
