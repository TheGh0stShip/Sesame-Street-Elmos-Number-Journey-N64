/* osAiSetFrequency: program the AI DAC and bit rates for a sample rate */
extern s32 D_80077C08; /* osViClock */
#define IO_WRITE(addr, data) (*(volatile u32 *)(0xA0000000 | (addr)) = (u32)(data))
s32 func_8005F250(u32 frequency) {
    register unsigned int dacRate;
    register unsigned char bitRate;
    register float f;

    f = D_80077C08 / (float)frequency + .5f;
    dacRate = f;
    if (dacRate < 132) {
        return -1;
    }
    bitRate = dacRate / 66;
    if (bitRate > 16) {
        bitRate = 16;
    }
    IO_WRITE(0x04500010, dacRate - 1);
    IO_WRITE(0x04500014, bitRate - 1);
    IO_WRITE(0x04500008, 1);
    return D_80077C08 / (s32)dacRate;
}
