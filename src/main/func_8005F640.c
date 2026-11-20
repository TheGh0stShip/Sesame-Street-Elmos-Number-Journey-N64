/* __osSiRawWriteIo: write a word to an SI register unless SI is busy */
s32 func_800643A0(void); /* __osSiDeviceBusy */
s32 func_8005F640(u32 devAddr, u32 data)
{
    if (func_800643A0()) {
        return -1;
    }
    *(volatile u32 *)(devAddr | 0xA0000000) = data;
    return 0;
}
