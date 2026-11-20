/* osPiRawReadIo */
s32 func_80068440(void); /* __osPiDeviceBusy */
s32 func_800637F0(u32 devAddr, u32 *data) {
    if (func_80068440()) {
        return -1;
    }
    *data = *(volatile u32 *)(devAddr | 0xA0000000);
    return 0;
}
