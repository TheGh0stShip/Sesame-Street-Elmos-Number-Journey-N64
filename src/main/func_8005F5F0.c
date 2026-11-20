/* osPiRawReadIo */
extern s32 func_800643A0(void);   /* __osPiDeviceBusy */
s32 func_8005F5F0(u32 devAddr, u32 *data)
{
    if (func_800643A0()) {
        return -1;
    }
    *data = *(volatile u32 *)(devAddr | 0xA0000000);
    return 0;
}
