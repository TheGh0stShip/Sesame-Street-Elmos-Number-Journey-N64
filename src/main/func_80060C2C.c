/* Wait until one byte has been sent over rdb (__osRdbSend). */
s32 func_80063440(u8 *buf, u32 size, u32 type); /* __osRdbSend */
void func_80060C2C(void) {
    u8 buf[4];
    s32 sent = 0;
    do {
        sent += func_80063440(buf, 1, 10);
    } while (sent <= 0);
}
