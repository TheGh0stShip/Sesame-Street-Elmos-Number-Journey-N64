/* proutSprintf: copies count bytes and returns the end of the destination */
void *func_80060E7C(void *dst, const void *src, u32 n); /* memcpy */
char *func_80060F08(char *dst, const char *src, u32 count) {
    func_80060E7C(dst, src, count);
    return dst + count;
}
