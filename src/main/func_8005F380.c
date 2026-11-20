/* osAiSetNextBuffer */
extern u8 D_80077F20; /* hdwrBugFlag */
s32 func_80063730(void); /* __osAiDeviceBusy */
u32 func_80059460(void *); /* osVirtualToPhysical */
#define IO_WRITE(a,v) (*(volatile u32 *)(0xA0000000 | (a)) = (v))
s32 func_8005F380(void *bufPtr, u32 size)
{
    char *bptr = bufPtr;
    if (D_80077F20 != 0)
        bptr -= 0x2000;
    if ((((u32)bufPtr + size) & 0x1fff) == 0)
        D_80077F20 = 1;
    else
        D_80077F20 = 0;
    if (func_80063730())
        return -1;
    IO_WRITE(0x04500000, func_80059460(bptr));
    IO_WRITE(0x04500004, size);
    return 0;
}
