/* sprintf */
typedef char *va_list; /* gcc 2.7 va-mips.h */
#define va_start(ap, last) (ap = (va_list)__builtin_next_arg(last))
char *func_80060F08(char *dst, const char *src, u32 count); /* proutSprintf */
s32 func_80067910(void *prout, void *arg, const char *fmt, va_list ap); /* _Printf */
s32 func_80060EB0(char *s, const char *fmt, ...) {
    s32 ans;
    va_list ap;
    va_start(ap, fmt);
    ans = func_80067910(func_80060F08, s, fmt, ap);
    if (ans >= 0) {
        s[ans] = 0;
    }
    return ans;
}
