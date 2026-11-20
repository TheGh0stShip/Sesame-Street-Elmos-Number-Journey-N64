/* __rmonSendData: send a block to the host word by word (RSP memory read via SP raw IO) */
extern s32 func_800637F0(u32 devAddr, u32 *data); /* __osSpRawReadIo */
extern void func_80060C68(u32 w);                 /* __rmonIOputw */
extern void func_80065A20(u8 *dst, u8 *src, u32 n); /* __rmonMemcpy */
extern void func_80060C2C(void);                  /* __rmonIOflush */

void func_80060950(char *const block, unsigned int blockSize)
{
    int *blockPointer = (int *)block;
    unsigned int wordCount = (u32)(blockSize + 3) / 4;
    u32 data;
    union {
        char bufBytes[4];
        u32 bufWord;
    } buffer;

    if (((u32)block & 3) == 0) {
        while (wordCount--) {
            if ((u32)blockPointer >= 0x04000000 && (u32)blockPointer < 0x05000000) {
                func_800637F0((u32)blockPointer++, &data);
                func_80060C68(data);
            } else {
                func_80060C68(*(blockPointer++));
            }
        }
    } else
        while (wordCount--) {
            func_80065A20((u8 *)buffer.bufBytes, (u8 *)blockPointer, sizeof(buffer));
            func_80060C68(buffer.bufWord);
            blockPointer++;
        }
    func_80060C2C();
}
