/* byte copy: copy n bytes from src to dst */
void func_80065A20(u8 *dst, u8 *src, s32 n)
{
    while (n--) {
        *dst++ = *src++;
    }
}
